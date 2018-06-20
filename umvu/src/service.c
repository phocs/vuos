/*
 *   VUOS: view OS project
 *   Copyright (C) 2017  Renzo Davoli <renzo@cs.unibo.it>, Antonio Cardace <anto.cardace@gmail.com>
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

#include <service.h>
#include <umvu_peekpoke.h>
#include <vu_fs.h>

static __thread struct vuht_entry_t *thread_private_ht_for_modules;

struct vuht_entry_t *vu_mod_setht(struct vuht_entry_t *ht) {
	struct vuht_entry_t *oldht = thread_private_ht_for_modules;
	thread_private_ht_for_modules = ht;
	return oldht;
}

struct vuht_entry_t *vu_mod_getht(void) {
	return thread_private_ht_for_modules;
}

unsigned int vu_mod_gettid() {
  return umvu_gettid();
}

mode_t vu_mod_getumask(void) {
	return vu_fs_get_umask();
}
