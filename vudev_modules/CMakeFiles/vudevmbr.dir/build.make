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
include vudev_modules/CMakeFiles/vudevmbr.dir/depend.make

# Include the progress variables for this target.
include vudev_modules/CMakeFiles/vudevmbr.dir/progress.make

# Include the compile flags for this target's objects.
include vudev_modules/CMakeFiles/vudevmbr.dir/flags.make

vudev_modules/CMakeFiles/vudevmbr.dir/vudevmbr.c.o: vudev_modules/CMakeFiles/vudevmbr.dir/flags.make
vudev_modules/CMakeFiles/vudevmbr.dir/vudevmbr.c.o: vudev_modules/vudevmbr.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/alessio/Desktop/vuos/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object vudev_modules/CMakeFiles/vudevmbr.dir/vudevmbr.c.o"
	cd /home/alessio/Desktop/vuos/vudev_modules && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/vudevmbr.dir/vudevmbr.c.o   -c /home/alessio/Desktop/vuos/vudev_modules/vudevmbr.c

vudev_modules/CMakeFiles/vudevmbr.dir/vudevmbr.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/vudevmbr.dir/vudevmbr.c.i"
	cd /home/alessio/Desktop/vuos/vudev_modules && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/alessio/Desktop/vuos/vudev_modules/vudevmbr.c > CMakeFiles/vudevmbr.dir/vudevmbr.c.i

vudev_modules/CMakeFiles/vudevmbr.dir/vudevmbr.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/vudevmbr.dir/vudevmbr.c.s"
	cd /home/alessio/Desktop/vuos/vudev_modules && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/alessio/Desktop/vuos/vudev_modules/vudevmbr.c -o CMakeFiles/vudevmbr.dir/vudevmbr.c.s

vudev_modules/CMakeFiles/vudevmbr.dir/vudevmbr.c.o.requires:

.PHONY : vudev_modules/CMakeFiles/vudevmbr.dir/vudevmbr.c.o.requires

vudev_modules/CMakeFiles/vudevmbr.dir/vudevmbr.c.o.provides: vudev_modules/CMakeFiles/vudevmbr.dir/vudevmbr.c.o.requires
	$(MAKE) -f vudev_modules/CMakeFiles/vudevmbr.dir/build.make vudev_modules/CMakeFiles/vudevmbr.dir/vudevmbr.c.o.provides.build
.PHONY : vudev_modules/CMakeFiles/vudevmbr.dir/vudevmbr.c.o.provides

vudev_modules/CMakeFiles/vudevmbr.dir/vudevmbr.c.o.provides.build: vudev_modules/CMakeFiles/vudevmbr.dir/vudevmbr.c.o


# Object files for target vudevmbr
vudevmbr_OBJECTS = \
"CMakeFiles/vudevmbr.dir/vudevmbr.c.o"

# External object files for target vudevmbr
vudevmbr_EXTERNAL_OBJECTS =

lib/vudevmbr.so: vudev_modules/CMakeFiles/vudevmbr.dir/vudevmbr.c.o
lib/vudevmbr.so: vudev_modules/CMakeFiles/vudevmbr.dir/build.make
lib/vudevmbr.so: vudev_modules/CMakeFiles/vudevmbr.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/alessio/Desktop/vuos/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C shared library ../lib/vudevmbr.so"
	cd /home/alessio/Desktop/vuos/vudev_modules && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/vudevmbr.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
vudev_modules/CMakeFiles/vudevmbr.dir/build: lib/vudevmbr.so

.PHONY : vudev_modules/CMakeFiles/vudevmbr.dir/build

vudev_modules/CMakeFiles/vudevmbr.dir/requires: vudev_modules/CMakeFiles/vudevmbr.dir/vudevmbr.c.o.requires

.PHONY : vudev_modules/CMakeFiles/vudevmbr.dir/requires

vudev_modules/CMakeFiles/vudevmbr.dir/clean:
	cd /home/alessio/Desktop/vuos/vudev_modules && $(CMAKE_COMMAND) -P CMakeFiles/vudevmbr.dir/cmake_clean.cmake
.PHONY : vudev_modules/CMakeFiles/vudevmbr.dir/clean

vudev_modules/CMakeFiles/vudevmbr.dir/depend:
	cd /home/alessio/Desktop/vuos && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/alessio/Desktop/vuos /home/alessio/Desktop/vuos/vudev_modules /home/alessio/Desktop/vuos /home/alessio/Desktop/vuos/vudev_modules /home/alessio/Desktop/vuos/vudev_modules/CMakeFiles/vudevmbr.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : vudev_modules/CMakeFiles/vudevmbr.dir/depend

