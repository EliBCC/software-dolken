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
CMAKE_BINARY_DIR = C:/Users/jakob/OneDrive/Dokumenter/GitHub/Software-dolken/build/MAX31855-debug

# Utility rule file for MAX31855.bin.

# Include the progress variables for this target.
include CMakeFiles/MAX31855.bin.dir/progress.make

CMakeFiles/MAX31855.bin: MAX31855.elf
	arm-none-eabi-objcopy.exe -Obinary MAX31855.elf MAX31855.bin

MAX31855.bin: CMakeFiles/MAX31855.bin
MAX31855.bin: CMakeFiles/MAX31855.bin.dir/build.make

.PHONY : MAX31855.bin

# Rule to build all files generated by this target.
CMakeFiles/MAX31855.bin.dir/build: MAX31855.bin

.PHONY : CMakeFiles/MAX31855.bin.dir/build

CMakeFiles/MAX31855.bin.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/MAX31855.bin.dir/cmake_clean.cmake
.PHONY : CMakeFiles/MAX31855.bin.dir/clean

CMakeFiles/MAX31855.bin.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" C:/Users/jakob/OneDrive/Dokumenter/GitHub/Software-dolken C:/Users/jakob/OneDrive/Dokumenter/GitHub/Software-dolken C:/Users/jakob/OneDrive/Dokumenter/GitHub/Software-dolken/build/MAX31855-debug C:/Users/jakob/OneDrive/Dokumenter/GitHub/Software-dolken/build/MAX31855-debug C:/Users/jakob/OneDrive/Dokumenter/GitHub/Software-dolken/build/MAX31855-debug/CMakeFiles/MAX31855.bin.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/MAX31855.bin.dir/depend

