
#include "vufusedents.h"
#include "vufuse.h"

static inline unsigned long hashnodeid (const char *s);


int vu_vufuse_lstat(char *pathname, struct vu_stat *buf, int flags, int sfd, void *private) {

	struct fuse_context *fc=vu_get_ht_private_data();
	int rv;
	
	if (sfd >= 0 && private != NULL) {
		struct fileinfo *ft= (struct fileinfo*) private;
		pathname=FILEPATH(ft);
	} 		

	fc->pid=umvu_gettid(); 
	memset(buf, 0, sizeof(struct vu_stat));
	rv = fc->fuse->fops.getattr(pathname, buf);
	
	printkdebug(F,"LSTAT path:%s status: %s retvalue:%d", pathname, rv ? "ERROR" : "SUCCESS", (rv < 0) ? -rv : 0);
	
	if (rv<0) ERRNO_N_RETURN(-rv,-1)
	else {
		/*heuristics for file system which does not set st_ino */
		if (buf->st_ino == 0)
			buf->st_ino=(ino_t) hashnodeid(pathname);
		/*heuristics for file system which does not set st_dev */
		if (buf->st_dev == 0)
			buf->st_dev=(dev_t)((unsigned long) fc);
	}
	return rv;
}


int vu_vufuse_access(char *path, int mode, int flags) {

	struct fuse_context *fc=vu_get_ht_private_data();
	int rv=0;
	struct vu_stat buf;
	assert(fc);
	fc->pid=umvu_gettid(); 

	/* "default permission" management */
	if (fc->fuse->fops.access != NULL)
		rv= fc->fuse->fops.access(path, mode);
	else
	{
		rv = fc->fuse->fops.getattr( path, &buf);
	}
	
	printkdebug(F,"ACCESS path:%s mode:%s%s%s%s satus:%s retvalue:%d",path,
				(mode & R_OK) ? "R_OK": "", (mode & W_OK) ? "W_OK": "",
				(mode & X_OK) ? "X_OK": "", (mode & F_OK) ? "F_OK": "",
				rv ? "ERROR" : "SUCCESS", (rv < 0) ? -rv : 0);

	if (rv < 0) ERRNO_N_RETURN(-rv,-1)
	else ERRNO_N_RETURN(0,0)
	
}


ssize_t vu_vufuse_readlink(char *path, char *buf, size_t bufsiz) {
	struct fuse_context *fc=vu_get_ht_private_data();
	int rv;
	assert(fc != NULL);
	fc->pid=umvu_gettid();

	rv = fc->fuse->fops.readlink(path, buf, bufsiz);
	
	if (rv == 0)
		rv=strnlen(buf,bufsiz);

	printkdebug(F,"READLINK path:%s buf:%s status:%s retvalue:%d",path,buf,rv ? "ERROR" : "SUCCESS", (rv < 0) ? -rv : 0);
	if (rv < 0)
		 ERRNO_N_RETURN(-rv,-1)
	else
		return rv;
}

int vu_vufuse_open(const char *pathname, int flags, mode_t mode, void **private) {
	struct fuse_context *fc = vu_get_ht_private_data();

	struct fileinfo *ft = malloc(sizeof(struct fileinfo));
	int rv; /*it will be the sfd*/
	int exists_err;
	struct vu_stat buf;
	assert(fc!=NULL);
	fc->pid=umvu_gettid();


	ft->context = fc;
	ft->pos = 0;
	ft->ffi.flags = flags & ~(O_CREAT | O_EXCL | O_NOCTTY | O_TRUNC);
	ft->ffi.writepage = 0; 
	ft->node = NULL;
	ft->dirinfo = NULL;
	ft->dirpos = NULL;
	exists_err = fc->fuse->fops.getattr(pathname, &buf); /* if 0 the file already exists.*/
	ft->size = buf.st_size;

	*private = NULL;

	if ((flags & O_ACCMODE) != O_RDONLY && fc->fuse->flags & MS_RDONLY) ERRNO_N_RETURN(EROFS,-1)

	if ( (flags & (O_DIRECTORY)) && (!S_ISDIR(buf.st_mode))) ERRNO_N_RETURN(ENOTDIR,-1)

	if ((flags & O_ACCMODE) != O_RDONLY && (S_ISDIR(buf.st_mode))) ERRNO_N_RETURN(EISDIR,-1)


	if (exists_err == 0){ /* the file exists*/
		if ((flags & O_CREAT) && (flags & O_EXCL)) ERRNO_N_RETURN(EEXIST,-1)

		if((flags & O_TRUNC) && (flags & O_ACCMODE)!= O_RDONLY){
			rv=fc->fuse->fops.truncate(pathname, 0);
			
			printkdebug(F,"TRUNCATE path:%s flags:%x status:%s retvalue:%d",pathname,flags,rv ? "ERROR" : "SUCCESS", (rv < 0) ? -rv : 0);
			if (rv < 0) ERRNO_N_RETURN(-rv,-1)
		}
	}
	/* create the file: create or (obsolete mode) mknod+open */
	if ((flags & O_CREAT) && (exists_err != 0)){ 
		
		if (fc->fuse->fops.create != NULL) {
			rv = fc->fuse->fops.create(pathname, S_IFREG | mode, &ft->ffi);
			
			printkdebug(F,"CREATE path:%s flags:%x status:%s retvalue:%d",pathname,flags,rv ? "ERROR" : "SUCCESS", (rv < 0) ? -rv : 0);
		} else {
			rv = fc->fuse->fops.mknod(pathname, S_IFREG | mode, (dev_t) 0);
			
			printkdebug(F,"MKNOD path:%s flags:%x status:%s retvalue:%d",pathname,flags,rv ? "ERROR" : "SUCCESS", (rv < 0) ? -rv : 0);
			if (rv < 0)  ERRNO_N_RETURN(-rv,-1)
			rv = fc->fuse->fops.open(pathname, &ft->ffi);
		}

		if (rv >= 0) {
			if (fc->fuse->fops.fgetattr != NULL)
				rv = fc->fuse->fops.fgetattr(pathname, &buf, &ft->ffi);
			else
				rv = fc->fuse->fops.getattr(pathname, &buf);
		}

	} else { /* the file exists! */
		if ((flags & O_DIRECTORY) && fc->fuse->fops.readdir)
			rv = fc->fuse->fops.opendir(pathname, &ft->ffi);
		else 
			rv = fc->fuse->fops.open(pathname, &ft->ffi);
	}

	printkdebug(F,"OPEN path:%s flags:%x status:%s retvalue:%d",pathname,flags,rv ? "ERROR" : "SUCCESS", (rv < 0) ? -rv : 0);
	if (rv < 0)	 ERRNO_N_RETURN(-rv,-1)
	else {
		
		ft->node = node_add(fc->fuse, (char *)pathname);
		fc->fuse->inuse++;
		*private = ft;
		/*file related function will check sfd >= 0 before accessing fdprivate. That sfd will have the value of rv, so returning 0 is ok*/
		return rv; 
	}
}

int vu_vufuse_close(int fd, void *fdprivate){
	int rv;
	
	if(fd < 0 || fdprivate == NULL)
		ERRNO_N_RETURN(EBADF,-1)

	struct fileinfo *ft= (struct fileinfo *)fdprivate;

	struct fuse_context *fc=ft->context;
	fc->pid=umvu_gettid(); 

	if (!(ft->ffi.flags & O_DIRECTORY)) {
		rv=fc->fuse->fops.flush(FILEPATH(ft), &ft->ffi);
	}

	fc->fuse->inuse--;
	if ((ft->ffi.flags & O_DIRECTORY) && fc->fuse->fops.readdir)
		rv = fc->fuse->fops.releasedir(FILEPATH(ft), &ft->ffi);
	else
		rv=fc->fuse->fops.release(FILEPATH(ft), &ft->ffi);

	if (node_hiddenpathcheck(ft->node)) {
		rv = fc->fuse->fops.unlink(FILEPATH(ft));
	}

	printkdebug(F,"CLOSE path:%s status:%s retvalue:%d", FILEPATH(ft),rv ? "ERROR" : "SUCCESS", (rv < 0) ? -rv : 0);

	node_del(ft->node);
	vucleandirinfo(ft->dirinfo);
	
	free(fdprivate);
	fdprivate = NULL;

	if (rv<0) ERRNO_N_RETURN(-rv,-1)
	else {
		return rv;
	}
}


ssize_t vu_vufuse_read (int fd, void *buf, size_t count, void *fdprivate)
{
	int rv;

	if(fd < 0 || fdprivate == NULL)
		ERRNO_N_RETURN(EBADF,-1)

	struct fileinfo *ft = (struct fileinfo*)fdprivate; 
	if ( (ft->ffi.flags & O_ACCMODE) == O_WRONLY) 
		ERRNO_N_RETURN(EBADF,-1)
	else if (ft->pos == ft->size)
		return 0;
	else {
		struct fuse_context *fc=ft->context;
		fc->pid=umvu_gettid(); 
		rv = fc->fuse->fops.read(
				FILEPATH(ft),
				buf,
				count,
				ft->pos,
				&ft->ffi);

		printkdebug(F,"READ path:%s count:%u status:%s retvalue:%d",FILEPATH(ft), count,rv ? "ERROR" : "SUCCESS", (rv < 0) ? -rv : 0);
		if (rv<0) ERRNO_N_RETURN(-rv,-1)
		else {
			ft->pos += rv;
			return rv;
		}
	}
}

ssize_t vu_vufuse_write(int fd, const void *buf, size_t count, void *fdprivate)
{
	int rv=0;

	if(fd < 0 || fdprivate == NULL)
		ERRNO_N_RETURN(EBADF,-1)

	struct fileinfo *ft = (struct fileinfo *) fdprivate;
	if ( (ft->ffi.flags & O_ACCMODE) == O_RDONLY) {
		
		ERRNO_N_RETURN(EBADF,-1)
	} else {
		struct fuse_context *fc=ft->context;
		fc->pid=umvu_gettid(); 
		if (ft->ffi.flags & O_APPEND)
			rv=(int)vu_vufuse_lseek(fd,0,SEEK_END,fdprivate);
		if (rv!=-1) {
			rv = fc->fuse->fops.write(FILEPATH(ft),
					buf, count, ft->pos, &ft->ffi);
		}

		printkdebug(F,"WRITE path:%s count:%x status:%s retvalue:%d",FILEPATH(ft),count, rv ? "ERROR" : "SUCCESS", (rv < 0) ? -rv : 0);

		if (rv<0) ERRNO_N_RETURN(-rv,-1)
		else {
			ft->pos += rv;
			return rv;
		}
	}
}

int vu_vufuse_getdents64(unsigned int fd, struct dirent64 *dirp, unsigned int count, void *private) {

	printkdebug(F,"GETDENTS");

	if(fd < 0 || private == NULL)
		ERRNO_N_RETURN(EBADF,-1)

	struct fileinfo *ft= (struct fileinfo *)private;
	unsigned int curoffs=0;

	if (ft->dirinfo == NULL) 
		ft->dirinfo = vufilldirinfo(ft); 

	if (ft->dirinfo==NULL)
	 	return 0;
	else {
		struct dirent64 *current;
		char *base=(char *)dirp;
		int last=0;

		if (ft->dirpos==NULL) 
			ft->dirpos=ft->dirinfo;
		else
			last=(ft->dirpos==ft->dirinfo);

		while (!last && curoffs + ft->dirpos->next->de.d_reclen < count)
		{
			ft->dirpos=ft->dirpos->next;
			/*coping vufuse_dirent64 to dirp*/
			current=(struct dirent64 *)base;
			current->d_ino=ft->dirpos->de.d_ino;
			current->d_off=ft->dirpos->de.d_off;
			current->d_reclen=ft->dirpos->de.d_reclen;
			current->d_type=ft->dirpos->de.d_type;
			strcpy(current->d_name,ft->dirpos->de.d_name);
			/* workaround: some FS do not set d_ino, but
			 * inode 0 is special and is skipped by libc */
			if (current->d_ino == 0)
				current->d_ino = 2;
			base+=ft->dirpos->de.d_reclen;
			curoffs+=ft->dirpos->de.d_reclen;
			last=(ft->dirpos == ft->dirinfo);
		}
	}
	return curoffs;
}

off_t vu_vufuse_lseek(int fd, off_t offset, int whence, void *fdprivate)
{
	printkdebug(F,"LSEEK");

	if(fd < 0 || fdprivate == NULL)
		ERRNO_N_RETURN(EBADF,-1)

	struct fileinfo *ft =(struct fileinfo *)fdprivate;
	switch (whence) {
		case SEEK_SET:
			ft->pos=offset;
			break;
		case SEEK_CUR:
			ft->pos += offset;
			break;
		case SEEK_END:
			{
				struct stat buf;
				int rv;
				struct fuse_context *fc=ft->context;
				assert(fc != NULL);

				fc->pid=umvu_gettid(); 
				rv = fc->fuse->fops.getattr(FILEPATH(ft),&buf);
				if (rv>=0) {
					ft->pos = buf.st_size + offset;
				} else ERRNO_N_RETURN(EBADF,-1)																																																	      }
			break;
	}

	return ft->pos;
}

#define FUSE_SUPER_MAGIC 0x65735546
int vu_vufuse_statfs (const char *pathname, struct statfs *buf, int fd, void *fdprivate){
	printkdebug(F,"STATFS");
	long rv;
	struct fuse_context *fc = vu_get_ht_private_data();
	struct statvfs svfs;
	memset (&svfs, 0, sizeof(struct statvfs));
	fc->pid=umvu_gettid();
	if (fc->fuse->fops.statfs) {
		rv=fc->fuse->fops.statfs(pathname, &svfs);
		if (rv >= 0) {
			buf->f_type = FUSE_SUPER_MAGIC; //
			buf->f_bsize = svfs.f_bsize;
			buf->f_blocks = svfs.f_blocks;
			buf->f_bfree = svfs.f_bfree;
			buf->f_bavail = svfs.f_bavail;
			buf->f_files = svfs.f_files;
			buf->f_ffree = svfs.f_ffree;
			buf->f_namelen =svfs.f_namemax;
			buf->f_frsize =svfs.f_frsize;
			/* fsid is left zero */
			return rv;
		} else 
			ERRNO_N_RETURN(-rv,-1)
	}
	else 
		ERRNO_N_RETURN(ENOSYS,-1)
}

int vu_vufuse_unlink (const char *pathname){
	struct fuse_context *fc=vu_get_ht_private_data();
	int rv=0;
	int exists_err;
	struct fuse_node *node;
	struct vu_stat buf;

	assert(fc != NULL);
	fc->pid=umvu_gettid();

	if (fc->fuse->flags & MS_RDONLY) 
		ERRNO_N_RETURN(EROFS,-1)

	if ((exists_err = fc->fuse->fops.getattr(pathname, &buf)) < 0) 
		ERRNO_N_RETURN(ENOENT,-1)
	
	/* hardremove or the file is not open -> unlink */
	if (fc->fuse->flags & FUSE_HARDREMOVE || fc->fuse->fops.rename == NULL ||
			(node = node_search(fc->fuse, (char *)pathname)) == NULL) {		
		rv = fc->fuse->fops.unlink(pathname);
		
		printkdebug(F,"UNLINK path:%s status:%s retvalue:%d",pathname,rv ? "ERROR" : "SUCCESS", (rv < 0) ? -rv : 0);
	} else {
		/* rename the file ! */
		char *hiddenpath=node_hiddenpath(node);
		rv = fc->fuse->fops.rename(pathname,hiddenpath);
		
		printkdebug(F,"RENAME(UNLINK) path:%s hiddenpath:%s status:%s retvalue:%d",pathname,hiddenpath,rv ? "ERROR" : "SUCCESS", (rv < 0) ? -rv : 0);
		if (rv == 0) 
			node_newpath(node,hiddenpath);
		free(hiddenpath);
	}
	if (rv < 0) 
		ERRNO_N_RETURN(-rv,-1)
	else
		return rv;
}

int vu_vufuse_truncate(const char *path, off_t length, int fd, void *fdprivate){
	struct fuse_context *fc=vu_get_ht_private_data();
	int rv;

	/* FUSE defines length as off_t, so we will not truncate a file
	 * if the desired length can't be contained in an off_t.
	 * Please ask FUSE developers to define length as loff_t. */
	if (length != (loff_t)((off_t)length))
		ERRNO_N_RETURN(EINVAL,-1)

	assert(fc != NULL);
	fc->pid=umvu_gettid();
	if (fc->fuse->flags & MS_RDONLY) 
		ERRNO_N_RETURN(EROFS,-1)

	rv = fc->fuse->fops.truncate(path,(off_t)length);

	printkdebug(F,"TRUNCATE path:%s status:%s retvalue:%d",path,rv ? "ERROR" : "SUCCESS", (rv < 0) ? -rv : 0);
	if (rv < 0) 
		ERRNO_N_RETURN(-rv,-1)
	else
		return rv;	

}

int vu_vufuse_mkdir (const char *pathname, mode_t mode){
	struct fuse_context *fc=vu_get_ht_private_data();
	int rv=0;
	assert(fc != NULL);
	fc->pid=umvu_gettid();

	if (fc->fuse->flags & MS_RDONLY)
		ERRNO_N_RETURN(EROFS,-1)

	rv = fc->fuse->fops.mkdir( pathname, mode);

	printkdebug(F,"MKDIR path:%s status:%s retvalue:%d",pathname,rv ? "ERROR" : "SUCCESS", (rv < 0) ? -rv : 0);
	if (rv < 0) 
		ERRNO_N_RETURN(-rv,-1)
	else
		return rv;
}

int vu_vufuse_mknod (const char *pathname, mode_t mode, dev_t dev)
{
	struct fuse_context *fc = vu_get_ht_private_data();
	int rv;
	assert(fc != NULL);

	if (fc->fuse->flags & MS_RDONLY)
		 ERRNO_N_RETURN(EROFS,-1)
		
	if (S_ISREG(mode)) {
		struct fuse_file_info fi;
		memset(&fi, 0, sizeof(fi));
		fi.flags = O_CREAT | O_EXCL | O_WRONLY;
		rv = fc->fuse->fops.create(pathname, mode, &fi);
		if (rv >= 0) {
			fc->fuse->fops.release(pathname, &fi);
		}
	} else {
		rv = fc->fuse->fops.mknod(pathname, mode, dev);
	}

	printkdebug(F,"MKNOD path:%s major:%d minor:%d  status:%s retvalue:%d",pathname,major(dev),minor(dev),rv ? "ERROR" : "SUCCESS", (rv < 0) ? -rv : 0);
	if (rv < 0) ERRNO_N_RETURN(-rv,-1)
	else return rv;
}

int vu_vufuse_rmdir(const char *pathname){
	struct fuse_context *fc= vu_get_ht_private_data();
	int rv;
	assert(fc!=NULL);
	fc->pid=umvu_gettid();

	if (fc->fuse->flags & MS_RDONLY) 
		ERRNO_N_RETURN(EROFS,-1)
	
	rv= fc->fuse->fops.rmdir( pathname);
	
	printkdebug(F,"RMDIR path:%s status:%s retvalue:%d",pathname,rv ? "ERROR" : "SUCCESS", (rv < 0) ? -rv : 0);
	if (rv < 0) 
		ERRNO_N_RETURN(-rv,-1)
	else
		return rv;
}


int vu_vufuse_chmod (const char *pathname, mode_t mode, int fd, void *fdprivate){

	struct fuse_context *fc =vu_get_ht_private_data();
	int rv;
	char *unpath;
	assert(fc != NULL);
	fc->pid=umvu_gettid();
	
	if (fd >= 0 && fdprivate != NULL) {
		struct fileinfo *ft= (struct fileinfo*) fdprivate;
		pathname=FILEPATH(ft); 
	} 


	if (fc->fuse->flags & MS_RDONLY) 
		ERRNO_N_RETURN(EROFS,-1)

	rv= fc->fuse->fops.chmod(pathname, mode);

	printkdebug(F,"CHMOD path:%s status:%s retvalue:%d",pathname,rv ? "ERROR" : "SUCCESS", (rv < 0) ? -rv : 0);
	if (rv < 0) 
		ERRNO_N_RETURN(-rv,-1)
	return rv;
}

int vu_vufuse_lchown (const char *pathname, uid_t owner, gid_t group,int fd, void *fdprivate){
	
	struct fuse_context *fc = vu_get_ht_private_data();
	int rv=0;
	assert(fc != NULL);
	fc->pid=umvu_gettid();
	
	if (fd >= 0 && fdprivate != NULL) {
		struct fileinfo *ft= (struct fileinfo*) fdprivate;
		pathname=FILEPATH(ft);
	} 

	if (fc->fuse->flags & MS_RDONLY) 
		ERRNO_N_RETURN(EROFS,-1)

	rv = fc->fuse->fops.chown(pathname, owner, group);

	printkdebug(F,"LCHOWN  status:%s retvalue:%d",rv ? "ERROR" : "SUCCESS", (rv < 0) ? -rv : 0);
	if (rv < 0) 
		ERRNO_N_RETURN(-rv,-1)
	else
		return rv;
}

int vu_vufuse_utimensat(int dirfd, const char *pathname, 
		const struct timespec times[2], int flags, int fd, void *fdprivate){
	struct fuse_context *fc = vu_get_ht_private_data();
	int rv;
	assert(fc != NULL);
	fc->pid=umvu_gettid();

	if (fc->fuse->flags & MS_RDONLY) 
		ERRNO_N_RETURN(EROFS,-1)
	

	if (fc->fuse->fops.utimens) {
		struct timespec tvspec[2];

		if (times == NULL) {
			struct timeval nowtv;
			gettimeofday(&nowtv,NULL);
			tvspec[0].tv_sec = nowtv.tv_sec;
			tvspec[0].tv_nsec = nowtv.tv_usec * 1000;
			tvspec[1]=tvspec[0];

			rv = fc->fuse->fops.utimens( pathname, tvspec);
		} else {
			rv = fc->fuse->fops.utimens( pathname, times);
		}
		
		printkdebug(F,"UTIMESAT path:%s status:%s retvalue:%d",pathname,rv ? "ERROR" : "SUCCESS", (rv < 0) ? -rv : 0);
	} else {
		struct utimbuf buf;
		if (times == NULL) 
			buf.actime=buf.modtime=time(NULL);
		else {
			buf.actime=times[0].tv_sec;
			buf.modtime=times[1].tv_sec;
		}
		rv = fc->fuse->fops.utime( pathname , &buf);
		
		printkdebug(F,"UTIME path:%s status:%s retvalue:%d",pathname,rv ? "ERROR" : "SUCCESS", (rv < 0) ? -rv : 0);
	}
	
	if (rv < 0) 
		ERRNO_N_RETURN(-rv,-1)
	else
		return rv;
}


int vu_vufuse_symlink (const char *target, const char *linkpath){
	struct fuse_context *fc=vu_get_ht_private_data();
	int rv=0;
	assert(fc != NULL);
	fc->pid=umvu_gettid();
	if (fc->fuse->flags & MS_RDONLY) 
		ERRNO_N_RETURN(EROFS,-1)

	rv = fc->fuse->fops.symlink( target, linkpath);
	
	printkdebug(F,"SYMLINK target:%s linkpath:%s status:%s retvalue:%d",target,linkpath,rv ? "ERROR" : "SUCCESS", (rv < 0) ? -rv : 0);	
	if (rv < 0) 
		ERRNO_N_RETURN(-rv,-1)
	else
		return rv;
}


int vu_vufuse_link (const char *target, const char *linkpath){
	struct fuse_context *fc= vu_get_ht_private_data();
	int rv=0;
	assert(fc != NULL);
	fc->pid=umvu_gettid();

	if (fc->fuse->flags & MS_RDONLY) 
		ERRNO_N_RETURN(EROFS,-1)

	rv = fc->fuse->fops.link(target, linkpath);
		
	printkdebug(F,"LINK oldpath:%s newpath:%s status:%s retvalue:%d",target,linkpath,rv ? "ERROR" : "SUCCESS", (rv < 0) ? -rv : 0);		
	if (rv < 0) 
		ERRNO_N_RETURN(-rv,-1)
	else
		return rv;	
}

int vu_vufuse_rename (const char *target, const char *linkpath, int flags){

	struct fuse_context *fc=vu_get_ht_private_data();
	int rv=0;
	assert(fc != NULL);
	fc->pid=umvu_gettid();
	
	rv = fc->fuse->fops.rename(target,linkpath);
	
	printkdebug(F,"RENAME oldpath:%s newpath:%s status:%s retvalue:%d",target,linkpath,rv ? "ERROR" : "SUCCESS", (rv < 0) ? -rv : 0);
	if (rv < 0) 
		ERRNO_N_RETURN(-rv,-1)
	else {
		struct fuse_node *oldnode=node_search(fc->fuse,(char *)target);
		struct fuse_node *newnode=node_search(fc->fuse,(char *)linkpath);
		if (newnode != NULL) {
			char *hiddenpath=node_hiddenpath(newnode);
			
			rv = fc->fuse->fops.rename(linkpath,hiddenpath);
			printkdebug(F,"RENAME oldpath:%s newpath:%s status:%s retvalue:%d",linkpath,hiddenpath,rv ? "ERROR" : "SUCCESS", (rv < 0) ? -rv : 0);
			if (rv == 0)
				node_newpath(newnode,hiddenpath);
			free(hiddenpath);
		}
		if (oldnode != NULL)
			node_newpath(oldnode,(char *)linkpath);
		return rv;	
	}
}


/*heuristics for file system which does not set st_ino */
static inline unsigned long hashnodeid (const char *s) {
	unsigned long sum=0;
	while (*s) {
		sum=sum ^ ((sum << 5) + (sum >> 2) + *s);
		s++;
	}
	return sum;
}