# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.11

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
CMAKE_SOURCE_DIR = /wksp/int/KGLRefactor/karma

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /wksp/int/KGLRefactor/karma/nbbuild

# Utility rule file for ContinuousCoverage.

# Include the progress variables for this target.
include src/tools/glslang/CMakeFiles/ContinuousCoverage.dir/progress.make

src/tools/glslang/CMakeFiles/ContinuousCoverage:
	cd /wksp/int/KGLRefactor/karma/nbbuild/src/tools/glslang && /usr/bin/ctest -D ContinuousCoverage

ContinuousCoverage: src/tools/glslang/CMakeFiles/ContinuousCoverage
ContinuousCoverage: src/tools/glslang/CMakeFiles/ContinuousCoverage.dir/build.make

.PHONY : ContinuousCoverage

# Rule to build all files generated by this target.
src/tools/glslang/CMakeFiles/ContinuousCoverage.dir/build: ContinuousCoverage

.PHONY : src/tools/glslang/CMakeFiles/ContinuousCoverage.dir/build

src/tools/glslang/CMakeFiles/ContinuousCoverage.dir/clean:
	cd /wksp/int/KGLRefactor/karma/nbbuild/src/tools/glslang && $(CMAKE_COMMAND) -P CMakeFiles/ContinuousCoverage.dir/cmake_clean.cmake
.PHONY : src/tools/glslang/CMakeFiles/ContinuousCoverage.dir/clean

src/tools/glslang/CMakeFiles/ContinuousCoverage.dir/depend:
	cd /wksp/int/KGLRefactor/karma/nbbuild && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /wksp/int/KGLRefactor/karma /wksp/int/KGLRefactor/karma/src/tools/glslang /wksp/int/KGLRefactor/karma/nbbuild /wksp/int/KGLRefactor/karma/nbbuild/src/tools/glslang /wksp/int/KGLRefactor/karma/nbbuild/src/tools/glslang/CMakeFiles/ContinuousCoverage.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/tools/glslang/CMakeFiles/ContinuousCoverage.dir/depend

