# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.15

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
CMAKE_COMMAND = /home/viktor/Загрузки/clion-2019.3.4/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /home/viktor/Загрузки/clion-2019.3.4/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/viktor/os/lab01withFetch

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/viktor/os/lab01withFetch/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/lab01withFetch.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/lab01withFetch.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/lab01withFetch.dir/flags.make

CMakeFiles/lab01withFetch.dir/main.cpp.o: CMakeFiles/lab01withFetch.dir/flags.make
CMakeFiles/lab01withFetch.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/viktor/os/lab01withFetch/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/lab01withFetch.dir/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/lab01withFetch.dir/main.cpp.o -c /home/viktor/os/lab01withFetch/main.cpp

CMakeFiles/lab01withFetch.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/lab01withFetch.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/viktor/os/lab01withFetch/main.cpp > CMakeFiles/lab01withFetch.dir/main.cpp.i

CMakeFiles/lab01withFetch.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/lab01withFetch.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/viktor/os/lab01withFetch/main.cpp -o CMakeFiles/lab01withFetch.dir/main.cpp.s

# Object files for target lab01withFetch
lab01withFetch_OBJECTS = \
"CMakeFiles/lab01withFetch.dir/main.cpp.o"

# External object files for target lab01withFetch
lab01withFetch_EXTERNAL_OBJECTS =

lab01withFetch: CMakeFiles/lab01withFetch.dir/main.cpp.o
lab01withFetch: CMakeFiles/lab01withFetch.dir/build.make
lab01withFetch: CMakeFiles/lab01withFetch.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/viktor/os/lab01withFetch/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable lab01withFetch"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/lab01withFetch.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/lab01withFetch.dir/build: lab01withFetch

.PHONY : CMakeFiles/lab01withFetch.dir/build

CMakeFiles/lab01withFetch.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/lab01withFetch.dir/cmake_clean.cmake
.PHONY : CMakeFiles/lab01withFetch.dir/clean

CMakeFiles/lab01withFetch.dir/depend:
	cd /home/viktor/os/lab01withFetch/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/viktor/os/lab01withFetch /home/viktor/os/lab01withFetch /home/viktor/os/lab01withFetch/cmake-build-debug /home/viktor/os/lab01withFetch/cmake-build-debug /home/viktor/os/lab01withFetch/cmake-build-debug/CMakeFiles/lab01withFetch.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/lab01withFetch.dir/depend

