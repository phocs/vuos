/*
 *   VUOS: view OS project
 *   Copyright (C) 2018  Renzo Davoli <renzo@cs.unibo.it>
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

#include <time.h>
#include <ctype.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include <sys/stat.h>
#include <sys/types.h>

#include <dlfcn.h>
#include <fcntl.h>

#include <vudev.h>
#include <strcase.h>
#include <vumodule.h>

VU_PROTOTYPES(vudev)

  struct vu_module_t vu_module = {
    .name = "vudev",
    .description = "vu virtual devices"
  };

static __thread struct vudevfd_t *cur_vudevfd = NULL;

/******************************************************************************/
/************************************UTILS*************************************/

void *vudev_get_fdprivate(void) {
	if (cur_vudevfd == NULL)
		return NULL;
	else
		return cur_vudevfd->fdprivate;
}

void vudev_set_fdprivate(void *fdprivate) {
	if (cur_vudevfd != NULL)
		cur_vudevfd->fdprivate = fdprivate;
}

void *vudev_get_private_data(void) {
	struct vudev_t *vudev = vu_get_ht_private_data();
	if (vudev == NULL)
		return NULL;
	else
		return vudev->private_data;
}

void vudev_set_nsubdev(unsigned int n) {
  struct vudev_t *vudev = vu_get_ht_private_data();
  vudev->nsubdev = n;
}

dev_t vudev_get_devno(void) {
  if (cur_vudevfd != NULL)
    return cur_vudevfd->devno;
  else
    return 0;
}

static inline int _get_minorno(struct vudev_t *vudev, const char *pathname) {
  if(atoi(pathname) > 0)
    return atoi(pathname);
  else if(strcmp(pathname, "/") == 0 || strcmp(pathname, vudev->target) == 0)
    return 0;
  else if(strncmp(pathname, vudev->target, vudev->targetlen) == 0)
    return atoi(pathname + vudev->targetlen);
  else
    return -1;
}

static int vudev_confirm(uint8_t type, void *arg, int arglen, struct vuht_entry_t *ht) {
  struct vudev_t *vudev = vuht_get_private_data(ht);
  switch (type) {
    case CHECKPATH: {
      //vudev_printd("CHECKPATH [%s]", (char *) arg);
      int minorno = _get_minorno(vudev, arg);
      return (minorno != -1 && minorno <= (int) vudev->nsubdev);
    }
    case CHECKCHRDEVICE:
    case CHECKBLKDEVICE: {
      dev_t *dev = arg;
      //vudev_printd("CHECKCHRDEVICE|CHECKBLKDEVICE ret [%d]",(major(*dev) == major(vudev->stat.st_rdev) && minor(*dev) <= minor(vudev->stat.st_rdev) + vudev->nsubdev));
      return (major(*dev) == major(vudev->stat.st_rdev) && minor(*dev) <= minor(vudev->stat.st_rdev) + vudev->nsubdev);
    }
    case CHECKIOCTL: {
      unsigned long *request = arg;
      if(*request == BLKSSZGET)
        vudev_printd("CHECKIOCTL request [%d] ret [%d]", *request, (vudev->devops->ioctl_parms(*request) != 0));
      return (vudev->devops->ioctl_parms(*request) != 0);
    }
  }
  return 0;
}

/******************************************************************************/
/********************************MOUNT**OPTIONS********************************/

static void set_mount_options(const char *args, struct vudev_t *vudev) {
  if (args == NULL) return;
  int n;
  char *key, *val;
  char *s, *old;
  s = old = strdup(args);
  for(n = 0; *s != 0; n++) {
    s = parse_mount_option(s, &key, &val);
    switch(strcase(key)) {
      case STRCASE(m,o,d,e): {
        mode_t mode;
        sscanf(val,"%o",&mode);
        vudev->stat.st_mode = (vudev->stat.st_mode & S_IFMT) | (mode & 0777);
        break;
      }
      case STRCASE(u,i,d):
        vudev->stat.st_uid = atoi(val);
        break;
      case STRCASE(g,i,d):
        vudev->stat.st_gid = atoi(val);
        break;
      case STRCASE(m,a,j,o,r):
        vudev->stat.st_rdev = makedev(atoi(val), minor(vudev->stat.st_rdev));
        break;
      case STRCASE(m,i,n,o,r):
        vudev->stat.st_rdev = makedev(major(vudev->stat.st_rdev), atoi(val));
        break;
      case STRCASE(d,e,v,n,o): {
        if(S_ISCHR(vudev->stat.st_mode))
          vudev->mjmi_ht = vuht_add(CHECKCHRDEVICE, NULL, 0, vu_mod_getservice(), vudev_confirm, vudev, 0);
        else if(S_ISBLK(vudev->stat.st_mode))
          vudev->mjmi_ht = vuht_add(CHECKBLKDEVICE, NULL, 0, vu_mod_getservice(), vudev_confirm, vudev, 0);
      }
    }
  }
  free(old);
}

/******************************************************************************/
/***********************************SYSCALL************************************/

int vu_vudev_open(const char *pathname, int flags, mode_t mode, void **fdprivate) {
  vudev_printd("[%s] flags [%d] mode [%o]", pathname, flags, mode);
  /* access control? */
  int ret_value = 0;
  struct vudev_t *vudev = vu_get_ht_private_data();
  if((cur_vudevfd = calloc(1, sizeof(struct vudevfd_t))) == NULL){
    vudev_perror("cur_vudevfd = calloc"); return -1;
  }
  cur_vudevfd->devno = makedev(major(vudev->stat.st_rdev), _get_minorno(vudev, pathname));
  cur_vudevfd->open_flags = flags & ~(O_CREAT | O_EXCL | O_NOCTTY | O_TRUNC);
  cur_vudevfd->vudev = vudev;
  if(vudev->devops->open)
    ret_value = vudev->devops->open(pathname, flags, mode);
  if(ret_value < 0) {
    vudev_perror("ret_value < 0");
    free(cur_vudevfd); cur_vudevfd = NULL;
  }
  *fdprivate = cur_vudevfd; /* NULL if an error has occurred */
  return ret_value;
}

int vu_vudev_close(int fd, void *fdprivate) {
  vudev_printd("fd [%d] fdprivate [%p]", fd, fdprivate);
  int ret_value = 0;
  cur_vudevfd = fdprivate;
  if (cur_vudevfd->vudev->devops->close)
    ret_value = cur_vudevfd->vudev->devops->close(fd);
  if(ret_value == 0) {
    free(cur_vudevfd);
    fdprivate = NULL;
  }
  return ret_value;
}

ssize_t vu_vudev_read(int fd, void *buf, size_t count, void *fdprivate) {
  vudev_printd("fd [%d] buf [%p] count [%d]", fd, buf, count);
  int ret_value = 0;
  cur_vudevfd = fdprivate;
  if((cur_vudevfd->open_flags&O_WRONLY) != 0) {
    errno = EBADF; return -1;
  }
  if(cur_vudevfd->vudev->devops->read)
    ret_value = cur_vudevfd->vudev->devops->read(fd, buf, count);
  vudev_printd("read: [%lu]", ret_value);
  return ret_value;
}

ssize_t vu_vudev_write(int fd, const void *buf, size_t count, void *fdprivate) {
  vudev_printd("fd [%d] buf [%p] count [%d]", fd, buf, count);
  int ret_value = 0;
  cur_vudevfd = fdprivate;
  if((cur_vudevfd->open_flags&O_RDONLY) != 0) {
    errno = EBADF; return -1;
  }
  if(cur_vudevfd->vudev->devops->write)
    ret_value = cur_vudevfd->vudev->devops->write(fd, buf, count);
  vudev_printd("wrote: [%lu]", ret_value);
  return ret_value;
}

ssize_t vu_vudev_pread64(int fd, void *buf, size_t count, off_t offset, int flags, void *fdprivate) {
  vudev_printd("fd [%d] buf [%p] count [%d] offset [%d] flags [%d]", fd, buf, count, offset, flags);
  int ret_value = 0;
  cur_vudevfd = fdprivate;
  if((cur_vudevfd->open_flags&O_RDONLY) != 0) {
    errno = EBADF; return -1;
  }
  if(cur_vudevfd->vudev->devops->pwrite64)
    ret_value = cur_vudevfd->vudev->devops->pread64(fd, buf, count, offset);
  vudev_printd("read: [%lu]", count);
  return ret_value;
}

ssize_t vu_vudev_pwrite64(int fd, const void *buf, size_t count, off_t offset, int flags, void *fdprivate) {
  printkdebug(D, "vudev_pwrite64: fd [%d] buf [%p] count [%d] offset [%d] flags [%d]", fd, buf, count, offset, flags);
  int ret_value = 0;
  cur_vudevfd = fdprivate;
  if((cur_vudevfd->open_flags&O_WRONLY) != 0) {
    errno = EBADF; return -1;
  }
  if(cur_vudevfd->vudev->devops->read)
    ret_value = cur_vudevfd->vudev->devops->pwrite64(fd, buf, count, offset);
  vudev_printd("wrote: [%lu]", ret_value);
  return ret_value;
}

int vu_vudev_access(char *path, int mode, int flags) {
  vudev_printd("path [%s] mode [%d] flags [%d]", path, mode, flags);
  return 0;
}

off_t vu_vudev_lseek(int fd, off_t offset, int whence, void *fdprivate) {
  vudev_printd("fd [%d] offset [%lu] whence [%d]", fd, offset, whence);
  cur_vudevfd = fdprivate;
  if(cur_vudevfd->vudev->devops->lseek == NULL) {
    errno = ENOSYS; return -1;
  }
  return cur_vudevfd->vudev->devops->lseek(fd, offset, whence);
}

int vu_vudev_lstat(char *pathname, struct vu_stat *buf, int flags, int sfd, void *fdprivate) {
  vudev_printd("path [%s] buf [%p] flags [%d] sfd [%d] fdprivate [%p]", pathname, buf, flags, sfd, fdprivate);
  cur_vudevfd = fdprivate;
  struct vudev_t *vudev = vu_get_ht_private_data();
  memcpy(buf, &vudev->stat, sizeof(struct vu_stat));
  buf->st_rdev = makedev(major(vudev->stat.st_rdev), _get_minorno(vudev, pathname));
  return 0;
}

int vu_vudev_lchown(const char *pathname, uid_t owner, gid_t group, int fd, void *fdprivate) {
  vudev_printd("path [%s] owner [%d] group [%d] fd [%d]", pathname, owner, group, fd);
  struct vudev_t *vudev = vu_get_ht_private_data();
  /* access control */
  if(getuid() != 0 && getuid() != vudev->stat.st_uid) {
    errno = EACCES;
    return -1;
  }
	if (owner != (uid_t) -1)
		vudev->stat.st_uid = owner;
	if (group != (gid_t) -1)
		vudev->stat.st_gid = group;
  vudev->stat.st_ctime = time(NULL);
	return 0;
}

int vu_vudev_chmod(const char *pathname, mode_t mode, int fd, void *fdprivate) {
  vudev_printd("path [%s] mode [%o] fd [%d]", pathname, mode, fd);
  struct vudev_t *vudev = vu_get_ht_private_data();
  /* access control */
  if(getuid() != 0 && getuid() != vudev->stat.st_uid) {
    errno = EACCES;
    return -1;
  }
  vudev->stat.st_mode = (vudev->stat.st_mode&S_IFMT) | (mode&(S_IRWXU|S_IRWXG|S_IRWXO));
  vudev->stat.st_ctime = time(NULL);
	return 0;
}

int vu_vudev_mount(const char *source, const char *target,
  const char *filesystemtype, unsigned long mountflags, const void *data) {
  vudev_printd("source [%s] target [%s] filesystemtype [%s] mountflags [%u]", source, target, filesystemtype, mountflags);
  struct vudev_operations_t *devops = NULL;
  void *dlhandle = vu_mod_dlopen(filesystemtype, RTLD_NOW);
  if(dlhandle == NULL || (devops = dlsym(dlhandle,"vudev_ops")) == NULL) {
    vudev_perror("dlhandle");
		if(dlhandle != NULL)
			dlclose(dlhandle);
		errno = ENODEV;
		return -1;
	}
  struct vudev_t *new;
  struct vu_service_t *s = vu_mod_getservice();
  if((new =  calloc(1, sizeof(struct vudev_t))) == NULL) {
    vudev_perror("new = calloc");
    dlclose(dlhandle);
    return -1;
  }
  if(vu_stat(source, &new->stat) == -1)
    vudev_perror("vu_stat");
  new->targetlen = strlen(target);
  new->target = strdup(target);
  new->dlhandle = dlhandle;
  new->private_data = NULL;
  new->stat.st_uid = 0;
  new->stat.st_gid = 0;
  new->stat.st_size = 0;
  new->stat.st_blocks = 0;
  new->stat.st_blksize = (new->stat.st_blksize)? new->stat.st_blksize:4096;
  new->stat.st_mode = ((new->stat.st_mode & S_IFMT) | 0666);
  new->stat.st_mtime = new->stat.st_atime = time(NULL);
  new->devops = devops;
  int ret_value = 0;
  if(devops->init != NULL)
    ret_value = devops->init(source, mountflags, data, new);
  if(ret_value < 0) {
    printkdebug(D, "vudev_mount devops_init error: [%d]", errno);
    free(new->target);
    free(new);
    return -1;
  }
  set_mount_options(data, new);
  new->path_ht = vuht_pathadd(CHECKPATH, source, target, filesystemtype, mountflags, data, s, new->nsubdev, vudev_confirm, new);
  if (new->devops->ioctl_parms)
    new->ioctl_ht = vuht_add(CHECKIOCTL, NULL, 0, s, vudev_confirm, new, 0);
  return 0;
}

int vu_vudev_umount2(const char *target, int flags) {
  printkdebug(D, "vudev_umount2: target [%s] flags [%d]", target, flags);
  int ret_value;
  struct vudev_t *vudev = vu_get_ht_private_data();
  if(vudev->mjmi_ht)
    vuht_del(vudev->mjmi_ht, flags);
  if (vudev->ioctl_ht)
    vuht_del(vudev->ioctl_ht, flags);
  if((ret_value = vuht_del(vudev->path_ht, flags)) < 0) {
		errno = -ret_value;
    return -1;
  }
	return 0;
}

void vu_vudev_cleanup(uint8_t type, void *arg, int arglen, struct vuht_entry_t *ht) {
  printkdebug(D, "vudev_cleanup: type [%u]", type);
  struct vudev_t *vudev = vuht_get_private_data(ht);
  if(type == CHECKCHRDEVICE || type == CHECKBLKDEVICE)
    vudev->mjmi_ht = NULL;
  else if (type == CHECKIOCTL)
    vudev->ioctl_ht = NULL;
  else if(type == CHECKPATH)
    vudev->path_ht = NULL;
  if(vudev->mjmi_ht == NULL && vudev->ioctl_ht == NULL && vudev->path_ht == NULL) {
    if(vudev->devops->fini)
      vudev->devops->fini(vudev->private_data);
    dlclose(vudev->dlhandle);
    free(vudev->target);
    free(vudev);
  }
}

int vu_vudev_ioctl(int fd, unsigned long request, void *buf, uintptr_t addr, void *fdprivate) {
  if(fd == -1) {
    int ret_value;
    struct vudev_t *vudev = vu_get_ht_private_data();
		if(vudev->devops->ioctl_parms == NULL ||
      (ret_value = vudev->devops->ioctl_parms(request)) == 0) {
      errno = ENOSYS; return -1;
    }
    return ret_value;
  }
  printkdebug(D, "vudev_ioctl: fd [%d] request [%X] buf [%p] addr [%p] fdprivate [%p]", fd, request, buf, addr, fdprivate);
  cur_vudevfd = fdprivate;
  if(cur_vudevfd->vudev->devops->ioctl == NULL) {
    errno = ENOSYS; return -1;
  }
  return cur_vudevfd->vudev->devops->ioctl(fd, request, buf);
}

int vu_vudev_epoll_ctl(int epfd, int op, int fd, struct epoll_event *event, void *fdprivate) {
  cur_vudevfd = fdprivate;
	if(cur_vudevfd->vudev->devops->epoll_ctl == NULL) {
    errno = ENOSYS; return -1;
  } else
    return cur_vudevfd->vudev->devops->epoll_ctl(epfd, op, fd, event);
}

void *vu_vudev_init(void) {
	return NULL;
}

int vu_vudev_fini(void *fdprivate) {
	return 0;
}

__attribute__((constructor))
  static void init(void) {
		debug_set_name(D, "VUDEV");
	}

__attribute__((destructor))
  static void fini(void) {
		debug_set_name(D, "");
	}
