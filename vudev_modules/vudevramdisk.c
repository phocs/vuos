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

#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/ioctl.h>

#include <asm/ioctl.h>

#include <linux/hdreg.h>

#include <vudev.h>
#include <strcase.h>
#include <vumodule.h>

#define STD_SIZE 64*1024
#define STD_SECTORSIZE 512

#define READONLY 1
#define MBR 2

#define GET_CYLINDERS(ramdisk) \
  ((ramdisk->rd_size + (ramdisk->geometry.heads*ramdisk->geometry.sectors) -1) / (ramdisk->geometry.heads*ramdisk->geometry.sectors))

#define RAMDISK_SIZE(ramdisk) (ramdisk->rd_size * STD_SECTORSIZE)

struct vuramdisk_t {
  char flags;
	char *diskdata;
	size_t rd_size;
	struct hd_geometry geometry;
};

struct vuramdiskfd_t {
  off_t offset;
};

/******************************************************************************/
/************************************UTILS*************************************/

static inline ssize_t _get_size(char unit, ssize_t size) {
  switch (unit) {
    case 'k':
		case 'K': size *= 1024 / STD_SECTORSIZE; return size;
		case 'm':
		case 'M': size *= 1024 * 1024 / STD_SECTORSIZE; return size;
		case 'g':
		case 'G': size *= 1024 * 1024 * 1024 / STD_SECTORSIZE; return size;
    default: return size;
  }
}

static void set_mount_options(const char *args, struct vuramdisk_t *ramdisk) {
  if (args == NULL) return;
  int n;
  char *key, *val;
  char *s, *old;
  s = old = strdup(args);
  for(n = 0; *s != 0; n++) {
    s = parse_mount_option(s, &key, &val);
    switch(strcase(key)) {
      case STRCASE(s,i,z,e):
        ramdisk->rd_size = _get_size(val[strlen(val)-1], atoi(val));
        break;
      case STRCASE(m,b,r):
        ramdisk->flags = MBR;
        break;
    }
  }
  free(old);
}

static inline ssize_t _wrap_count_size(size_t count, off_t offset) {
  struct vuramdisk_t *ramdisk = vudev_get_private_data();
  if((size_t) offset >= RAMDISK_SIZE(ramdisk))
    return 0;
  count = (offset + count <= RAMDISK_SIZE(ramdisk))? count:(RAMDISK_SIZE(ramdisk) - offset);
  return count;
}

ssize_t _vuramdisk_pread64(void *buf, size_t count, off_t offset) {
  struct vuramdisk_t *ramdisk = vudev_get_private_data();
  count = _wrap_count_size(count, offset);
  memcpy(buf, (ramdisk->diskdata + offset), count);
  return count;
}

ssize_t _vuramdisk_pwrite64(const void *buf, size_t count, off_t offset) {
  struct vuramdisk_t *ramdisk = vudev_get_private_data();
  count = _wrap_count_size(count, offset);
  memcpy((ramdisk->diskdata + offset), buf, count);
  return count;
}

/******************************************************************************/
/***********************************SYSCALL************************************/

int vuramdisk_open(const char *pathname, int flags, mode_t mode) {
  struct vuramdiskfd_t *ramdiskfd;
  if((ramdiskfd = calloc(1, sizeof(struct vuramdiskfd_t))) == NULL)
    return -1;
  vudev_set_fdprivate(ramdiskfd);
  return 0;
}

int vuramdisk_close(int fd) {
  struct vuramdiskfd_t *ramdiskfd = vudev_get_fdprivate();
  free(ramdiskfd);
  return 0;
}

ssize_t vuramdisk_read(int fd, void *buf, size_t count) {
  struct vuramdiskfd_t *ramdiskfd = vudev_get_fdprivate();
  size_t ret_value = _vuramdisk_pread64(buf, count, ramdiskfd->offset);
  ramdiskfd->offset += ret_value;
  return ret_value;
}

ssize_t vuramdisk_write(int fd, const void *buf, size_t count) {
  struct vuramdisk_t *ramdisk = vudev_get_private_data();
  if(ramdisk->flags & READONLY) {
    errno = EBADF; return -1;
  }
  struct vuramdiskfd_t *ramdiskfd = vudev_get_fdprivate();
  size_t ret_value = _vuramdisk_pwrite64(buf, count, ramdiskfd->offset);
  ramdiskfd->offset += ret_value;
	return ret_value;
}

ssize_t vuramdisk_pread64(int fd, void *buf, size_t count, off_t offset) {
  return _vuramdisk_pread64(buf, count, offset);
}

ssize_t vuramdisk_pwrite64(int fd, const void *buf, size_t count, off_t offset) {
  struct vuramdisk_t *ramdisk = vudev_get_private_data();
  if(ramdisk->flags & READONLY) {
    errno = EBADF; return -1;
  }
  return _vuramdisk_pwrite64(buf, count, offset);
}

off_t vuramdisk_lseek(int fd, off_t offset, int whence) {
  off_t ret_value;
	struct vuramdiskfd_t *ramdiskfd = vudev_get_fdprivate();
	switch (whence) {
		case SEEK_SET: ret_value = ramdiskfd->offset = offset; break;
		case SEEK_CUR: ret_value = ramdiskfd->offset = ramdiskfd->offset + offset; break;
		case SEEK_END: {
      struct vuramdisk_t *ramdisk = vudev_get_private_data();
      ret_value = ramdiskfd->offset = RAMDISK_SIZE(ramdisk) + offset; break;
    }
    default: errno = EINVAL; ret_value = (off_t) -1; break;
	}
	return ret_value;
}


long vuramdisk_ioctl_parms(unsigned long request) {
  //printkdebug(D, "vumbr_ioctl_parms 0x%X", request);
  switch (request) {
    case HDIO_GETGEO: return _IOW('D', 0, struct hd_geometry);
    default: return 0;
	}
}

int vuramdisk_ioctl(int fd, unsigned long request, void *addr){
  switch (request) {
    case BLKROGET: {
      struct vuramdisk_t *ramdisk = vudev_get_private_data();
      *(int *)addr = (ramdisk->flags & READONLY);
      printd("BLKROGET [%d]", (ramdisk->flags & READONLY));
      break;
    }
    case BLKROSET:{
      struct vuramdisk_t *ramdisk = vudev_get_private_data();
      ramdisk->flags |= (*(int *)addr > 0)? READONLY:0;
      break;
    }
    case BLKSSZGET:
      *(int *)addr = STD_SECTORSIZE;
  		break;
    case BLKGETSIZE: {
      struct vuramdisk_t *ramdisk = vudev_get_private_data();
      *(int *)addr = ramdisk->rd_size * ((ramdisk->flags & MBR)? 1:STD_SECTORSIZE);
			break;
    }
    case BLKGETSIZE64: {
      struct vuramdisk_t *ramdisk = vudev_get_private_data();
      *(long long *)addr = ramdisk->rd_size * STD_SECTORSIZE;
      break;
    }
    case BLKRRPART: break;
    case HDIO_GETGEO: {
      struct vuramdisk_t *ramdisk = vudev_get_private_data();
      memcpy(addr, &(ramdisk->geometry), sizeof(struct hd_geometry));
      break;
    }
    default:
      printd("UNDEFINE: [%lu]", request);
      errno = EINVAL;
      return -1;
  }
  return 0;
}

int vuramdisk_init(const char *source, unsigned long flags, const char *args, struct vudev_t *vudev) {
  struct vuramdisk_t *ramdisk;
  if((ramdisk = calloc(1, sizeof(struct vuramdisk_t))) == NULL)
    return -1;
  set_mount_options(args, ramdisk);
  if(ramdisk->rd_size == 0)
		ramdisk->rd_size = STD_SIZE;
  ramdisk->geometry.start = 0;
  if (ramdisk->rd_size == (unsigned int) ramdisk->rd_size) { /* 32 */
    ramdisk->geometry.heads = 16;
		ramdisk->geometry.sectors = 16;
  } else { /* 64 */
		ramdisk->geometry.heads = 128;
		ramdisk->geometry.sectors = 128;
	}
  ramdisk->geometry.cylinders = GET_CYLINDERS(ramdisk);
  ramdisk->rd_size = ramdisk->geometry.heads * ramdisk->geometry.sectors * ramdisk->geometry.cylinders;
  if((ramdisk->diskdata = calloc(1, ramdisk->rd_size * STD_SECTORSIZE)) == NULL) {
    free(ramdisk); return -1;
  }
  vudev->stat.st_mode = (vudev->stat.st_mode & ~S_IFMT) | S_IFBLK;
	vudev->private_data = ramdisk;
  return 0;
}

int vuramdisk_fini(void *private_data) {
  struct vuramdisk_t *ramdisk = private_data;
	if (ramdisk) {
		free(ramdisk->diskdata);
		free(ramdisk);
    private_data = NULL;
	}
	return 0;
}

struct vudev_operations_t vudev_ops = {
  .open = vuramdisk_open,
  .close = vuramdisk_close,
  .read = vuramdisk_read,
  .write = vuramdisk_write,
	.pread64 = vuramdisk_pread64,
	.pwrite64 = vuramdisk_pwrite64,
  .lseek = vuramdisk_lseek,
  .ioctl = vuramdisk_ioctl,
  .ioctl_parms = vuramdisk_ioctl_parms,
  .init = vuramdisk_init,
  .fini = vuramdisk_fini,
};
