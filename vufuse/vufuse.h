#ifndef VUFUSE_H
#define VUFUSE_H

#include <unistd.h>
#include <assert.h>
#include <fcntl.h>
#include <string.h>
#include <utime.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/ipc.h>
#include <stdarg.h>
#include <math.h>
#include <values.h>
#include <grp.h>
#include <pwd.h>
#include <errno.h>
#include <dirent.h>

#include <bits/wordsize.h>
#include <linux/types.h>
#include <linux/unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mount.h>
#include <sys/statfs.h>
#include <sys/statvfs.h>

#include <fuse.h>

#include "vufusestd.h"
#include "vufuse_node.h"
#include "vufuseargs.h"


#include <vumodule.h>
VU_PROTOTYPES(vufuse)

#ifndef VUFUSE_FUSE_VERSION
#define VUFUSE_FUSE_VERSION 29
#endif

#if ( FUSE_MINOR_VERSION <= 5 )
#error VUFUSE NEEDS FUSE >= 2.6
#endif

struct fuse {
	char *filesystemtype;
	char *path;
	char **exceptions;
	short pathlen;
	void *dlhandle;
	pthread_t thread;
	pthread_cond_t startloop;
	pthread_cond_t endloop;
	pthread_mutex_t endmutex;
	struct fuse_operations fops;
	int inuse;
	unsigned long flags;
};

struct fileinfo {
	struct fuse_context *context;
	//char *path;
	long long pos;				/* file offset */
	long long size;				/* file offset */
	struct fuse_file_info ffi;		/* includes open flags, file handle and page_write mode  */
	struct fuse_node *node;
	struct vudirent *dirinfo;		/* conversion fuse-getdir into kernel compliant dirent. Dir head pointer (list of vudirent) */
	struct vudirent *dirpos;		/* same conversion above: current pos entry (position in the list)*/
};

#define FILEPATH(f) ((f)->node->path)


#define ERRNO_N_RETURN(err,ret) \
{ \
	errno = err; \
	return ret; \
}

#endif
