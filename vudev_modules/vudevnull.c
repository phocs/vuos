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

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <vudev.h>
#include <vumodule.h>

static int null_open(const char *pathname, int flags, mode_t mode) {
	printd("null_open [%d]", pathname);
	return 0;
}

static int null_close(int fd) {
	printd("null_close [%d]", fd);
	return 0;
}

static ssize_t null_read(int fd, void *buf, size_t count) {
	printd("null_read: [%d]", count);
	return 0;
}

static ssize_t null_write(int fd, void *buf, size_t count) {
	printd("null_write: [%d]", count);
	return count;
}

struct vudev_operations_t vudev_ops = {
	.open = null_open,
	.close = null_close,
	.read = null_read,
	.write= null_write,
};
