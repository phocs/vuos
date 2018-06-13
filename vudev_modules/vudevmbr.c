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

#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ioctl.h>

//#include <asm/ioctl.h>

#include <linux/hdreg.h>

#include <vudev.h>
#include <vumodule.h>


#include <linux/blkpg.h>
#include <linux/fs.h>

#define IDE_MAXPART 63
#define IDE_BLOCKSIZE 512
#define IDE_BLOCKSIZE_LOG 9
#define IDE_HEADER_OFFSET 446

#define LE32_INT(X) (((X)[0])+(((X)[1])<<8)+(((X)[2])<<16)+(((X)[3])<<24))

#define PART_ADDRBASE(partition) (((off_t) partition->LBAbegin) << IDE_BLOCKSIZE_LOG)
#define PART_ADDRMAX(partition) (((off_t) partition->LBAnoblocks) << IDE_BLOCKSIZE_LOG)

struct vupartition_t {
	int flags;
	char type;
	unsigned int LBAbegin;
	unsigned int LBAnoblocks;
};

struct vumbr_t {
  int fd;
  off_t size;
  int rdonly[IDE_MAXPART+1];
  struct hd_geometry geometry;
  struct vupartition_t *part[IDE_MAXPART];
};

struct vumbrfd_t {
  int *rdonly;
  off_t offset;
//  struct vumbr_t *vumbr;
  struct vupartition_t *partition;
};

/******************************************************************************/
/************************************UTILS*************************************/

static inline void _read_mbr_geometry(struct hd_geometry *geom, const unsigned char *chs) {
  unsigned char h = chs[0];
	unsigned char s = chs[1] & 0x3f;
	unsigned short c = chs[2] + ((chs[1] & 0xc0) << 2);
	if ((h+1) > geom->heads)
		geom->heads = h+1;
	if (s > geom->sectors)
		geom->sectors = s;
	if (c > geom->cylinders)
		geom->cylinders = c;
}

/* return number subdev */
static int _read_mbr(struct vumbr_t *vumbr) {
  char vumbr_signature[2] = {0x55, 0xAA};
  unsigned int ext_part_base = 0;
  struct {
  	char code[IDE_HEADER_OFFSET];
  	struct {
  		unsigned char flags;
  		unsigned char chs_begin[3];
  		unsigned char type;
  		unsigned char chs_end[3];
  		unsigned char lba_begin[4];
  		unsigned char lba_noblocks[4];
  	} vumbrpart[4];
  	unsigned char signature[2];
  } vumbr_header;
	pread64(vumbr->fd, &vumbr_header, sizeof(vumbr_header), (off_t) 0);
	if(memcmp(vumbr_header.signature, vumbr_signature, 2) != 0) {
    printkdebug(D, "Bad signature in vumbr %x %x\n", vumbr_header.signature[0], vumbr_header.signature[1]);
    return 0;
  }
  /* MBR is okay. Read MBR */
  int i, partno;
  unsigned int offset = 0;
  for (i = partno = 0; i < 4; i++) {
    if(vumbr_header.vumbrpart[i].type != 0) {
      struct vupartition_t *new = vumbr->part[partno++] = malloc(sizeof(struct vupartition_t));
      _read_mbr_geometry(&(vumbr->geometry), vumbr_header.vumbrpart[i].chs_end);
      new->flags = vumbr_header.vumbrpart[i].flags;
      new->type = vumbr_header.vumbrpart[i].type;
      new->LBAbegin = LE32_INT(vumbr_header.vumbrpart[i].lba_begin);
      new->LBAnoblocks = LE32_INT(vumbr_header.vumbrpart[i].lba_noblocks);
      if(vumbr_header.vumbrpart[i].type == 5) {/* extended partition*/
        if (ext_part_base == 0)
          ext_part_base = new->LBAbegin;
        else
          vudev_printd("There are more than one extended partitions against the specifications", vumbr_header.vumbrpart[i].type);
      }
    }
  }
  if(vumbr->geometry.heads == 0)
    vumbr->geometry.heads = 255;
  if(vumbr->geometry.sectors == 0)
    vumbr->geometry.sectors = 63;
  vumbr->geometry.cylinders = (vumbr->size >> IDE_BLOCKSIZE_LOG) / (vumbr->geometry.heads * vumbr->geometry.sectors);
  /* Read the chain of logical partitions inside the extended partition */
  while (ext_part_base > 0) {
    off_t base = ((off_t)(ext_part_base + offset)) << IDE_BLOCKSIZE_LOG;
    pread64(vumbr->fd, &vumbr_header, sizeof(vumbr_header), base);
    if(memcmp(vumbr_header.signature, vumbr_signature, 2) != 0) {
      printkdebug(D, "Bad signature in block %lld=%x %x\n", base, vumbr_header.signature[0],vumbr_header.signature[1]);
      ext_part_base = 0;
    } else {
      if(vumbr_header.vumbrpart[0].type != 0) {
        struct vupartition_t *new = vumbr->part[partno++] = malloc(sizeof(struct vupartition_t));
        new->flags = vumbr_header.vumbrpart[0].flags;
        new->type = vumbr_header.vumbrpart[0].type;
        new->LBAbegin = LE32_INT(vumbr_header.vumbrpart[0].lba_begin) + ext_part_base + offset;
        new->LBAnoblocks = LE32_INT(vumbr_header.vumbrpart[0].lba_noblocks);
      }
      if(vumbr_header.vumbrpart[1].type == 5)
        offset = LE32_INT(vumbr_header.vumbrpart[1].lba_begin);
      else
        ext_part_base=0;
    }
  }
  return partno;
}

static inline ssize_t _wrap_offset(struct vupartition_t *partition, off_t offset) {
	if(partition && ((offset >> IDE_BLOCKSIZE_LOG) <= partition->LBAnoblocks))
    offset += PART_ADDRBASE(partition);
  return offset;
}

static inline size_t _vumbr_pread64(int fd, void *buf, size_t count, off_t offset, struct vumbrfd_t *vumbrfd) {
  return pread64(fd, buf, count, _wrap_offset(vumbrfd->partition, vumbrfd->offset));
}

static inline size_t _vumbr_pwrite64(int fd, const void *buf, size_t count, off_t offset, struct vumbrfd_t *vumbrfd) {
  return pwrite64(fd, buf, count, _wrap_offset(vumbrfd->partition, vumbrfd->offset));
}

/******************************************************************************/
/***********************************SYSCALL************************************/

int vumbr_open(const char *pathname, int flags, mode_t mode) {
  struct vumbr_t *vumbr = vudev_get_private_data();
  struct vumbrfd_t *vumbrfd;
	if((vumbrfd = calloc(1, sizeof(struct vumbrfd_t))) == NULL)
		return -1;
  int partno = minor(vudev_get_devno());
  vumbrfd->rdonly = &(vumbr->rdonly[partno]);
  if(partno > 0 && vumbr->part[partno-1] == NULL) {
    free(vumbrfd); return -1;
  } else
    vumbrfd->partition = vumbr->part[partno-1];
	vudev_set_fdprivate(vumbrfd);
  return vumbr->fd;
}

int vumbr_close(int fd) {
  struct vumbrfd_t *mbrfd= vudev_get_fdprivate();
  free(mbrfd);
  return 0;
}

ssize_t vumbr_read(int fd, void *buf, size_t count) {
  struct vumbrfd_t *vumbrfd = vudev_get_fdprivate();
  ssize_t ret_value = _vumbr_pread64(fd, buf, count, vumbrfd->offset, vumbrfd);
  if(ret_value != -1)
    vumbrfd->offset += ret_value;
  return ret_value;
}

ssize_t vumbr_write(int fd, const void *buf, size_t count) {
  struct vumbrfd_t *vumbrfd = vudev_get_fdprivate();
  if(*(vumbrfd->rdonly)) {
    errno = EBADF; return -1;
  }
  ssize_t ret_value = _vumbr_pwrite64(fd, buf, count, vumbrfd->offset, vumbrfd);
  if(ret_value != -1)
    vumbrfd->offset += ret_value;
  return ret_value;
}

ssize_t vumbr_pread64(int fd, void *buf, size_t count, off_t offset) {
	return _vumbr_pread64(fd, buf, count, offset, vudev_get_fdprivate());
}

ssize_t vumbr_pwrite64(int fd, const void *buf, size_t count, off_t offset) {
  struct vumbrfd_t *vumbrfd= vudev_get_fdprivate();
  if(*(vumbrfd->rdonly)) {
    errno = EBADF; return -1;
  }
	return _vumbr_pwrite64(fd, buf, count, offset, vumbrfd);
}

off_t vumbr_lseek(int fd, off_t offset, int whence) {
  off_t ret_value;
  struct vumbrfd_t *vumbrfd = vudev_get_fdprivate();
	switch (whence) {
		case SEEK_SET:
      ret_value = vumbrfd->offset = offset;
      break;
		case SEEK_CUR:
      ret_value = vumbrfd->offset = vumbrfd->offset + offset;
      break;
		case SEEK_END: {
		  if(vumbrfd->partition == NULL) {
        struct vumbr_t *vumbr = vudev_get_private_data();
			  ret_value = vumbrfd->offset = vumbr->size + offset;
		  } else
			  ret_value = vumbrfd->offset = PART_ADDRMAX(vumbrfd->partition) + offset;
		  break;
    }
    default: errno = EINVAL; ret_value = (off_t) -1; break;
	}
  return ret_value;
}

unsigned long vumbr_ioctl_parms(unsigned long request) {
	unsigned long parameter;
	VUDEV_GET_IOCTL_PARM(request, parameter);
	return parameter;
}

int vumbr_ioctl(int fd, unsigned long request, void *addr){
  switch (request) {
    case BLKROGET: {
      struct vumbrfd_t *vumbrfd = vudev_get_fdprivate();
      *(int *)addr = *(vumbrfd->rdonly);
      break;
    }
    case BLKROSET:{
      struct vumbrfd_t *vumbrfd = vudev_get_fdprivate();
      *(vumbrfd->rdonly) = (*(int *)addr > 0)? 1:0;
      break;
    }
    case BLKSSZGET:
      *(int *)addr = IDE_BLOCKSIZE;
      break;
    case BLKGETSIZE: {
      struct vumbrfd_t *vumbrfd = vudev_get_fdprivate();
      if(vumbrfd->partition == NULL) {
        struct vumbr_t *vumbr = vudev_get_private_data();
        *((int *) addr) = (vumbr->size >> IDE_BLOCKSIZE_LOG);
      } else {
        *(int *)addr = vumbrfd->partition->LBAnoblocks;
      }
      break;
    }
    case BLKGETSIZE64: {
      struct vumbrfd_t *vumbrfd = vudev_get_fdprivate();
      if(vumbrfd->partition == NULL) {
        struct vumbr_t *vumbr = vudev_get_private_data();
        *(long long *)addr = vumbr->size;
      } else
        *(long long *)addr = (vumbrfd->partition->LBAnoblocks) << IDE_BLOCKSIZE_LOG;
      break;
    }
    case BLKRRPART: {
      int i;
      struct vumbr_t *vumbr = vudev_get_private_data();
      for(i = 0; i < IDE_MAXPART && vumbr->part[i]; i++) {
        free(vumbr->part[i]); vumbr->part[i] = NULL;
      }
      vudev_set_nsubdev(_read_mbr(vumbr));
      break;
    }
    case HDIO_GETGEO: {
      struct vumbr_t *vumbr = vudev_get_private_data();
      memcpy(addr, &(vumbr->geometry), sizeof(struct hd_geometry));
      break;
    }
    default: errno = EINVAL; return -1;
  }
  return 0;
}

int vumbr_init(const char *source, unsigned long flags, const char *args, struct vudev_t *vudev) {
  struct vumbr_t *vumbr;
	if((vumbr = calloc(1, sizeof(struct vumbr_t))) == NULL)
		return -1;
	if((vumbr->fd = open(source, O_RDWR|O_CLOEXEC)) == -1) {
		free(vumbr);
		return -1;
	}
	if((vumbr->size = lseek(vumbr->fd, 0, SEEK_END)) == -1)  {
    if(ioctl(vumbr->fd, BLKGETSIZE64, &(vumbr->size)) < 0) {
      close(vumbr->fd);
  		free(vumbr);
  	  return -1;
    }
	}
  vudev->nsubdev = _read_mbr(vumbr);
  vudev->stat.st_mode = (vudev->stat.st_mode & ~S_IFMT) | S_IFBLK;
  vudev->private_data = vumbr;
	return 0;
}

int vumbr_fini(void *private_data) {
	struct vumbr_t *vumbr = private_data;
  if(vumbr) {
    close(vumbr->fd);
    free(vumbr);
    private_data = NULL;
  }
	return 0;
}

struct vudev_operations_t vudev_ops = {
  .open = vumbr_open,
  .close = vumbr_close,
  .read = vumbr_read,
  .write = vumbr_write,
	.pread64 = vumbr_pread64,
	.pwrite64 = vumbr_pwrite64,
  .lseek = vumbr_lseek,
  .ioctl = vumbr_ioctl,
  .ioctl_parms = vumbr_ioctl_parms,
	.epoll_ctl = epoll_ctl,
  .init = vumbr_init,
  .fini = vumbr_fini,
};
