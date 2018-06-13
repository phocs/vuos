#include "vufusedents.h"

#define WORDLEN sizeof(int *)
#define WORDALIGN(X) (((X) + WORDLEN) & ~(WORDLEN-1))
#define SIZEDIRENT64NONAME (sizeof(__u64)+sizeof(__s64)+sizeof(unsigned short)+sizeof(unsigned char))
//#define SIZEDIRENT32NONAME (sizeof(long)+sizeof(__kernel_off_t)+sizeof(unsigned short))

int getdents64(unsigned int fd, struct dirent64 *dirp, unsigned int count)
{
	return syscall(__NR_getdents64, fd, dirp, count);
}

#define adjust_list(h,new) \
{ \
	if (h->tail==NULL) { \
			new->next=new; \
		} else { \
			new->next=h->tail->next; \
			h->tail->next=new; \
		} \
		h->tail=new; \
}


static int vufusefilldir(fuse_dirh_t h, const char *name, int type, ino_t ino)
{
	if (name != NULL) {
		struct vudirent *new=(struct vudirent *)malloc(sizeof(struct vudirent));
		new->de.d_ino=ino;
		new->de.d_type=type;
		new->de.d_name=strdup(name);
		new->de.d_reclen=WORDALIGN(SIZEDIRENT64NONAME+strlen(name)+1);
		/* virtualize the offset on a real file, 64bit ino+16len+8namlen+8type */
		new->de.d_off=h->offset=h->offset+WORDALIGN(12+strlen(name));
		
		adjust_list(h,new)
	}
	return 0;
}

static int vufusefillreaddir(void *buf, const char *name, const struct stat *stbuf, off_t off)
{
	fuse_dirh_t h=buf;
	if (name != NULL) {
		struct vudirent *new=(struct vudirent *)malloc(sizeof(struct vudirent));
		if (stbuf == NULL) {
			new->de.d_ino=-1;
			new->de.d_type=0;
		} else {
			new->de.d_ino=stbuf->st_ino;
			new->de.d_type=stbuf->st_mode >> 12;
		}
		new->de.d_name=strdup(name);
		new->de.d_reclen=WORDALIGN(SIZEDIRENT64NONAME+strlen(name)+1);
		/* virtualize the offset on a real file, 64bit ino+16len+8namlen+8type */
		new->de.d_off=h->offset=h->offset+WORDALIGN(12+strlen(name));
		
		adjust_list(h,new)
	}
	return 0;
}

static int merge_newentry(char *name,struct vudirent *tail,struct vudirent *oldtail)
{
	if (oldtail) {
		do {
			struct vudirent *next=tail->next;
			if (strcmp(next->de.d_name,name)==0)
				return 0;
			tail=next;
		} while (tail != oldtail);
	}
	return 1;
}


static void vu_mergedir(char *path,struct fuse_context *fc,fuse_dirh_t h)
{
	char *abspath;
	int fd;
	asprintf(&abspath,"%s%s",fc->fuse->path,path);
	fd=open(abspath,O_RDONLY|O_DIRECTORY);
	free(abspath);
	if (fd) {
		char buf[4096];
		int len;
		struct vudirent *oldtail=h->tail;
		while ((len=getdents64(fd,(struct dirent64 *)buf,4096)) > 0) {
			off_t off=0;
			while (off<len) {
				struct dirent64 *de=(struct dirent64 *)(buf+off);
				if (merge_newentry(de->d_name,h->tail,oldtail))
				{
					struct vudirent *new=(struct vudirent *)malloc(sizeof(struct vudirent));
					new->de.d_name=strdup(de->d_name);
					new->de.d_type=de->d_type;
					new->de.d_ino=de->d_ino;
					new->de.d_reclen=WORDALIGN(SIZEDIRENT64NONAME+strlen(de->d_name)+1);
					new->de.d_off=h->offset=h->offset+WORDALIGN(12+strlen(de->d_name));
					
					adjust_list(h,new)
				}
				off+=de->d_reclen;
			}
		}
		close(fd);
	}
}


struct vudirent *vufilldirinfo(struct fileinfo *fi)
{
	int rv;
	struct fuse_dirhandle dh;
	struct fuse_context *fc=fi->context;
	dh.tail=NULL;
	dh.offset=0;
	if (fc->fuse->fops.readdir)
		rv=fc->fuse->fops.readdir(FILEPATH(fi),&dh, vufusefillreaddir, 0, &fi->ffi);
	else
		rv=fc->fuse->fops.getdir(FILEPATH(fi), &dh, vufusefilldir);
	if (fc->fuse->flags & FUSE_MERGE && rv>=0) 
		vu_mergedir(FILEPATH(fi),fc,&dh);
	
	if (rv < 0)
		return NULL;
	else 
		return dh.tail;
}


void vucleandirinfo(struct vudirent *tail)
{
	if (tail != NULL) {
		while (tail->next != tail) {
			struct vudirent *tmp;
			tmp=tail->next;
			tail->next=tmp->next;
			free(tmp->de.d_name);
			free(tmp);
		}
		free(tail);
	}
}