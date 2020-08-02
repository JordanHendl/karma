# CMake generated Testfile for 
# Source directory: /wksp/int/KGLRefactor/karma/src/tools/glslang
# Build directory: /wksp/int/KGLRefactor/karma/nbbuild/src/tools/glslang
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(glslang-testsuite "bash" "runtests" "/wksp/int/KGLRefactor/karma/nbbuild/src/tools/glslang/localResults" "/wksp/int/KGLRefactor/karma/nbbuild/src/tools/glslang/StandAlone/glslangValidator" "/wksp/int/KGLRefactor/karma/nbbuild/src/tools/glslang/StandAlone/spirv-remap")
set_tests_properties(glslang-testsuite PROPERTIES  WORKING_DIRECTORY "/wksp/int/KGLRefactor/karma/src/tools/glslang/Test/")
subdirs("External")
subdirs("glslang")
subdirs("OGLCompilersDLL")
subdirs("StandAlone")
subdirs("SPIRV")
subdirs("hlsl")
subdirs("gtests")
