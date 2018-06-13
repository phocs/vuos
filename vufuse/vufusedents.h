
#ifndef VUFUSDENTS_H
#define VUFUSDENTS_H

#include "vufuse.h"

/*linux_dirent64 struct*/
struct vufuse_dirent64 {
	__u64             d_ino;
	__s64             d_off;
	unsigned short    d_reclen;
	unsigned char     d_type;
	char             *d_name;
};

struct vudirent {
	struct vufuse_dirent64 de;
	//unsigned short d_reclen32;
	struct vudirent *next;
};

/* Handle for a getdir() operation */
struct fuse_dirhandle {
	struct vudirent *tail;
	long long offset;
};

int getdents64(unsigned int fd, struct dirent64 *dirp, unsigned int count);

struct vudirent *vufilldirinfo(struct fileinfo *fi);

void vucleandirinfo(struct vudirent *tail);


#endif