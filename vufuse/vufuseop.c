/*
 *   VUOS: view OS project
 *   Copyright (C) 2018  Renzo Davoli <renzo@cs.unibo.it>
 *                       Leonardo Frioli <leonardo.frioli@studio.unibo.it>
 *   VirtualSquare team.
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>
#include <stddef.h>
#include <sys/sysmacros.h>
#include <fuse.h>
#include <volatilestream.h>
#include <vumodule.h>
#include <vufuse.h>

#define FILEPATH(x) x->path

/*heuristics for file system which does not set st_ino */
static inline unsigned long hashnodeid (const char *s) {
	unsigned long sum = 0;
	while (*s) {
		sum = sum ^ ((sum << 5) + (sum >> 2) + *s);
		s++;
	}
	return sum;
}

int vu_vufuse_lstat(char *pathname, struct vu_stat *buf, int flags, int sfd, void *private) {
	int rv;
	struct fuse_context fc, *ofc;
	ofc = fuse_push_context(&fc);

	memset(buf, 0, sizeof(struct vu_stat));
	rv = fc.fuse->fops.getattr(pathname, buf);
	fuse_pop_context(ofc);

	printkdebug(F,"LSTAT path:%s status: %s retvalue:%d", pathname, rv ? "ERROR" : "SUCCESS", (rv < 0) ? -rv : 0);

	if (rv<0) {
		errno = -rv;
		return -1;
	} else {
		/*heuristics for file system which does not set st_ino */
		if (buf->st_ino == 0)
			buf->st_ino = (ino_t) hashnodeid(pathname);
		/*heuristics for file system which does not set st_dev */
		if (buf->st_dev == 0)
			buf->st_dev = (dev_t)((unsigned long) &fc);
	}
	return rv;
}

int vu_vufuse_access(char *path, int mode, int flags) {
	int rv = 0;
	struct fuse_context fc, *ofc;
	ofc = fuse_push_context(&fc);

	/* "default permission" management */
	if (fc.fuse->fops.access != NULL)
		rv = fc.fuse->fops.access(path, mode);
	else {
		struct vu_stat buf;
		rv = fc.fuse->fops.getattr(path, &buf);
	}
	fuse_pop_context(ofc);

	printkdebug(F,"ACCESS path:%s mode:%s%s%s%s satus:%s retvalue:%d",path,
			(mode & R_OK) ? "R_OK": "", (mode & W_OK) ? "W_OK": "",
			(mode & X_OK) ? "X_OK": "", (mode & F_OK) ? "F_OK": "",
			rv ? "ERROR" : "SUCCESS", (rv < 0) ? -rv : 0);

	if (rv < 0) {
		errno = -rv;
		return -1;
	} else
		return rv;
}

ssize_t vu_vufuse_readlink(char *path, char *buf, size_t bufsiz) {
	int rv;
	struct fuse_context fc, *ofc;
	ofc = fuse_push_context(&fc);

	rv = fc.fuse->fops.readlink(path, buf, bufsiz);

	if (rv == 0)
		rv = strnlen(buf,bufsiz);
	fuse_pop_context(ofc);

	printkdebug(F,"READLINK path:%s buf:%s status:%s retvalue:%d",path,buf,rv ? "ERROR" : "SUCCESS", (rv < 0) ? -rv : 0);

	if (rv < 0) {
		errno = -rv;
		return -1;
	} else
		return rv;
}

#define FUSE_SUPER_MAGIC 0x65735546
int vu_vufuse_statfs (const char *pathname, struct statfs *buf, int fd, void *fdprivate){
	int rv;
	struct statvfs svfs;
	struct fuse_context fc, *ofc;
	ofc = fuse_push_context(&fc);
	memset (&svfs, 0, sizeof(struct statvfs));
	printkdebug(F,"STATFS", NULL);
	if (fc.fuse->fops.statfs) {
		rv = fc.fuse->fops.statfs(pathname, &svfs);
		fuse_pop_context(ofc);
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
		} else {
			errno = -rv;
			return -1;
		}
	}	else {
		fuse_pop_context(ofc);
		errno = ENOSYS;
		return -1;
	}
}

int vu_vufuse_mkdir (const char *pathname, mode_t mode){
	int rv = 0;
	struct fuse_context fc, *ofc;
	ofc = fuse_push_context(&fc);

	if (fc.fuse->flags & MS_RDONLY) {
		fuse_pop_context(ofc);
		errno = EROFS;
		return -1;
	}

	rv = fc.fuse->fops.mkdir(pathname, mode);
	fuse_pop_context(ofc);

	printkdebug(F,"MKDIR path:%s status:%s retvalue:%d",pathname,rv ? "ERROR" : "SUCCESS", (rv < 0) ? -rv : 0);
	if (rv < 0) {
		errno = -rv;
		return -1;
	} else
		return rv;
}

int vu_vufuse_mknod (const char *pathname, mode_t mode, dev_t dev)
{
	int rv;
	struct fuse_context fc, *ofc;
	ofc = fuse_push_context(&fc);

	if (fc.fuse->flags & MS_RDONLY) {
		fuse_pop_context(ofc);
		errno = EROFS;
		return -1;
	}

	if (S_ISREG(mode)) {
		struct fuse_file_info fi;
		memset(&fi, 0, sizeof(fi));
		fi.flags = O_CREAT | O_EXCL | O_WRONLY;
		rv = fc.fuse->fops.create(pathname, mode, &fi);
		if (rv >= 0) {
			fc.fuse->fops.release(pathname, &fi);
		}
	} else {
		rv = fc.fuse->fops.mknod(pathname, mode, dev);
	}
	fuse_pop_context(ofc);

	printkdebug(F,"MKNOD path:%s major:%d minor:%d  status:%s retvalue:%d",pathname,major(dev),minor(dev),rv ? "ERROR" : "SUCCESS", (rv < 0) ? -rv : 0);
	if (rv < 0) {
		errno = -rv;
		return -1;
	} else
		return rv;
}

int vu_vufuse_rmdir(const char *pathname){
	int rv;
	struct fuse_context fc, *ofc;
	ofc = fuse_push_context(&fc);

	if (fc.fuse->flags & MS_RDONLY) {
		fuse_pop_context(ofc);
		errno = EROFS;
		return -1;
	}

	rv = fc.fuse->fops.rmdir( pathname);
	fuse_pop_context(ofc);

	printkdebug(F,"RMDIR path:%s status:%s retvalue:%d",pathname,rv ? "ERROR" : "SUCCESS", (rv < 0) ? -rv : 0);
	if (rv < 0) {
		errno = -rv;
		return -1;
	} else
		return rv;
}


int vu_vufuse_chmod (const char *pathname, mode_t mode, int fd, void *fdprivate){
	int rv;
	struct fuse_context fc, *ofc;
	ofc = fuse_push_context(&fc);

	if (fc.fuse->flags & MS_RDONLY) {
		fuse_pop_context(ofc);
		errno = EROFS;
		return -1;
	}

	rv = fc.fuse->fops.chmod(pathname, mode);
	fuse_pop_context(ofc);

	printkdebug(F,"CHMOD path:%s status:%s retvalue:%d",pathname,rv ? "ERROR" : "SUCCESS", (rv < 0) ? -rv : 0);
	if (rv < 0) {
		errno = -rv;
		return -1;
	} else
		return rv;
}

int vu_vufuse_lchown (const char *pathname, uid_t owner, gid_t group,int fd, void *fdprivate){
	int rv;
	struct fuse_context fc, *ofc;
	ofc = fuse_push_context(&fc);

	if (fc.fuse->flags & MS_RDONLY) {
		fuse_pop_context(ofc);
		errno = EROFS;
		return -1;
	}

	rv = fc.fuse->fops.chown(pathname, owner, group);
	fuse_pop_context(ofc);

	printkdebug(F,"LCHOWN  status:%s retvalue:%d",rv ? "ERROR" : "SUCCESS", (rv < 0) ? -rv : 0);
	if (rv < 0) {
		errno = -rv;
		return -1;
	} else
		return rv;
}

int vu_vufuse_symlink (const char *target, const char *linkpath){
	int rv;
	struct fuse_context fc, *ofc;
	ofc = fuse_push_context(&fc);

	if (fc.fuse->flags & MS_RDONLY) {
		fuse_pop_context(ofc);
		errno = EROFS;
		return -1;
	}

	rv = fc.fuse->fops.symlink( target, linkpath);
	fuse_pop_context(ofc);

	printkdebug(F,"SYMLINK target:%s linkpath:%s status:%s retvalue:%d",target,linkpath,rv ? "ERROR" : "SUCCESS", (rv < 0) ? -rv : 0);
	if (rv < 0) {
		errno = -rv;
		return -1;
	} else
		return rv;
}

int vu_vufuse_truncate(const char *path, off_t length, int fd, void *fdprivate){
	int rv;
	struct fuse_context fc, *ofc;
	ofc = fuse_push_context(&fc);

	if (fc.fuse->flags & MS_RDONLY) {
		fuse_pop_context(ofc);
		errno = EROFS;
		return -1;
	}

	rv = fc.fuse->fops.truncate(path,(off_t)length);
	fuse_pop_context(ofc);

	printkdebug(F,"TRUNCATE path:%s status:%s retvalue:%d",path,rv ? "ERROR" : "SUCCESS", (rv < 0) ? -rv : 0);

	if (rv < 0) {
		errno = -rv;
		return -1;
	} else
		return rv;
}

int vu_vufuse_link (const char *target, const char *linkpath){
	int rv;
	struct fuse_context fc, *ofc;
	ofc = fuse_push_context(&fc);

	if (fc.fuse->flags & MS_RDONLY) {
		fuse_pop_context(ofc);
		errno = EROFS;
		return -1;
	}

	rv = fc.fuse->fops.link(target, linkpath);
	fuse_pop_context(ofc);

	printkdebug(F,"LINK oldpath:%s newpath:%s status:%s retvalue:%d",target,linkpath,rv ? "ERROR" : "SUCCESS", (rv < 0) ? -rv : 0);
	if (rv < 0) {
		errno = -rv;
		return -1;
	} else
		return rv;
}

int vu_vufuse_open(const char *pathname, int flags, mode_t mode, void **private) {
	int rv;
	int exists_err;
	struct fileinfo *ft;
	struct vu_stat buf;

	struct fuse_context fc, *ofc;
	ofc = fuse_push_context(&fc);

	exists_err = fc.fuse->fops.getattr(pathname, &buf); /* if 0 the file already exists.*/

	if ((flags & O_ACCMODE) != O_RDONLY && fc.fuse->flags & MS_RDONLY) {
		fuse_pop_context(ofc);
		errno = EROFS;
		return -1;
	}

	if ( (flags & (O_DIRECTORY)) && (!S_ISDIR(buf.st_mode))) {
		fuse_pop_context(ofc);
		errno = ENOTDIR;
		return -1;
	}

	if ((flags & O_ACCMODE) != O_RDONLY && (S_ISDIR(buf.st_mode))) {
		fuse_pop_context(ofc);
		errno = EISDIR;
		return -1;
	}

	if (exists_err == 0){ /* the file exists*/
		if ((flags & O_CREAT) && (flags & O_EXCL)) {
			errno = EEXIST;
			return -1;
		}

		if ((flags & O_TRUNC) && (flags & O_ACCMODE)!= O_RDONLY){
			rv = fc.fuse->fops.truncate(pathname, 0);

			printkdebug(F,"TRUNCATE path:%s flags:%x status:%s retvalue:%d",pathname,flags,rv ? "ERROR" : "SUCCESS", (rv < 0) ? -rv : 0);
			if (rv < 0) {
				fuse_pop_context(ofc);
				errno = -rv;
				return -1;
			}
		}
	}

	ft = malloc(sizeof(struct fileinfo));
	ft->pos = 0;
	ft->ffi.flags = flags & ~(O_CREAT | O_EXCL | O_NOCTTY | O_TRUNC);
	ft->ffi.writepage = 0;
	ft->path = NULL;
	//ft->node = NULL;
	ft->dirf = NULL;
	ft->size = buf.st_size;

	*private = NULL;

	/* create the file: create or (obsolete mode) mknod+open */
	if ((flags & O_CREAT) && (exists_err != 0)){

		if (fc.fuse->fops.create != NULL) {
			rv = fc.fuse->fops.create(pathname, S_IFREG | mode, &ft->ffi);

			printkdebug(F,"CREATE path:%s flags:%x status:%s retvalue:%d",pathname,flags,rv ? "ERROR" : "SUCCESS", (rv < 0) ? -rv : 0);
		} else {
			rv = fc.fuse->fops.mknod(pathname, S_IFREG | mode, (dev_t) 0);

			printkdebug(F,"MKNOD path:%s flags:%x status:%s retvalue:%d",pathname,flags,rv ? "ERROR" : "SUCCESS", (rv < 0) ? -rv : 0);
			if (rv < 0) {
				fuse_pop_context(ofc);
				free(ft);
				errno = -rv;
				return -1;
			}
			rv = fc.fuse->fops.open(pathname, &ft->ffi);
		}

#if 0
		/* this should be useless */
		if (rv >= 0) {
			if (fc.fuse->fops.fgetattr != NULL)
				rv = fc.fuse->fops.fgetattr(pathname, &buf, &ft->ffi);
			else
				rv = fc.fuse->fops.getattr(pathname, &buf);
		}
#endif

	} else { /* the file exists! */
		if ((flags & O_DIRECTORY) && fc.fuse->fops.readdir)
			rv = fc.fuse->fops.opendir(pathname, &ft->ffi);
		else
			rv = fc.fuse->fops.open(pathname, &ft->ffi);
	}
	fuse_pop_context(ofc);

	printkdebug(F,"OPEN path:%s flags:%x status:%s retvalue:%d",pathname,flags,rv ? "ERROR" : "SUCCESS", (rv < 0) ? -rv : 0);
	if (rv < 0) {
		free(ft);
		errno = -rv;
		return -1;
	} else {
		//ft->node = node_add(fc.fuse, (char *)pathname);
		ft->path = strdup(pathname);
		fc.fuse->inuse++;
		*private = ft;
		/*file related function will check sfd >= 0 before accessing fdprivate. That sfd will have the value of rv, so returning 0 is ok*/
		return rv;
	}
}

int vu_vufuse_close(int fd, void *fdprivate){
	if (fd < 0 || fdprivate == NULL) {
		errno = EBADF;
		return -1;
	} else {
		int rv;
		struct fileinfo *ft = (struct fileinfo *)fdprivate;
		struct fuse_context fc, *ofc;
		ofc = fuse_push_context(&fc);

		if (!(ft->ffi.flags & O_DIRECTORY)) {
			rv = fc.fuse->fops.flush(FILEPATH(ft), &ft->ffi);
		}

		fc.fuse->inuse--;
		if ((ft->ffi.flags & O_DIRECTORY) && fc.fuse->fops.readdir)
			rv = fc.fuse->fops.releasedir(FILEPATH(ft), &ft->ffi);
		else
			rv = fc.fuse->fops.release(FILEPATH(ft), &ft->ffi);

#if 0
		if (node_hiddenpathcheck(ft->node)) {
			rv = fc.fuse->fops.unlink(FILEPATH(ft));
		}
#endif

		fuse_pop_context(ofc);
		printkdebug(F,"CLOSE path:%s status:%s retvalue:%d", FILEPATH(ft),rv ? "ERROR" : "SUCCESS", (rv < 0) ? -rv : 0);

		if (rv<0) {
			errno = -rv;
			return -1;
		} else {
			if (ft->path)
				free(ft->path);
			//node_del(ft->node);
			if (ft->dirf)
				fclose(ft->dirf);

			free(fdprivate);
			fdprivate = NULL;
			return rv;
		}
	}
}

off_t vu_vufuse_lseek(int fd, off_t offset, int whence, void *fdprivate)
{
	if (fd < 0 || fdprivate == NULL) {
		errno = EBADF;
		return -1;
	} else {
		struct fileinfo *ft =(struct fileinfo *)fdprivate;

		printkdebug(F,"LSEEK", NULL);
		switch (whence) {
			case SEEK_SET:
				ft->pos = offset;
				break;
			case SEEK_CUR:
				ft->pos += offset;
				break;
#if 0
			case SEEK_END:
				ft->pos = ft->size + offset;
				break;
#else
			case SEEK_END:
				{
					struct vu_stat buf;
					int rv = vu_vufuse_lstat(FILEPATH(ft), &buf, 0, fd, fdprivate);
					if (rv < 0)
						return rv;
					else {
						ft->size = buf.st_size;
						ft->pos =  ft->size + offset;
					}
				}
#endif
		}
		return ft->pos;
	}
}

ssize_t vu_vufuse_read (int fd, void *buf, size_t count, void *fdprivate)
{
	if (fd < 0 || fdprivate == NULL) {
		errno = EBADF;
		return -1;
	} else {
		int rv;
		struct fileinfo *ft = (struct fileinfo *)fdprivate;
		if ((ft->ffi.flags & O_ACCMODE) == O_WRONLY) {
			errno = EBADF;
			return -1;
		} else if (ft->pos == ft->size)
			return 0;
		else {
			struct fuse_context fc, *ofc;
			ofc = fuse_push_context(&fc);
			rv = fc.fuse->fops.read(FILEPATH(ft), buf, count, ft->pos, &ft->ffi);
			fuse_pop_context(ofc);

			printkdebug(F,"READ path:%s count:%u status:%s retvalue:%d",FILEPATH(ft), count,rv ? "ERROR" : "SUCCESS", (rv < 0) ? -rv : 0);
			if (rv<0) {
				errno = -rv;
				return -1;
			} else {
				ft->pos += rv;
				return rv;
			}
		}
	}
}

ssize_t vu_vufuse_write(int fd, const void *buf, size_t count, void *fdprivate)
{
	if (fd < 0 || fdprivate == NULL) {
		errno = EBADF;
		return -1;
	} else {
		int rv = 0;

		struct fileinfo *ft = (struct fileinfo *)fdprivate;
		if ((ft->ffi.flags & O_ACCMODE) == O_RDONLY) {
			errno = EBADF;
			return -1;
		} else {
			struct fuse_context fc, *ofc;
			ofc = fuse_push_context(&fc);
			if (ft->ffi.flags & O_APPEND)
				rv = (int)vu_vufuse_lseek(fd,0,SEEK_END,fdprivate);
			if (rv != -1)
				rv = fc.fuse->fops.write(FILEPATH(ft), buf, count, ft->pos, &ft->ffi);
			fuse_pop_context(ofc);

			printkdebug(F,"WRITE path:%s count:%x status:%s retvalue:%d",FILEPATH(ft),count, rv ? "ERROR" : "SUCCESS", (rv < 0) ? -rv : 0);
			if (rv<0) {
				errno = -rv;
				return -1;
			} else {
				ft->pos += rv;
				if (ft->pos > ft->size)
					ft->size = ft->pos;
				return rv;
			}
		}
	}
}

static int vufuse_common_filldir(FILE *f, const char *name, unsigned char type, __ino64_t ino) {
	struct dirent64 entry = {
		.d_ino = ino,
		.d_type = type,
		.d_off = ftello(f),
	};
	static char filler[7];
	unsigned short int namelen = strlen(name) + 1;
	unsigned short int reclen  = offsetof(struct dirent64, d_name) + namelen;
	int ret_value;
	snprintf(entry.d_name, 256, "%s", name);
	/* entries are always 8 bytes aligned */
	entry.d_reclen = (reclen + 7) & (~7);
	ret_value = fwrite(&entry, reclen, 1, f);
	/* add a filler to align the next entry */
	if (entry.d_reclen > reclen)
		ret_value += fwrite(filler, entry.d_reclen - reclen, 1, f);
	return 0;
}

/* Function to add an entry in a readdir() operation */
static int vufusefillreaddir(void *buf, const char *name, const struct stat *stbuf, off_t off) {
	FILE *f = buf;
	__ino64_t d_ino;
	unsigned char d_type;
	if (stbuf == NULL) {
		d_ino = -1;
		d_type = 0;
	} else {
		d_ino = stbuf->st_ino;
		d_type = stbuf->st_mode >> 12;
	}
	return vufuse_common_filldir(f, name, d_type, d_ino);
}

struct fuse_dirhandle {
	FILE *f;
};

static int vufusefilldir(fuse_dirh_t h, const char *name, int type, ino_t ino) {
	return vufuse_common_filldir(h->f, name, type, ino);
}

int vu_vufuse_getdents64(int fd, struct dirent64 *dirp, unsigned int count, void *fdprivate) {
	if (fd < 0 || fdprivate == NULL) {
		errno = EBADF;
		return -1;
	} else {
		struct fileinfo *ft = (struct fileinfo *)fdprivate;
		size_t freadout;
		printkdebug(F,"GETDENTS", NULL);

		if (ft->dirf == NULL) {
			int rv;
			struct fuse_context fc, *ofc;
			ofc = fuse_push_context(&fc);
			ft->dirf = volstream_open();
			if (fc.fuse->fops.readdir)
				rv = fc.fuse->fops.readdir(FILEPATH(ft), ft->dirf, vufusefillreaddir, 0, &ft->ffi);
			else {
				struct fuse_dirhandle dh = {.f = ft->dirf};
				rv = fc.fuse->fops.getdir(FILEPATH(ft), &dh, vufusefilldir);
			}
			fuse_pop_context(ofc);
			if (rv < 0) {
				fclose(ft->dirf);
				return 0;
			}
		}

		freadout = fread(dirp, 1, count, ft->dirf);
		/* if the buffer is full the last entry might be incomplete.
			 update freadout to drop the last incomplete entry,
			 and seek back the position in the file to reread it
			 from its beginning at the next getdents64 */
		if (freadout == count) {
			unsigned int bpos = 0;
			struct dirent64 *d;
			char *buf = (char *) dirp;
			while (1) {
				d = (struct dirent64 *) (buf + bpos);
				if (count - bpos < offsetof(struct dirent64, d_name))
					break;
				if (bpos + d->d_reclen > count)
					break;
				bpos += d->d_reclen;
			}
			if (bpos < count) {
				fseeko(ft->dirf, - (int) (count - bpos), SEEK_CUR);
				freadout -= count - bpos;
			}
			/* the buffer is so short that it does not fit one
				 entry. Return EINVAL! */
			if (freadout == 0) {
				errno = EINVAL;
				return -1;
			}
		}
		return freadout;
	}
}