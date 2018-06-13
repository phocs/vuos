
#include <dlfcn.h>
#include <pthread.h>

#include "vufusedents.h"
#include "vufuse.h"

struct vu_module_t vu_module = {
		.name = "vufuse",
		.description = "/vufuse virtual file systems (user level FUSE)"
	};

/* values for INUSE and thread synchro */
#define WAITING_FOR_LOOP -1
#define EXITING -2
#define FUSE_ABORT -3
static pthread_mutex_t condition_mutex = PTHREAD_MUTEX_INITIALIZER;

struct startmainopt {
	struct fuse_context *new;
	char *source;
	unsigned long *pmountflags;
	void *data;
	struct vuht_entry_t *mount_hte;
};

static char *mountflag2options(unsigned long mountflags, void *data);
int vufuse_abort(struct fuse *f);

static void *startmain(void *vsmo)
{

	struct startmainopt *psmo = (struct startmainopt *) vsmo;
	struct fuse *new_fuse = psmo->new->fuse;
	int (*pmain)() = dlsym(new_fuse->dlhandle,"main");
	char *opts;
	int newargc;
	char **newargv;
	char **newnewargv;
	int i;

	if (pmain == NULL) {
		printk("dlerror ! %s\n", dlerror());
		vufuse_abort(new_fuse);
		return NULL;
	}

	/*each thread has a hash table element variable, startmain lives in a new thread so we have to set it's hte var.*/
	/*preventing fuse_get_contex to return NULL and cause a segfault*/
	vu_mod_setht(psmo->mount_hte);
	/* handle -o options and specific filesystem options */
	opts = mountflag2options(*(psmo->pmountflags), psmo->data);

	newargc=fuseargs(new_fuse->filesystemtype,psmo->source, new_fuse->path,opts, &newargv,psmo->new, &(new_fuse->flags), &(new_fuse->exceptions));
	free(opts);

	/* some modules could change argv! */
	if ((newnewargv=malloc(newargc * sizeof (char *))) != NULL) {
		for (i=0;i<newargc;i++)
			newnewargv[i]=newargv[i];
		optind=0;
		/*calling submodule main function */
		if (pmain(newargc,newnewargv) != 0)
			vufuse_abort(new_fuse);

		free(newnewargv);
	}
	fusefreearg(newargc,newargv);
	pthread_exit(NULL);
	return NULL;
}


int vu_vufuse_mount(const char *source, const char *target, const char *filesystemtype,
	 unsigned long mountflags, const void *data) {

		printk("vu_vufuse_mount %s %s %s\n", source, target, filesystemtype);

	struct stat target_stat;
#if 0
    if ((vu_stat(target, &target_stat) != 0) || !S_ISDIR(target_stat.st_mode))
			ERRNO_N_RETURN(ENOTDIR,-1)
#endif
		printk("filesystem type [%s]", filesystemtype);
    void *dlhandle = vu_mod_dlopen(filesystemtype, RTLD_NOW);

    if(dlhandle == NULL || dlsym(dlhandle,"main") == NULL) {
		printk("%s",dlerror());
		if (dlhandle != NULL) dlclose(dlhandle);
		ERRNO_N_RETURN(ENODEV,-1)

	} else {

		struct startmainopt smo;
		struct vuht_entry_t *ht;
		struct vu_service_t *s = vu_mod_getservice();
		struct fuse_context *new = (struct fuse_context *) malloc(sizeof(struct fuse_context));
		struct fuse *new_fuse;
		assert(new);

		new->fuse = (struct fuse *)malloc(sizeof(struct fuse));
		assert(new->fuse);
		new_fuse = new->fuse;
		new_fuse->path = strdup(target);
		new_fuse->exceptions = NULL;
		if (strcmp(target,"/")==0) new_fuse->pathlen = 0;
		else new_fuse->pathlen = strlen(target);
		new_fuse->filesystemtype = strdup(filesystemtype);
		new_fuse->dlhandle = dlhandle;
		memset(&new_fuse->fops,0,sizeof(struct fuse_operations));
		new_fuse->inuse = WAITING_FOR_LOOP;

		new->uid = getuid();
		new->gid = getgid();
		new->pid = umvu_gettid();

		new->private_data = NULL;
		new_fuse->flags = mountflags;

		//confirm function = NULL
		ht = vuht_pathadd(CHECKPATH, source, target, filesystemtype, mountflags, data, s, 0, NULL, new);
		vu_mod_setht(ht);

		smo.new = new;
		smo.pmountflags = &(new_fuse->flags);
		smo.source = (char *) source;
		smo.data = (void *) data;
		smo.mount_hte = ht;

		pthread_cond_init(&(new_fuse->startloop),NULL);
		pthread_cond_init(&(new_fuse->endloop),NULL);
		pthread_mutex_init(&(new_fuse->endmutex),NULL);
		pthread_create(&(new_fuse->thread), NULL, startmain, (void *)&smo);

		pthread_mutex_lock( &condition_mutex );
		if (new_fuse->inuse== WAITING_FOR_LOOP)
			pthread_cond_wait( &(new_fuse->startloop), &condition_mutex);
		pthread_mutex_unlock( &condition_mutex );

		if (new_fuse->inuse == FUSE_ABORT)
		{
			/*clean up function and umount_internal will free new->fuse*/
			vuht_del(ht,1);

			ERRNO_N_RETURN(EIO,-1)
		}

		if (new_fuse->fops.init != NULL) {
			struct fuse_conn_info conn;
			new->private_data=new_fuse->fops.init(&conn);
		}

		printk("\n"); //solo per questioni di chiarezza dell'output
		printkdebug(F, "MOUNT source:%s target:%s filesystemtype:%s mountflags:%x data:%s",
								source,target,filesystemtype,mountflags, (data!=NULL)?data:"<NULL>");

		ERRNO_N_RETURN(0,0)
	}
}

static void vufuse_umount_internal(struct fuse_context *fc){

	struct fuse_context *fc_norace=fc;
	char *target=fc->fuse->path;

	fc_norace->pid = umvu_gettid();

	pthread_mutex_lock( &fc_norace->fuse->endmutex );
	//pthread_mutex_lock( &condition_mutex );

	if (fc_norace->fuse->fops.destroy != NULL )
			fc_norace->fuse->fops.destroy(fc_norace->private_data);

	fc_norace->fuse->inuse= EXITING;

	pthread_cond_signal(&fc_norace->fuse->endloop);
	pthread_mutex_unlock(&fc_norace->fuse->endmutex );
	//pthread_mutex_unlock( &condition_mutex );
	pthread_join(fc_norace->fuse->thread, NULL);

	free(fc_norace->fuse->filesystemtype);
	freeexceptions(fc_norace->fuse->exceptions);
	free(fc_norace->fuse->path);
	dlclose(fc_norace->fuse->dlhandle);
	free(fc_norace->fuse);
	free(fc_norace);

}
int vu_vufuse_umount2(const char *target, int flags) {
	struct fuse_context *fc=vu_get_ht_private_data();

	if (fc == NULL)
		ERRNO_N_RETURN(EINVAL,-1)
	else if (fc->fuse->inuse)
		ERRNO_N_RETURN(EBUSY,-1)
	else {
		/*cleanup and umount_internal will do the right umounting sequence in a lazy way*/
		vuht_del(vu_mod_getht(),flags);

		printkdebug(F,"UMOUNT target:%s flags:%d",target,flags);
		return 0;
	}

}

void vu_vufuse_cleanup(uint8_t type, void *arg, int arglen,struct vuht_entry_t *ht){
	if (type == CHECKPATH) {
		vu_mod_setht(ht);
		struct fuse_context *fc = vuht_get_private_data(ht);
		if (fc == NULL)
			ERRNO_N_RETURN(EINVAL,-1)
		else vufuse_umount_internal(fc);
		vu_mod_setht(NULL);

	}
	return 0;
}

/*******************************************************************************************/

void *vu_vufuse_init(void) {
	printk("vufuse init\n");
	return NULL;
}


int vu_vufuse_fini(void *private) {
	printk("vufuse fini\n");

	return 0;
}


/*******************************************************************************************/
/* fuse related functions*/

int fuse_version(void) { return VUFUSE_FUSE_VERSION;}

struct fuse_context *fuse_get_context(void)
{

	struct fuse_context *context=(struct fuse_context *)vu_get_ht_private_data();
	/* fs ids are more consistent than effective ids */
	/*um_mod_getfs_uid_gid(&(context->uid),&(context->gid)); /*needs to be discussed*/
	context->pid=umvu_gettid();
	return context;
}

int fuse_main_real(int argc, char *argv[], const struct fuse_operations *op,
		size_t op_size, void *user_data)
{
	struct fuse *f;
	struct fuse_chan *fuseargs = fuse_mount(NULL, NULL); /*options have been already parsed*/
	f = fuse_new(fuseargs, NULL, op, op_size, user_data);

	return fuse_loop(f);
}

/* fuse_mount and fuse_unmount are dummy functions,
 * the real mount operation has been done in umfuse_mount */
struct fuse_chan *fuse_mount(const char *mountpoint, struct fuse_args *args)
{
	return (struct fuse_chan *) fuse_get_context();
}


void fuse_unmount(const char *mountpoint, struct fuse_chan *ch)
{
	return;
}

/* set standard fuse_operations (vufusestd.c) for undefined fields in the
 * fuse_operations structure */
typedef long (*sysfun)();
static void fopsfill (struct fuse_operations *fops,size_t size)
{
	sysfun *f=(sysfun *)fops;
	sysfun *std=(sysfun *) &defaultservice;
	int i;
	int nfun=size/sizeof(sysfun);
	for (i=0; i<nfun; i++)
		if (f[i] == NULL) {
			f[i]=std[i];
		}
}

struct fuse *fuse_new(struct fuse_chan *ch, struct fuse_args *args,
		const struct fuse_operations *op, size_t op_size,
		void *user_data)
{
	struct fuse_context *fc=(struct fuse_context *)ch;
	if (op_size != sizeof(struct fuse_operations))
		printk("Fuse module vs vufuse support version mismatch");
	if (fc != fuse_get_context() || op_size != sizeof(struct fuse_operations)){
		fc->fuse->inuse=FUSE_ABORT;
		return NULL;
	}
	else {
		fc->fuse->fops = *op;
		fc->private_data = user_data;
		fopsfill(&fc->fuse->fops, op_size);
		return fc->fuse;
	}
}

void fuse_destroy(struct fuse *f)
{
	/*	**
	 * Destroy the FUSE handle.
	 *
	 * The filesystem is not unmounted.
	 *
	 * @param f the FUSE handle
	 */

}

int fuse_loop(struct fuse *f)
{
	if (f != NULL) {

		pthread_mutex_lock( &condition_mutex );
		pthread_cond_signal( &f->startloop );
		pthread_mutex_unlock( &condition_mutex );
		f->inuse = 0;
		pthread_mutex_lock( &f->endmutex );
		//pthread_mutex_lock( &condition_mutex );
		if (f->inuse != EXITING)
			pthread_cond_wait( &f->endloop, &f->endmutex );
		//pthread_cond_wait( &f->endloop, &condition_mutex );
		pthread_mutex_unlock( &f->endmutex );
		//pthread_mutex_unlock( &condition_mutex );
	}
	return 0;
}


void fuse_exit(struct fuse *f)
{
	/**
	 * Exit from event loop
	 *
	 * @param f the FUSE handle
	 */

}

int fuse_loop_mt(struct fuse *f)
{
	//in fuselib is FUSE event loop with multiple threads,
	//but here is all with multiple threads ;-)
	return fuse_loop(f);
}

int fuse_invalidate(struct fuse *f, const char *path)
{
	/**
	 * Invalidate cached data of a file.
	 *
	 * Useful if the 'kernel_cache' mount option is given, since in that
	 * case the cache is not invalidated on file open.
	 *
	 * @return 0 on success or -errno on failure
	 */

	return 0;
}

int fuse_is_lib_option(const char *opt)
{/**
	* Check whether a mount option should be passed to the kernel or the
	* library
	*
	* @param opt the option to check
	* @return 1 if it is a library option, 0 otherwise
	*/
	return 0;
}


/***********************************************************************************/

int vufuse_abort(struct fuse *f)
{
	f->inuse = FUSE_ABORT;
	pthread_mutex_lock( &condition_mutex );
	pthread_cond_signal( &f->startloop );
	pthread_mutex_unlock( &condition_mutex );

	return 0;
}

static char *mountflag2options(unsigned long mountflags, void *data)
{
	char opts[PATH_MAX];
	char *mountopts=(char *)data;
	opts[0]=0;

	if (mountflags & MS_REMOUNT)
		strncat(opts,"remount,",PATH_MAX);
	if (mountflags & MS_RDONLY)
		strncat(opts,"ro,",PATH_MAX);
	if (mountflags & MS_NOATIME)
		strncat(opts,"noatime,",PATH_MAX);
	if (mountflags & MS_NODEV)
		strncat(opts,"nodev,",PATH_MAX);
	if (mountflags & MS_NOEXEC)
		strncat(opts,"noexec,",PATH_MAX);
	if (mountflags & MS_NOSUID)
		strncat(opts,"nosuid,",PATH_MAX);
	if (mountflags & MS_SYNCHRONOUS)
		strncat(opts,"sync,",PATH_MAX);

	/* if there are options trailing comma is removed,
	 * otherwise "rw" becomes a comment */
	if (data && *mountopts)
		strncat(opts,mountopts,PATH_MAX);
	else if (*opts)
		opts[strlen(opts)-1]=0;
	else
		strncpy(opts,"rw",PATH_MAX);

	return(strdup(opts));
}
