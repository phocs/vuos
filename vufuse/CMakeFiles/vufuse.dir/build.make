# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.7

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


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

# Include any dependencies generated for this target.
include vufuse/CMakeFiles/vufuse.dir/depend.make

# Include the progress variables for this target.
include vufuse/CMakeFiles/vufuse.dir/progress.make

# Include the compile flags for this target's objects.
include vufuse/CMakeFiles/vufuse.dir/flags.make

vufuse/CMakeFiles/vufuse.dir/fuse_opt.c.o: vufuse/CMakeFiles/vufuse.dir/flags.make
vufuse/CMakeFiles/vufuse.dir/fuse_opt.c.o: vufuse/fuse_opt.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/alessio/Desktop/vuos/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object vufuse/CMakeFiles/vufuse.dir/fuse_opt.c.o"
	cd /home/alessio/Desktop/vuos/vufuse && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/vufuse.dir/fuse_opt.c.o   -c /home/alessio/Desktop/vuos/vufuse/fuse_opt.c

vufuse/CMakeFiles/vufuse.dir/fuse_opt.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/vufuse.dir/fuse_opt.c.i"
	cd /home/alessio/Desktop/vuos/vufuse && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/alessio/Desktop/vuos/vufuse/fuse_opt.c > CMakeFiles/vufuse.dir/fuse_opt.c.i

vufuse/CMakeFiles/vufuse.dir/fuse_opt.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/vufuse.dir/fuse_opt.c.s"
	cd /home/alessio/Desktop/vuos/vufuse && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/alessio/Desktop/vuos/vufuse/fuse_opt.c -o CMakeFiles/vufuse.dir/fuse_opt.c.s

vufuse/CMakeFiles/vufuse.dir/fuse_opt.c.o.requires:

.PHONY : vufuse/CMakeFiles/vufuse.dir/fuse_opt.c.o.requires

vufuse/CMakeFiles/vufuse.dir/fuse_opt.c.o.provides: vufuse/CMakeFiles/vufuse.dir/fuse_opt.c.o.requires
	$(MAKE) -f vufuse/CMakeFiles/vufuse.dir/build.make vufuse/CMakeFiles/vufuse.dir/fuse_opt.c.o.provides.build
.PHONY : vufuse/CMakeFiles/vufuse.dir/fuse_opt.c.o.provides

vufuse/CMakeFiles/vufuse.dir/fuse_opt.c.o.provides.build: vufuse/CMakeFiles/vufuse.dir/fuse_opt.c.o


vufuse/CMakeFiles/vufuse.dir/vufuse.c.o: vufuse/CMakeFiles/vufuse.dir/flags.make
vufuse/CMakeFiles/vufuse.dir/vufuse.c.o: vufuse/vufuse.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/alessio/Desktop/vuos/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object vufuse/CMakeFiles/vufuse.dir/vufuse.c.o"
	cd /home/alessio/Desktop/vuos/vufuse && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/vufuse.dir/vufuse.c.o   -c /home/alessio/Desktop/vuos/vufuse/vufuse.c

vufuse/CMakeFiles/vufuse.dir/vufuse.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/vufuse.dir/vufuse.c.i"
	cd /home/alessio/Desktop/vuos/vufuse && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/alessio/Desktop/vuos/vufuse/vufuse.c > CMakeFiles/vufuse.dir/vufuse.c.i

vufuse/CMakeFiles/vufuse.dir/vufuse.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/vufuse.dir/vufuse.c.s"
	cd /home/alessio/Desktop/vuos/vufuse && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/alessio/Desktop/vuos/vufuse/vufuse.c -o CMakeFiles/vufuse.dir/vufuse.c.s

vufuse/CMakeFiles/vufuse.dir/vufuse.c.o.requires:

.PHONY : vufuse/CMakeFiles/vufuse.dir/vufuse.c.o.requires

vufuse/CMakeFiles/vufuse.dir/vufuse.c.o.provides: vufuse/CMakeFiles/vufuse.dir/vufuse.c.o.requires
	$(MAKE) -f vufuse/CMakeFiles/vufuse.dir/build.make vufuse/CMakeFiles/vufuse.dir/vufuse.c.o.provides.build
.PHONY : vufuse/CMakeFiles/vufuse.dir/vufuse.c.o.provides

vufuse/CMakeFiles/vufuse.dir/vufuse.c.o.provides.build: vufuse/CMakeFiles/vufuse.dir/vufuse.c.o


vufuse/CMakeFiles/vufuse.dir/vufuse_node.c.o: vufuse/CMakeFiles/vufuse.dir/flags.make
vufuse/CMakeFiles/vufuse.dir/vufuse_node.c.o: vufuse/vufuse_node.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/alessio/Desktop/vuos/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object vufuse/CMakeFiles/vufuse.dir/vufuse_node.c.o"
	cd /home/alessio/Desktop/vuos/vufuse && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/vufuse.dir/vufuse_node.c.o   -c /home/alessio/Desktop/vuos/vufuse/vufuse_node.c

vufuse/CMakeFiles/vufuse.dir/vufuse_node.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/vufuse.dir/vufuse_node.c.i"
	cd /home/alessio/Desktop/vuos/vufuse && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/alessio/Desktop/vuos/vufuse/vufuse_node.c > CMakeFiles/vufuse.dir/vufuse_node.c.i

vufuse/CMakeFiles/vufuse.dir/vufuse_node.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/vufuse.dir/vufuse_node.c.s"
	cd /home/alessio/Desktop/vuos/vufuse && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/alessio/Desktop/vuos/vufuse/vufuse_node.c -o CMakeFiles/vufuse.dir/vufuse_node.c.s

vufuse/CMakeFiles/vufuse.dir/vufuse_node.c.o.requires:

.PHONY : vufuse/CMakeFiles/vufuse.dir/vufuse_node.c.o.requires

vufuse/CMakeFiles/vufuse.dir/vufuse_node.c.o.provides: vufuse/CMakeFiles/vufuse.dir/vufuse_node.c.o.requires
	$(MAKE) -f vufuse/CMakeFiles/vufuse.dir/build.make vufuse/CMakeFiles/vufuse.dir/vufuse_node.c.o.provides.build
.PHONY : vufuse/CMakeFiles/vufuse.dir/vufuse_node.c.o.provides

vufuse/CMakeFiles/vufuse.dir/vufuse_node.c.o.provides.build: vufuse/CMakeFiles/vufuse.dir/vufuse_node.c.o


vufuse/CMakeFiles/vufuse.dir/vufuseargs.c.o: vufuse/CMakeFiles/vufuse.dir/flags.make
vufuse/CMakeFiles/vufuse.dir/vufuseargs.c.o: vufuse/vufuseargs.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/alessio/Desktop/vuos/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object vufuse/CMakeFiles/vufuse.dir/vufuseargs.c.o"
	cd /home/alessio/Desktop/vuos/vufuse && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/vufuse.dir/vufuseargs.c.o   -c /home/alessio/Desktop/vuos/vufuse/vufuseargs.c

vufuse/CMakeFiles/vufuse.dir/vufuseargs.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/vufuse.dir/vufuseargs.c.i"
	cd /home/alessio/Desktop/vuos/vufuse && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/alessio/Desktop/vuos/vufuse/vufuseargs.c > CMakeFiles/vufuse.dir/vufuseargs.c.i

vufuse/CMakeFiles/vufuse.dir/vufuseargs.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/vufuse.dir/vufuseargs.c.s"
	cd /home/alessio/Desktop/vuos/vufuse && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/alessio/Desktop/vuos/vufuse/vufuseargs.c -o CMakeFiles/vufuse.dir/vufuseargs.c.s

vufuse/CMakeFiles/vufuse.dir/vufuseargs.c.o.requires:

.PHONY : vufuse/CMakeFiles/vufuse.dir/vufuseargs.c.o.requires

vufuse/CMakeFiles/vufuse.dir/vufuseargs.c.o.provides: vufuse/CMakeFiles/vufuse.dir/vufuseargs.c.o.requires
	$(MAKE) -f vufuse/CMakeFiles/vufuse.dir/build.make vufuse/CMakeFiles/vufuse.dir/vufuseargs.c.o.provides.build
.PHONY : vufuse/CMakeFiles/vufuse.dir/vufuseargs.c.o.provides

vufuse/CMakeFiles/vufuse.dir/vufuseargs.c.o.provides.build: vufuse/CMakeFiles/vufuse.dir/vufuseargs.c.o


vufuse/CMakeFiles/vufuse.dir/vufusedents.c.o: vufuse/CMakeFiles/vufuse.dir/flags.make
vufuse/CMakeFiles/vufuse.dir/vufusedents.c.o: vufuse/vufusedents.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/alessio/Desktop/vuos/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building C object vufuse/CMakeFiles/vufuse.dir/vufusedents.c.o"
	cd /home/alessio/Desktop/vuos/vufuse && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/vufuse.dir/vufusedents.c.o   -c /home/alessio/Desktop/vuos/vufuse/vufusedents.c

vufuse/CMakeFiles/vufuse.dir/vufusedents.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/vufuse.dir/vufusedents.c.i"
	cd /home/alessio/Desktop/vuos/vufuse && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/alessio/Desktop/vuos/vufuse/vufusedents.c > CMakeFiles/vufuse.dir/vufusedents.c.i

vufuse/CMakeFiles/vufuse.dir/vufusedents.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/vufuse.dir/vufusedents.c.s"
	cd /home/alessio/Desktop/vuos/vufuse && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/alessio/Desktop/vuos/vufuse/vufusedents.c -o CMakeFiles/vufuse.dir/vufusedents.c.s

vufuse/CMakeFiles/vufuse.dir/vufusedents.c.o.requires:

.PHONY : vufuse/CMakeFiles/vufuse.dir/vufusedents.c.o.requires

vufuse/CMakeFiles/vufuse.dir/vufusedents.c.o.provides: vufuse/CMakeFiles/vufuse.dir/vufusedents.c.o.requires
	$(MAKE) -f vufuse/CMakeFiles/vufuse.dir/build.make vufuse/CMakeFiles/vufuse.dir/vufusedents.c.o.provides.build
.PHONY : vufuse/CMakeFiles/vufuse.dir/vufusedents.c.o.provides

vufuse/CMakeFiles/vufuse.dir/vufusedents.c.o.provides.build: vufuse/CMakeFiles/vufuse.dir/vufusedents.c.o


vufuse/CMakeFiles/vufuse.dir/vufuseop.c.o: vufuse/CMakeFiles/vufuse.dir/flags.make
vufuse/CMakeFiles/vufuse.dir/vufuseop.c.o: vufuse/vufuseop.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/alessio/Desktop/vuos/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building C object vufuse/CMakeFiles/vufuse.dir/vufuseop.c.o"
	cd /home/alessio/Desktop/vuos/vufuse && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/vufuse.dir/vufuseop.c.o   -c /home/alessio/Desktop/vuos/vufuse/vufuseop.c

vufuse/CMakeFiles/vufuse.dir/vufuseop.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/vufuse.dir/vufuseop.c.i"
	cd /home/alessio/Desktop/vuos/vufuse && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/alessio/Desktop/vuos/vufuse/vufuseop.c > CMakeFiles/vufuse.dir/vufuseop.c.i

vufuse/CMakeFiles/vufuse.dir/vufuseop.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/vufuse.dir/vufuseop.c.s"
	cd /home/alessio/Desktop/vuos/vufuse && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/alessio/Desktop/vuos/vufuse/vufuseop.c -o CMakeFiles/vufuse.dir/vufuseop.c.s

vufuse/CMakeFiles/vufuse.dir/vufuseop.c.o.requires:

.PHONY : vufuse/CMakeFiles/vufuse.dir/vufuseop.c.o.requires

vufuse/CMakeFiles/vufuse.dir/vufuseop.c.o.provides: vufuse/CMakeFiles/vufuse.dir/vufuseop.c.o.requires
	$(MAKE) -f vufuse/CMakeFiles/vufuse.dir/build.make vufuse/CMakeFiles/vufuse.dir/vufuseop.c.o.provides.build
.PHONY : vufuse/CMakeFiles/vufuse.dir/vufuseop.c.o.provides

vufuse/CMakeFiles/vufuse.dir/vufuseop.c.o.provides.build: vufuse/CMakeFiles/vufuse.dir/vufuseop.c.o


vufuse/CMakeFiles/vufuse.dir/vufusestd.c.o: vufuse/CMakeFiles/vufuse.dir/flags.make
vufuse/CMakeFiles/vufuse.dir/vufusestd.c.o: vufuse/vufusestd.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/alessio/Desktop/vuos/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building C object vufuse/CMakeFiles/vufuse.dir/vufusestd.c.o"
	cd /home/alessio/Desktop/vuos/vufuse && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/vufuse.dir/vufusestd.c.o   -c /home/alessio/Desktop/vuos/vufuse/vufusestd.c

vufuse/CMakeFiles/vufuse.dir/vufusestd.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/vufuse.dir/vufusestd.c.i"
	cd /home/alessio/Desktop/vuos/vufuse && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/alessio/Desktop/vuos/vufuse/vufusestd.c > CMakeFiles/vufuse.dir/vufusestd.c.i

vufuse/CMakeFiles/vufuse.dir/vufusestd.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/vufuse.dir/vufusestd.c.s"
	cd /home/alessio/Desktop/vuos/vufuse && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/alessio/Desktop/vuos/vufuse/vufusestd.c -o CMakeFiles/vufuse.dir/vufusestd.c.s

vufuse/CMakeFiles/vufuse.dir/vufusestd.c.o.requires:

.PHONY : vufuse/CMakeFiles/vufuse.dir/vufusestd.c.o.requires

vufuse/CMakeFiles/vufuse.dir/vufusestd.c.o.provides: vufuse/CMakeFiles/vufuse.dir/vufusestd.c.o.requires
	$(MAKE) -f vufuse/CMakeFiles/vufuse.dir/build.make vufuse/CMakeFiles/vufuse.dir/vufusestd.c.o.provides.build
.PHONY : vufuse/CMakeFiles/vufuse.dir/vufusestd.c.o.provides

vufuse/CMakeFiles/vufuse.dir/vufusestd.c.o.provides.build: vufuse/CMakeFiles/vufuse.dir/vufusestd.c.o


# Object files for target vufuse
vufuse_OBJECTS = \
"CMakeFiles/vufuse.dir/fuse_opt.c.o" \
"CMakeFiles/vufuse.dir/vufuse.c.o" \
"CMakeFiles/vufuse.dir/vufuse_node.c.o" \
"CMakeFiles/vufuse.dir/vufuseargs.c.o" \
"CMakeFiles/vufuse.dir/vufusedents.c.o" \
"CMakeFiles/vufuse.dir/vufuseop.c.o" \
"CMakeFiles/vufuse.dir/vufusestd.c.o"

# External object files for target vufuse
vufuse_EXTERNAL_OBJECTS =

lib/vufuse.so: vufuse/CMakeFiles/vufuse.dir/fuse_opt.c.o
lib/vufuse.so: vufuse/CMakeFiles/vufuse.dir/vufuse.c.o
lib/vufuse.so: vufuse/CMakeFiles/vufuse.dir/vufuse_node.c.o
lib/vufuse.so: vufuse/CMakeFiles/vufuse.dir/vufuseargs.c.o
lib/vufuse.so: vufuse/CMakeFiles/vufuse.dir/vufusedents.c.o
lib/vufuse.so: vufuse/CMakeFiles/vufuse.dir/vufuseop.c.o
lib/vufuse.so: vufuse/CMakeFiles/vufuse.dir/vufusestd.c.o
lib/vufuse.so: vufuse/CMakeFiles/vufuse.dir/build.make
lib/vufuse.so: vufuse/CMakeFiles/vufuse.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/alessio/Desktop/vuos/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Linking C shared library ../lib/vufuse.so"
	cd /home/alessio/Desktop/vuos/vufuse && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/vufuse.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
vufuse/CMakeFiles/vufuse.dir/build: lib/vufuse.so

.PHONY : vufuse/CMakeFiles/vufuse.dir/build

vufuse/CMakeFiles/vufuse.dir/requires: vufuse/CMakeFiles/vufuse.dir/fuse_opt.c.o.requires
vufuse/CMakeFiles/vufuse.dir/requires: vufuse/CMakeFiles/vufuse.dir/vufuse.c.o.requires
vufuse/CMakeFiles/vufuse.dir/requires: vufuse/CMakeFiles/vufuse.dir/vufuse_node.c.o.requires
vufuse/CMakeFiles/vufuse.dir/requires: vufuse/CMakeFiles/vufuse.dir/vufuseargs.c.o.requires
vufuse/CMakeFiles/vufuse.dir/requires: vufuse/CMakeFiles/vufuse.dir/vufusedents.c.o.requires
vufuse/CMakeFiles/vufuse.dir/requires: vufuse/CMakeFiles/vufuse.dir/vufuseop.c.o.requires
vufuse/CMakeFiles/vufuse.dir/requires: vufuse/CMakeFiles/vufuse.dir/vufusestd.c.o.requires

.PHONY : vufuse/CMakeFiles/vufuse.dir/requires

vufuse/CMakeFiles/vufuse.dir/clean:
	cd /home/alessio/Desktop/vuos/vufuse && $(CMAKE_COMMAND) -P CMakeFiles/vufuse.dir/cmake_clean.cmake
.PHONY : vufuse/CMakeFiles/vufuse.dir/clean

vufuse/CMakeFiles/vufuse.dir/depend:
	cd /home/alessio/Desktop/vuos && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/alessio/Desktop/vuos /home/alessio/Desktop/vuos/vufuse /home/alessio/Desktop/vuos /home/alessio/Desktop/vuos/vufuse /home/alessio/Desktop/vuos/vufuse/CMakeFiles/vufuse.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : vufuse/CMakeFiles/vufuse.dir/depend

