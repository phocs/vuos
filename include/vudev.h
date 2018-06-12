#ifndef VUDEV_H
#define VUDEV_H

#include <errno.h>
#include <unistd.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/epoll.h>

#include <vumodule.h>

struct vudev_t {
  char *target;
  void *dlhandle;
  void *private_data;
	unsigned int nsubdev;
  unsigned int targetlen;
  struct vu_stat stat;
  struct vuht_entry_t *path_ht;
  struct vuht_entry_t *mjmi_ht;
  struct vuht_entry_t *ioctl_ht;
	struct vudev_operations_t *devops;
};

struct vudevfd_t {
  dev_t devno;
  int open_flags;
  void *fdprivate;
	struct vudev_t *vudev;
};

struct vudev_operations_t {
  int (*open) (const char *pathname, int flags, mode_t mode);
  int (*close) (int fd);
  ssize_t (*read) (int fd, void *buf, size_t count);
  ssize_t (*write) (int fd, const void *buf, size_t count);
  ssize_t (*pread64) (int fd, void *buf, size_t count, off_t offset);
  ssize_t (*pwrite64) (int fd, const void *buf, size_t count, off_t offset);
  off_t (*lseek) (int fd, off_t offset, int whence);
  int (*ioctl) (int fd,  unsigned long request, void *addr);
  unsigned long (*ioctl_parms) (unsigned long request);
  int (*epoll_ctl) (int epfd, int op, int fd, struct epoll_event *event);
  int (*init) (const char *source, unsigned long flags, const char *args, struct vudev_t *vudev);
  int (*fini) (void *private_data);
};

#define printd(fmt, ...) printkdebug(D, "%s " fmt, __func__, ##__VA_ARGS__)
#define printderror(str) if(errno) printd("error [%s]:[%s]", str, strerror(errno))

void *vudev_get_fdprivate(void);
void *vudev_get_private_data(void);
void vudev_set_fdprivate(void *fdprivate);
void vudev_set_nsubdev(unsigned int n);
dev_t vudev_get_devno(void);

static inline char *parse_mount_option(char *s, char **key, char **val) {
  for(*key = s; *s != 0 && *s != '=' && *s != ','; s++) ;
  if(*s == '=') {
    *s=0; s++;
    int quote = (*s == '\"');
    *val = quote? ++s:s;
    for(; *s != 0 && (*s != ',' || quote) && *s != '\"'; s++) ;
  } else
    *val = 0;
  *(s++) = 0;
  return s;
}

#endif
