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
#include <vumodule.h>

#define RDONLY 1
#define STD_SIZE (64*1024)
#define STD_SECTORSIZE (512)

struct {
  char rdonly;
  char *diskdata;
  unsigned long long size;
  struct hd_geometry geometry;
} vuvhd;

struct vuvhdfd_t {
  off_t offset;
};

#define VUVHD_DISKSIZE (vuvhd.size * STD_SECTORSIZE)
#define VUVHD_CYLINDERS \
	((vuvhd.size + (vuvhd.geometry.heads*vuvhd.geometry.sectors) -1) / (vuvhd.geometry.heads*vuvhd.geometry.sectors))

/******************************************************************************/
/************************************UTILS*************************************/

static inline ssize_t _wrap_count_size(size_t count, off_t offset) {
  if((size_t) offset >= VUVHD_DISKSIZE)
    return 0;
  count = (offset + count <= VUVHD_DISKSIZE)? count:(VUVHD_DISKSIZE - offset);
  return count;
}

ssize_t _vuvhd_pread64(void *buf, size_t count, off_t offset) {
  count = _wrap_count_size(count, offset);
  memcpy(buf, (vuvhd.diskdata + offset), count);
  return count;
}

ssize_t _vuvhd_pwrite64(const void *buf, size_t count, off_t offset) {
  count = _wrap_count_size(count, offset);
  memcpy((vuvhd.diskdata + offset), buf, count);
  return count;
}

/******************************************************************************/
/***********************************SYSCALL************************************/

int vuvhd_open(const char *pathname, int flags, mode_t mode) {
  struct vuvhdfd_t *vhdfd;
  if((vhdfd = calloc(1, sizeof(struct vuvhdfd_t))) == NULL)
    return -1;
  vudev_set_fdprivate(vhdfd);
  return 0;
}

int vuvhd_close(int fd) {
  struct vuvhdfd_t *vhdfd = vudev_get_fdprivate();
  free(vhdfd);
  return 0;
}

ssize_t vuvhd_read(int fd, void *buf, size_t count) {
  struct vuvhdfd_t *vhdfd = vudev_get_fdprivate();
  size_t ret_value = _vuvhd_pread64(buf, count, vhdfd->offset);
  vhdfd->offset += ret_value;
  return ret_value;
}

ssize_t vuvhd_write(int fd, const void *buf, size_t count) {
  if(vuvhd.rdonly & RDONLY) {
    errno = EBADF; return -1;
  }
  struct vuvhdfd_t *vhdfd = vudev_get_fdprivate();
  size_t ret_value = _vuvhd_pwrite64(buf, count, vhdfd->offset);
  vhdfd->offset += ret_value;
	return ret_value;
}

ssize_t vuvhd_pread64(int fd, void *buf, size_t count, off_t offset) {
  return _vuvhd_pread64(buf, count, offset);
}

ssize_t vuvhd_pwrite64(int fd, const void *buf, size_t count, off_t offset) {
  if(vuvhd.rdonly & RDONLY) {
    errno = EBADF; return -1;
  }
  return _vuvhd_pwrite64(buf, count, offset);
}

off_t vuvhd_lseek(int fd, off_t offset, int whence) {
  off_t ret_value;
	struct vuvhdfd_t *vhdfd = vudev_get_fdprivate();
	switch (whence) {
		case SEEK_SET: ret_value = vhdfd->offset = offset; break;
		case SEEK_CUR: ret_value = vhdfd->offset = vhdfd->offset + offset; break;
		case SEEK_END: {
      ret_value = vhdfd->offset = VUVHD_DISKSIZE + offset; break;
    }
    default: errno = EINVAL; ret_value = (off_t) -1; break;
	}
	return ret_value;
}

long vuvhd_ioctl_parms(unsigned long request) {
  switch (request) {
    case HDIO_GETGEO: return _IOW('D', 0, struct hd_geometry);
    default: return 0;
	}
}

int vuvhd_ioctl(int fd, unsigned long request, void *addr){
  switch (request) {
    case BLKROGET: {
      *(int *)addr = (vuvhd.rdonly & RDONLY);
      printd("BLKROGET [%d]", (vuvhd.rdonly & RDONLY));
      break;
    }
    case BLKROSET:{
      vuvhd.rdonly |= (*(int *)addr > 0)? RDONLY:0;
      break;
    }
    case BLKSSZGET:
      *(int *)addr = STD_SECTORSIZE;
  		break;
    case BLKGETSIZE: {
      *(int *)addr = VUVHD_DISKSIZE;
			break;
    }
    case BLKGETSIZE64: {
      *(long long *)addr = VUVHD_DISKSIZE;
      break;
    }
    case BLKRRPART: break;
    case HDIO_GETGEO: {
      memcpy(addr, &(vuvhd.geometry), sizeof(struct hd_geometry));
      break;
    }
    default:
      printd("UNDEFINE: [%lu]", request);
      errno = EINVAL;
      return -1;
  }
  return 0;
}

int vuvhd_init(const char *source, unsigned long flags, const char *args, struct vudev_t *vudev) {
  vuvhd.rdonly = 0;
	vuvhd.size = STD_SIZE;
	vuvhd.geometry.start = 0;
	if (vuvhd.size == (unsigned int) vuvhd.size) {
		vuvhd.geometry.heads = 16;
		vuvhd.geometry.sectors = 16;
	} else {
		vuvhd.geometry.heads = 128;
		vuvhd.geometry.sectors = 128;
	}
	vuvhd.geometry.cylinders = VUVHD_CYLINDERS;
	vuvhd.size = vuvhd.geometry.heads * vuvhd.geometry.sectors * vuvhd.geometry.cylinders;
	if ((vuvhd.diskdata = malloc(VUVHD_DISKSIZE)) == NULL) {
		printderror("malloc"); return -1;
	}
  vudev->stat.st_mode = (vudev->stat.st_mode & ~S_IFMT) | S_IFCHR;
	return 0;
}

int vuvhd_fini(void *private_data) {
	free(vuvhd.diskdata);
	return 0;
}

struct vudev_operations_t vudev_ops = {
  .open = vuvhd_open,
  .close = vuvhd_close,
  .read = vuvhd_read,
  .write = vuvhd_write,
	.pread64 = vuvhd_pread64,
	.pwrite64 = vuvhd_pwrite64,
  .lseek = vuvhd_lseek,
  .ioctl = vuvhd_ioctl,
  .ioctl_parms = vuvhd_ioctl_parms,
  .init = vuvhd_init,
  .fini = vuvhd_fini,
};
