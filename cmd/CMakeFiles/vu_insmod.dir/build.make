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
include cmd/CMakeFiles/vu_insmod.dir/depend.make

# Include the progress variables for this target.
include cmd/CMakeFiles/vu_insmod.dir/progress.make

# Include the compile flags for this target's objects.
include cmd/CMakeFiles/vu_insmod.dir/flags.make

cmd/CMakeFiles/vu_insmod.dir/vu_insmod.c.o: cmd/CMakeFiles/vu_insmod.dir/flags.make
cmd/CMakeFiles/vu_insmod.dir/vu_insmod.c.o: cmd/vu_insmod.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/alessio/Desktop/vuos/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object cmd/CMakeFiles/vu_insmod.dir/vu_insmod.c.o"
	cd /home/alessio/Desktop/vuos/cmd && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/vu_insmod.dir/vu_insmod.c.o   -c /home/alessio/Desktop/vuos/cmd/vu_insmod.c

cmd/CMakeFiles/vu_insmod.dir/vu_insmod.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/vu_insmod.dir/vu_insmod.c.i"
	cd /home/alessio/Desktop/vuos/cmd && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/alessio/Desktop/vuos/cmd/vu_insmod.c > CMakeFiles/vu_insmod.dir/vu_insmod.c.i

cmd/CMakeFiles/vu_insmod.dir/vu_insmod.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/vu_insmod.dir/vu_insmod.c.s"
	cd /home/alessio/Desktop/vuos/cmd && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/alessio/Desktop/vuos/cmd/vu_insmod.c -o CMakeFiles/vu_insmod.dir/vu_insmod.c.s

cmd/CMakeFiles/vu_insmod.dir/vu_insmod.c.o.requires:

.PHONY : cmd/CMakeFiles/vu_insmod.dir/vu_insmod.c.o.requires

cmd/CMakeFiles/vu_insmod.dir/vu_insmod.c.o.provides: cmd/CMakeFiles/vu_insmod.dir/vu_insmod.c.o.requires
	$(MAKE) -f cmd/CMakeFiles/vu_insmod.dir/build.make cmd/CMakeFiles/vu_insmod.dir/vu_insmod.c.o.provides.build
.PHONY : cmd/CMakeFiles/vu_insmod.dir/vu_insmod.c.o.provides

cmd/CMakeFiles/vu_insmod.dir/vu_insmod.c.o.provides.build: cmd/CMakeFiles/vu_insmod.dir/vu_insmod.c.o


# Object files for target vu_insmod
vu_insmod_OBJECTS = \
"CMakeFiles/vu_insmod.dir/vu_insmod.c.o"

# External object files for target vu_insmod
vu_insmod_EXTERNAL_OBJECTS =

bin/vu_insmod: cmd/CMakeFiles/vu_insmod.dir/vu_insmod.c.o
bin/vu_insmod: cmd/CMakeFiles/vu_insmod.dir/build.make
bin/vu_insmod: cmd/CMakeFiles/vu_insmod.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/alessio/Desktop/vuos/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable ../bin/vu_insmod"
	cd /home/alessio/Desktop/vuos/cmd && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/vu_insmod.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
cmd/CMakeFiles/vu_insmod.dir/build: bin/vu_insmod

.PHONY : cmd/CMakeFiles/vu_insmod.dir/build

cmd/CMakeFiles/vu_insmod.dir/requires: cmd/CMakeFiles/vu_insmod.dir/vu_insmod.c.o.requires

.PHONY : cmd/CMakeFiles/vu_insmod.dir/requires

cmd/CMakeFiles/vu_insmod.dir/clean:
	cd /home/alessio/Desktop/vuos/cmd && $(CMAKE_COMMAND) -P CMakeFiles/vu_insmod.dir/cmake_clean.cmake
.PHONY : cmd/CMakeFiles/vu_insmod.dir/clean

cmd/CMakeFiles/vu_insmod.dir/depend:
	cd /home/alessio/Desktop/vuos && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/alessio/Desktop/vuos /home/alessio/Desktop/vuos/cmd /home/alessio/Desktop/vuos /home/alessio/Desktop/vuos/cmd /home/alessio/Desktop/vuos/cmd/CMakeFiles/vu_insmod.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : cmd/CMakeFiles/vu_insmod.dir/depend

