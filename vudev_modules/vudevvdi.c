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
#include <dlfcn.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <asm/ioctl.h>

#include <linux/hdreg.h>

#include <vudev.h>
#include <strcase.h>
#include <vumodule.h>

#define STD_SECTORSIZE 512
#define STD_SECTORSIZE_OFFSET_MASK 0x1ff

#define RT_BIT(bit)                 (1UL << (bit))
#define VD_OPEN_FLAGS_NORMAL        0
#define VD_OPEN_FLAGS_READONLY      RT_BIT(0)

static void *vboxdd_hdl;
static int vboxdd_count;

static int (*vboxdd_create) ();
static int (*vboxdd_open) ();
static int (*vboxdd_close) ();
static int (*vboxdd_read) ();
static int (*vboxdd_write) ();
static int (*vboxdd_flush) ();
static uint64_t (*vboxdd_get_size) ();
static int (*vboxdd_get_LCHS_geometry) ();

struct vuvdi_t {
	int fd;
	void *disk;
  size_t size;
  unsigned long flags;
};

struct vuvdifd_t {
  off_t offset;
};

struct vuvdi_pdmmediageom_t {
	uint32_t cHeads;
	uint32_t cSectors;
  uint32_t cCylinders;
};

/******************************************************************************/
/************************************UTILS*************************************/

static int vboxdd_detect_type(int fd, char **disktype) {
	char buf[8];
	read(fd, buf, sizeof(buf));
	if (strncmp (buf, "conectix", 8) == 0)	*disktype = "VHD";
	else if (strncmp (buf, "VMDK", 4) == 0)	*disktype = "VMDK";
	else if (strncmp (buf, "KDMV", 4) == 0) *disktype = "VMDK";
	else if (strncmp (buf, "<<<",  3) == 0) *disktype = "VDI";
	else {
		vudev_perror("cannot autodetect disk type");
		errno = ENODEV; return -1;
	}
	vudev_printd("disktype [%s]\n", *disktype);
	return 0;
}

static int dlload_VBoxDD() {
	if(vboxdd_count)
		return ++vboxdd_count;
	if((vboxdd_hdl = vu_mod_dlopen("/usr/lib/virtualbox/VBoxDD.so", RTLD_NOW)) == NULL) {
		vudev_perror("VBoxDD library: not found");
		return -1;
	}
	vboxdd_create = (int *) dlsym(vboxdd_hdl, "VDCreate");
	vboxdd_open = (int *) dlsym(vboxdd_hdl, "VDOpen");
	vboxdd_close = (int *) dlsym(vboxdd_hdl, "VDClose");
	vboxdd_read = (int *) dlsym(vboxdd_hdl, "VDRead");
	vboxdd_write = (int *) dlsym(vboxdd_hdl, "VDWrite");
	vboxdd_flush = (int *) dlsym(vboxdd_hdl, "VDFlush");
	vboxdd_get_size = (uint64_t *) dlsym(vboxdd_hdl, "VDGetSize");
	vboxdd_get_LCHS_geometry = (int *) dlsym(vboxdd_hdl, "VDGetLCHSGeometry");
	vudev_printd("%p %p %p %p %p %p %p %p\n",
		vboxdd_create, vboxdd_open, vboxdd_close, vboxdd_read,
		vboxdd_write, vboxdd_flush, vboxdd_get_size, vboxdd_get_LCHS_geometry);
	return ++vboxdd_count;
}

static void dlunload_VBoxDD() {
	vboxdd_count--;
	if (vboxdd_count == 0) {
		dlclose(vboxdd_hdl);
		vboxdd_hdl=NULL;
	}
}




/******************************************************************************/
/***********************************SYSCALL************************************/

int vuvdi_open(const char *pathname, int flags, mode_t mode) {
	struct vuvdi_t *vdi = vudev_get_private_data();
  struct vuvdifd_t *vdifd = calloc(1, sizeof(struct vuvdifd_t));
  if(vdifd == NULL)
    return -1;
  vudev_set_fdprivate(vdifd);
  return vdi->fd;
}

int vuvhd_close(int fd) {
  free(vudev_get_fdprivate());
  return 0;
}

static inline ssize_t _vuvdi_pread64(struct vuvdi_t *vdi, void *buf, size_t count, off_t offset) {
	if(count == 0) return 0;
	else if((offset & STD_SECTORSIZE_OFFSET_MASK) == 0 &&
					(count & STD_SECTORSIZE_OFFSET_MASK) == 0) {
		vudev_printd("read [(offset & STD_SECTORSIZE_OFFSET_MASK) == 0 && (count & STD_SECTORSIZE_OFFSET_MASK) == 0]");
		return vboxdd_read(vdi->disk, offset, buf, count);
	} else {
		vudev_printd("read [(offset & STD_SECTORSIZE_OFFSET_MASK) == [%d] && (count & STD_SECTORSIZE_OFFSET_MASK) == [%d]]", (offset & STD_SECTORSIZE_OFFSET_MASK), (count & STD_SECTORSIZE_OFFSET_MASK));
		int ret_value;
		char tmp_buf[STD_SECTORSIZE];
		size_t tmp_offset, tmp_count, tmp_lconut;
		tmp_offset = pos & STD_SECTORSIZE_OFFSET_MASK;
		if(tmp_offset) {
			ret_value = vboxdd_read(vdi->disk, offset - tmp_offset, tmp_buf, STD_SECTORSIZE);
			if(ret_value > 0) {
				tmp_count = STD_SECTORSIZE -
			}
		}
	}
	return 0;
}

ssize_t vuvdi_read(int fd, void *buf, size_t count) {
	int ret_value = 0;
	struct vuvdi_t *vdi = vudev_get_private_data();
	struct vuvdifd_t *vdifd = vudev_get_fdprivate();
	if((ret_value = _vuvdi_pread64(vdi, buf, count, vdifd->offset) != -1)
		vdifd->offset += ret_value;
	return ret_value;
}





long vuvdi_ioctl_parms(unsigned long request) {
  unsigned long parameter;
	VUDEV_GET_IOCTL_PARM(request, parameter);
	return parameter;
}

static int vuvdi_init(const char *source, unsigned long flags, const char *args, struct vudev_t *vudev) {
	struct vuvdi_t *vdi;
	char *disk_type = "auto";
	if (dlload_vboxdd() == -1)
		return -1;
	if((vdi = calloc(1, sizeof(struct vuvdi_t))) == NULL)
		goto exit_err;
	if((vdi->fd = open(source, O_RDONLY)) == -1)
		goto exit_err;
	vdi->flags = flags;
	if(vboxdd_create(NULL, &vdi->disk) < 0) {
		vudev_perror("invalid initialisation of VD interface");
		goto exit_err;
	}
	if(vboxdd_detect_type(vdi->fd, &disk_type) < 0)
		goto exit_err;
	if(vboxdd_open(vdi->disk, disk_type, source,
			(flags&MS_RDONLY)? VD_OPEN_FLAGS_READONLY:VD_OPEN_FLAGS_NORMAL, NULL) < 0) {
		vudev_perror("opening vbox image failed");
		goto exit_err;
	}
	vdi->size = vboxdd_get_size(vdi->disk, 0);
	vudev->stat.st_mode = (vudev->stat.st_mode & ~S_IFMT) | S_IFBLK;
	vudev->private_data = vdi;
	return 0;
exit_err:
	dlunload_VBoxDD();
	free(vdi);
	return -1;
}

int vuvdi_fini(void *private_data) {
  struct vuvdi_t *vdi = private_data;
	if(vdi) {
		vboxdd_close(vdi->disk);
		dlunload_VBoxDD();
		free(vdi);
	}
	private_data = NULL;
	return 0;
}

struct vudev_operations_t vudev_ops = {
  //.open = vuvdi_open,
  //.close = vuvdi_close,
  //.read = vuvdi_read,
  //.write = vuvdi_write,
	//.pread64 = vuvdi_pread64,
	//.pwrite64 = vuvdi_pwrite64,
  //.lseek = vuvdi_lseek,
  //.ioctl = vuvdi_ioctl,
  //.ioctl_parms = vuvdi_ioctl_parms,
	.epoll_ctl = epoll_ctl,
  .init = vuvdi_init,
  .fini = vuvdi_fini,
};
