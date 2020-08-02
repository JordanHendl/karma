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

# Include any dependencies generated for this target.
include src/tools/glslang/SPIRV/CMakeFiles/SPVRemapper.dir/depend.make

# Include the progress variables for this target.
include src/tools/glslang/SPIRV/CMakeFiles/SPVRemapper.dir/progress.make

# Include the compile flags for this target's objects.
include src/tools/glslang/SPIRV/CMakeFiles/SPVRemapper.dir/flags.make

src/tools/glslang/SPIRV/CMakeFiles/SPVRemapper.dir/SPVRemapper.cpp.o: src/tools/glslang/SPIRV/CMakeFiles/SPVRemapper.dir/flags.make
src/tools/glslang/SPIRV/CMakeFiles/SPVRemapper.dir/SPVRemapper.cpp.o: ../src/tools/glslang/SPIRV/SPVRemapper.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/wksp/int/KGLRefactor/karma/nbbuild/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/tools/glslang/SPIRV/CMakeFiles/SPVRemapper.dir/SPVRemapper.cpp.o"
	cd /wksp/int/KGLRefactor/karma/nbbuild/src/tools/glslang/SPIRV && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/SPVRemapper.dir/SPVRemapper.cpp.o -c /wksp/int/KGLRefactor/karma/src/tools/glslang/SPIRV/SPVRemapper.cpp

src/tools/glslang/SPIRV/CMakeFiles/SPVRemapper.dir/SPVRemapper.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/SPVRemapper.dir/SPVRemapper.cpp.i"
	cd /wksp/int/KGLRefactor/karma/nbbuild/src/tools/glslang/SPIRV && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /wksp/int/KGLRefactor/karma/src/tools/glslang/SPIRV/SPVRemapper.cpp > CMakeFiles/SPVRemapper.dir/SPVRemapper.cpp.i

src/tools/glslang/SPIRV/CMakeFiles/SPVRemapper.dir/SPVRemapper.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/SPVRemapper.dir/SPVRemapper.cpp.s"
	cd /wksp/int/KGLRefactor/karma/nbbuild/src/tools/glslang/SPIRV && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /wksp/int/KGLRefactor/karma/src/tools/glslang/SPIRV/SPVRemapper.cpp -o CMakeFiles/SPVRemapper.dir/SPVRemapper.cpp.s

src/tools/glslang/SPIRV/CMakeFiles/SPVRemapper.dir/doc.cpp.o: src/tools/glslang/SPIRV/CMakeFiles/SPVRemapper.dir/flags.make
src/tools/glslang/SPIRV/CMakeFiles/SPVRemapper.dir/doc.cpp.o: ../src/tools/glslang/SPIRV/doc.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/wksp/int/KGLRefactor/karma/nbbuild/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object src/tools/glslang/SPIRV/CMakeFiles/SPVRemapper.dir/doc.cpp.o"
	cd /wksp/int/KGLRefactor/karma/nbbuild/src/tools/glslang/SPIRV && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/SPVRemapper.dir/doc.cpp.o -c /wksp/int/KGLRefactor/karma/src/tools/glslang/SPIRV/doc.cpp

src/tools/glslang/SPIRV/CMakeFiles/SPVRemapper.dir/doc.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/SPVRemapper.dir/doc.cpp.i"
	cd /wksp/int/KGLRefactor/karma/nbbuild/src/tools/glslang/SPIRV && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /wksp/int/KGLRefactor/karma/src/tools/glslang/SPIRV/doc.cpp > CMakeFiles/SPVRemapper.dir/doc.cpp.i

src/tools/glslang/SPIRV/CMakeFiles/SPVRemapper.dir/doc.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/SPVRemapper.dir/doc.cpp.s"
	cd /wksp/int/KGLRefactor/karma/nbbuild/src/tools/glslang/SPIRV && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /wksp/int/KGLRefactor/karma/src/tools/glslang/SPIRV/doc.cpp -o CMakeFiles/SPVRemapper.dir/doc.cpp.s

# Object files for target SPVRemapper
SPVRemapper_OBJECTS = \
"CMakeFiles/SPVRemapper.dir/SPVRemapper.cpp.o" \
"CMakeFiles/SPVRemapper.dir/doc.cpp.o"

# External object files for target SPVRemapper
SPVRemapper_EXTERNAL_OBJECTS =

lib/libSPVRemapper.a: src/tools/glslang/SPIRV/CMakeFiles/SPVRemapper.dir/SPVRemapper.cpp.o
lib/libSPVRemapper.a: src/tools/glslang/SPIRV/CMakeFiles/SPVRemapper.dir/doc.cpp.o
lib/libSPVRemapper.a: src/tools/glslang/SPIRV/CMakeFiles/SPVRemapper.dir/build.make
lib/libSPVRemapper.a: src/tools/glslang/SPIRV/CMakeFiles/SPVRemapper.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/wksp/int/KGLRefactor/karma/nbbuild/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX static library ../../../../lib/libSPVRemapper.a"
	cd /wksp/int/KGLRefactor/karma/nbbuild/src/tools/glslang/SPIRV && $(CMAKE_COMMAND) -P CMakeFiles/SPVRemapper.dir/cmake_clean_target.cmake
	cd /wksp/int/KGLRefactor/karma/nbbuild/src/tools/glslang/SPIRV && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/SPVRemapper.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/tools/glslang/SPIRV/CMakeFiles/SPVRemapper.dir/build: lib/libSPVRemapper.a

.PHONY : src/tools/glslang/SPIRV/CMakeFiles/SPVRemapper.dir/build

src/tools/glslang/SPIRV/CMakeFiles/SPVRemapper.dir/clean:
	cd /wksp/int/KGLRefactor/karma/nbbuild/src/tools/glslang/SPIRV && $(CMAKE_COMMAND) -P CMakeFiles/SPVRemapper.dir/cmake_clean.cmake
.PHONY : src/tools/glslang/SPIRV/CMakeFiles/SPVRemapper.dir/clean

src/tools/glslang/SPIRV/CMakeFiles/SPVRemapper.dir/depend:
	cd /wksp/int/KGLRefactor/karma/nbbuild && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /wksp/int/KGLRefactor/karma /wksp/int/KGLRefactor/karma/src/tools/glslang/SPIRV /wksp/int/KGLRefactor/karma/nbbuild /wksp/int/KGLRefactor/karma/nbbuild/src/tools/glslang/SPIRV /wksp/int/KGLRefactor/karma/nbbuild/src/tools/glslang/SPIRV/CMakeFiles/SPVRemapper.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/tools/glslang/SPIRV/CMakeFiles/SPVRemapper.dir/depend

