# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.12

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
CMAKE_COMMAND = C:/Users/jakob/scoop/apps/cmake/3.12.4/bin/cmake.exe

# The command to remove a file.
RM = C:/Users/jakob/scoop/apps/cmake/3.12.4/bin/cmake.exe -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:/Users/jakob/OneDrive/Dokumenter/GitHub/Software-dolken

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:/Users/jakob/OneDrive/Dokumenter/GitHub/Software-dolken/build/blink-debug

# Utility rule file for blink.lss.

# Include the progress variables for this target.
include CMakeFiles/blink.lss.dir/progress.make

CMakeFiles/blink.lss: blink.elf
	arm-none-eabi-objdump.exe -x -s -S -l -w blink.elf > blink.lss

blink.lss: CMakeFiles/blink.lss
blink.lss: CMakeFiles/blink.lss.dir/build.make

.PHONY : blink.lss

# Rule to build all files generated by this target.
CMakeFiles/blink.lss.dir/build: blink.lss

.PHONY : CMakeFiles/blink.lss.dir/build

CMakeFiles/blink.lss.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/blink.lss.dir/cmake_clean.cmake
.PHONY : CMakeFiles/blink.lss.dir/clean

CMakeFiles/blink.lss.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" C:/Users/jakob/OneDrive/Dokumenter/GitHub/Software-dolken C:/Users/jakob/OneDrive/Dokumenter/GitHub/Software-dolken C:/Users/jakob/OneDrive/Dokumenter/GitHub/Software-dolken/build/blink-debug C:/Users/jakob/OneDrive/Dokumenter/GitHub/Software-dolken/build/blink-debug C:/Users/jakob/OneDrive/Dokumenter/GitHub/Software-dolken/build/blink-debug/CMakeFiles/blink.lss.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/blink.lss.dir/depend

