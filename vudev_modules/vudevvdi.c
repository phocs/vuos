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
  off_t size;
  void *disk;
  unsigned long flags;
}

struct vuvdifd_t {
  off_t offset;
}

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
		printderror("cannot autodetect disk type");
		errno = ENODEV; return -1;
	}
	printd("disktype [%s]\n", *disktype);
	return 0;
}

static int dlload_VBoxDD() {
	if(VBoxDD_count)
		return ++VBoxDD_count;
	if((VBoxDD_hdl = vu_mod_dlopen("/usr/lib/virtualbox/VBoxDD.so", RTLD_LAZY)) == NULL) {
		printderror("VBoxDD library: not found");
		return -1;
	}
	vboxdd_create = dlsym(VBoxDD_hdl, "VDCreate");
	vboxdd_open = dlsym(VBoxDD_hdl, "VDOpen");
	vboxdd_close = dlsym(VBoxDD_hdl, "VDClose");
	vboxdd_read = dlsym(VBoxDD_hdl, "VDRead");
	vboxdd_write = dlsym(VBoxDD_hdl, "VDWrite");
	vboxdd_flush = dlsym(VBoxDD_hdl, "VDFlush");
	vboxdd_get_size = dlsym(VBoxDD_hdl, "VDGetSize");
	vboxdd_get_LCHS_geometry = dlsym(VBoxDD_hdl, "VDGetLCHSGeometry");
	printd("%p %p %p %p %p %p %p %p\n",
		vboxdd_create, vboxdd_open, vboxdd_close, vboxdd_read,
		vboxdd_write, vboxdd_flush, vboxdd_get_size, vboxdd_get_LCHS_geometry);
	return ++VBoxDD_count;
}

static void dlunload_VBoxDD() {
	vboxdd_count--;
	if (vboxdd_count == 0) {
		dlclose(VBoxDD_hdl);
		VBoxDD_hdl=NULL;
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
  struct vuvdifd_t *vdifd = vudev_get_fdprivate();
  free(vdifd);
  return 0;
}

/*
long vuvdi_ioctl_parms(unsigned long request) {
  switch (request) {
    case HDIO_GETGEO: return _IOW('D', 0, struct hd_geometry);
    default: return 0;
	}
}
*/

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
		printderror("invalid initialisation of VD interface");
		goto exit_err;
	}
	if(vboxdd_detect_type(vdi->fd, &disk_type) < 0)
		goto err_exit;
	if(vboxdd_open(vdi->disk, disk_type, source,
			(flags&MS_RDONLY)? VD_OPEN_FLAGS_READONLY:VD_OPEN_FLAGS_NORMAL, NULL) < 0) {
		printderror("opening vbox image failed");
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
