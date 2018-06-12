# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.7

# Default target executed when no arguments are given to make.
default_target: all

.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/alessio/Desktop/vuos

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/alessio/Desktop/vuos

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target install/strip
install/strip: preinstall
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Installing the project stripped..."
	/usr/bin/cmake -DCMAKE_INSTALL_DO_STRIP=1 -P cmake_install.cmake
.PHONY : install/strip

# Special rule for the target install/strip
install/strip/fast: install/strip

.PHONY : install/strip/fast

# Special rule for the target install/local
install/local: preinstall
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Installing only the local directory..."
	/usr/bin/cmake -DCMAKE_INSTALL_LOCAL_ONLY=1 -P cmake_install.cmake
.PHONY : install/local

# Special rule for the target install/local
install/local/fast: install/local

.PHONY : install/local/fast

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "No interactive CMake dialog available..."
	/usr/bin/cmake -E echo No\ interactive\ CMake\ dialog\ available.
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache

.PHONY : edit_cache/fast

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/usr/bin/cmake -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache

.PHONY : rebuild_cache/fast

# Special rule for the target list_install_components
list_install_components:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Available install components are: \"Unspecified\""
.PHONY : list_install_components

# Special rule for the target list_install_components
list_install_components/fast: list_install_components

.PHONY : list_install_components/fast

# Special rule for the target install
install: preinstall
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Install the project..."
	/usr/bin/cmake -P cmake_install.cmake
.PHONY : install

# Special rule for the target install
install/fast: preinstall/fast
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Install the project..."
	/usr/bin/cmake -P cmake_install.cmake
.PHONY : install/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /home/alessio/Desktop/vuos/CMakeFiles /home/alessio/Desktop/vuos/CMakeFiles/progress.marks
	$(MAKE) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /home/alessio/Desktop/vuos/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean

.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named Dynamic_Sources

# Build rule for target.
Dynamic_Sources: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 Dynamic_Sources
.PHONY : Dynamic_Sources

# fast build rule for target.
Dynamic_Sources/fast:
	$(MAKE) -f scripts/CMakeFiles/Dynamic_Sources.dir/build.make scripts/CMakeFiles/Dynamic_Sources.dir/build
.PHONY : Dynamic_Sources/fast

#=============================================================================
# Target rules for targets named umvu

# Build rule for target.
umvu: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 umvu
.PHONY : umvu

# fast build rule for target.
umvu/fast:
	$(MAKE) -f umvu/CMakeFiles/umvu.dir/build.make umvu/CMakeFiles/umvu.dir/build
.PHONY : umvu/fast

#=============================================================================
# Target rules for targets named vudebug

# Build rule for target.
vudebug: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 vudebug
.PHONY : vudebug

# fast build rule for target.
vudebug/fast:
	$(MAKE) -f cmd/CMakeFiles/vudebug.dir/build.make cmd/CMakeFiles/vudebug.dir/build
.PHONY : vudebug/fast

#=============================================================================
# Target rules for targets named vu_rmmod

# Build rule for target.
vu_rmmod: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 vu_rmmod
.PHONY : vu_rmmod

# fast build rule for target.
vu_rmmod/fast:
	$(MAKE) -f cmd/CMakeFiles/vu_rmmod.dir/build.make cmd/CMakeFiles/vu_rmmod.dir/build
.PHONY : vu_rmmod/fast

#=============================================================================
# Target rules for targets named vu_lsmod

# Build rule for target.
vu_lsmod: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 vu_lsmod
.PHONY : vu_lsmod

# fast build rule for target.
vu_lsmod/fast:
	$(MAKE) -f cmd/CMakeFiles/vu_lsmod.dir/build.make cmd/CMakeFiles/vu_lsmod.dir/build
.PHONY : vu_lsmod/fast

#=============================================================================
# Target rules for targets named vu_insmod

# Build rule for target.
vu_insmod: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 vu_insmod
.PHONY : vu_insmod

# fast build rule for target.
vu_insmod/fast:
	$(MAKE) -f cmd/CMakeFiles/vu_insmod.dir/build.make cmd/CMakeFiles/vu_insmod.dir/build
.PHONY : vu_insmod/fast

#=============================================================================
# Target rules for targets named vuname

# Build rule for target.
vuname: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 vuname
.PHONY : vuname

# fast build rule for target.
vuname/fast:
	$(MAKE) -f cmd/CMakeFiles/vuname.dir/build.make cmd/CMakeFiles/vuname.dir/build
.PHONY : vuname/fast

#=============================================================================
# Target rules for targets named vustack

# Build rule for target.
vustack: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 vustack
.PHONY : vustack

# fast build rule for target.
vustack/fast:
	$(MAKE) -f cmd/CMakeFiles/vustack.dir/build.make cmd/CMakeFiles/vustack.dir/build
.PHONY : vustack/fast

#=============================================================================
# Target rules for targets named vusu

# Build rule for target.
vusu: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 vusu
.PHONY : vusu

# fast build rule for target.
vusu/fast:
	$(MAKE) -f cmd/CMakeFiles/vusu.dir/build.make cmd/CMakeFiles/vusu.dir/build
.PHONY : vusu/fast

#=============================================================================
# Target rules for targets named unrealinfofs

# Build rule for target.
unrealinfofs: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 unrealinfofs
.PHONY : unrealinfofs

# fast build rule for target.
unrealinfofs/fast:
	$(MAKE) -f test_modules/CMakeFiles/unrealinfofs.dir/build.make test_modules/CMakeFiles/unrealinfofs.dir/build
.PHONY : unrealinfofs/fast

#=============================================================================
# Target rules for targets named unreal

# Build rule for target.
unreal: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 unreal
.PHONY : unreal

# fast build rule for target.
unreal/fast:
	$(MAKE) -f test_modules/CMakeFiles/unreal.dir/build.make test_modules/CMakeFiles/unreal.dir/build
.PHONY : unreal/fast

#=============================================================================
# Target rules for targets named unrealsock

# Build rule for target.
unrealsock: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 unrealsock
.PHONY : unrealsock

# fast build rule for target.
unrealsock/fast:
	$(MAKE) -f test_modules/CMakeFiles/unrealsock.dir/build.make test_modules/CMakeFiles/unrealsock.dir/build
.PHONY : unrealsock/fast

#=============================================================================
# Target rules for targets named mountreal

# Build rule for target.
mountreal: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 mountreal
.PHONY : mountreal

# fast build rule for target.
mountreal/fast:
	$(MAKE) -f test_modules/CMakeFiles/mountreal.dir/build.make test_modules/CMakeFiles/mountreal.dir/build
.PHONY : mountreal/fast

#=============================================================================
# Target rules for targets named unrealuidgid

# Build rule for target.
unrealuidgid: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 unrealuidgid
.PHONY : unrealuidgid

# fast build rule for target.
unrealuidgid/fast:
	$(MAKE) -f test_modules/CMakeFiles/unrealuidgid.dir/build.make test_modules/CMakeFiles/unrealuidgid.dir/build
.PHONY : unrealuidgid/fast

#=============================================================================
# Target rules for targets named vumod

# Build rule for target.
vumod: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 vumod
.PHONY : vumod

# fast build rule for target.
vumod/fast:
	$(MAKE) -f libvumod/CMakeFiles/vumod.dir/build.make libvumod/CMakeFiles/vumod.dir/build
.PHONY : vumod/fast

#=============================================================================
# Target rules for targets named vufuse

# Build rule for target.
vufuse: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 vufuse
.PHONY : vufuse

# fast build rule for target.
vufuse/fast:
	$(MAKE) -f vufuse/CMakeFiles/vufuse.dir/build.make vufuse/CMakeFiles/vufuse.dir/build
.PHONY : vufuse/fast

#=============================================================================
# Target rules for targets named vudev

# Build rule for target.
vudev: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 vudev
.PHONY : vudev

# fast build rule for target.
vudev/fast:
	$(MAKE) -f vudev/CMakeFiles/vudev.dir/build.make vudev/CMakeFiles/vudev.dir/build
.PHONY : vudev/fast

#=============================================================================
# Target rules for targets named vunet

# Build rule for target.
vunet: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 vunet
.PHONY : vunet

# fast build rule for target.
vunet/fast:
	$(MAKE) -f vunet/CMakeFiles/vunet.dir/build.make vunet/CMakeFiles/vunet.dir/build
.PHONY : vunet/fast

#=============================================================================
# Target rules for targets named vufs

# Build rule for target.
vufs: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 vufs
.PHONY : vufs

# fast build rule for target.
vufs/fast:
	$(MAKE) -f vufs/CMakeFiles/vufs.dir/build.make vufs/CMakeFiles/vufs.dir/build
.PHONY : vufs/fast

#=============================================================================
# Target rules for targets named vubinfmt

# Build rule for target.
vubinfmt: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 vubinfmt
.PHONY : vubinfmt

# fast build rule for target.
vubinfmt/fast:
	$(MAKE) -f vubinfmt/CMakeFiles/vubinfmt.dir/build.make vubinfmt/CMakeFiles/vubinfmt.dir/build
.PHONY : vubinfmt/fast

#=============================================================================
# Target rules for targets named vunetnull

# Build rule for target.
vunetnull: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 vunetnull
.PHONY : vunetnull

# fast build rule for target.
vunetnull/fast:
	$(MAKE) -f vunet_modules/CMakeFiles/vunetnull.dir/build.make vunet_modules/CMakeFiles/vunetnull.dir/build
.PHONY : vunetnull/fast

#=============================================================================
# Target rules for targets named vunetreal

# Build rule for target.
vunetreal: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 vunetreal
.PHONY : vunetreal

# fast build rule for target.
vunetreal/fast:
	$(MAKE) -f vunet_modules/CMakeFiles/vunetreal.dir/build.make vunet_modules/CMakeFiles/vunetreal.dir/build
.PHONY : vunetreal/fast

#=============================================================================
# Target rules for targets named vunetvdestack

# Build rule for target.
vunetvdestack: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 vunetvdestack
.PHONY : vunetvdestack

# fast build rule for target.
vunetvdestack/fast:
	$(MAKE) -f vunet_modules/CMakeFiles/vunetvdestack.dir/build.make vunet_modules/CMakeFiles/vunetvdestack.dir/build
.PHONY : vunetvdestack/fast

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... install/strip"
	@echo "... install/local"
	@echo "... edit_cache"
	@echo "... rebuild_cache"
	@echo "... list_install_components"
	@echo "... install"
	@echo "... Dynamic_Sources"
	@echo "... umvu"
	@echo "... vudebug"
	@echo "... vu_rmmod"
	@echo "... vu_lsmod"
	@echo "... vu_insmod"
	@echo "... vuname"
	@echo "... vustack"
	@echo "... vusu"
	@echo "... unrealinfofs"
	@echo "... unreal"
	@echo "... unrealsock"
	@echo "... mountreal"
	@echo "... unrealuidgid"
	@echo "... vumod"
	@echo "... vufuse"
	@echo "... vudev"
	@echo "... vunet"
	@echo "... vufs"
	@echo "... vubinfmt"
	@echo "... vunetnull"
	@echo "... vunetreal"
	@echo "... vunetvdestack"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

