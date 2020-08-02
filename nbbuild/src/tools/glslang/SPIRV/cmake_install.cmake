# Install script for directory: /wksp/int/KGLRefactor/karma/src/tools/glslang/SPIRV

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "0")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib64" TYPE STATIC_LIBRARY FILES "/wksp/int/KGLRefactor/karma/nbbuild/lib/libSPVRemapper.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib64" TYPE STATIC_LIBRARY FILES "/wksp/int/KGLRefactor/karma/nbbuild/lib/libSPIRV.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib64/cmake/SPVRemapperTargets.cmake")
    file(DIFFERENT EXPORT_FILE_CHANGED FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib64/cmake/SPVRemapperTargets.cmake"
         "/wksp/int/KGLRefactor/karma/nbbuild/src/tools/glslang/SPIRV/CMakeFiles/Export/lib64/cmake/SPVRemapperTargets.cmake")
    if(EXPORT_FILE_CHANGED)
      file(GLOB OLD_CONFIG_FILES "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib64/cmake/SPVRemapperTargets-*.cmake")
      if(OLD_CONFIG_FILES)
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib64/cmake/SPVRemapperTargets.cmake\" will be replaced.  Removing files [${OLD_CONFIG_FILES}].")
        file(REMOVE ${OLD_CONFIG_FILES})
      endif()
    endif()
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib64/cmake" TYPE FILE FILES "/wksp/int/KGLRefactor/karma/nbbuild/src/tools/glslang/SPIRV/CMakeFiles/Export/lib64/cmake/SPVRemapperTargets.cmake")
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^()$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib64/cmake" TYPE FILE FILES "/wksp/int/KGLRefactor/karma/nbbuild/src/tools/glslang/SPIRV/CMakeFiles/Export/lib64/cmake/SPVRemapperTargets-noconfig.cmake")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib64/cmake/SPIRVTargets.cmake")
    file(DIFFERENT EXPORT_FILE_CHANGED FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib64/cmake/SPIRVTargets.cmake"
         "/wksp/int/KGLRefactor/karma/nbbuild/src/tools/glslang/SPIRV/CMakeFiles/Export/lib64/cmake/SPIRVTargets.cmake")
    if(EXPORT_FILE_CHANGED)
      file(GLOB OLD_CONFIG_FILES "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib64/cmake/SPIRVTargets-*.cmake")
      if(OLD_CONFIG_FILES)
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib64/cmake/SPIRVTargets.cmake\" will be replaced.  Removing files [${OLD_CONFIG_FILES}].")
        file(REMOVE ${OLD_CONFIG_FILES})
      endif()
    endif()
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib64/cmake" TYPE FILE FILES "/wksp/int/KGLRefactor/karma/nbbuild/src/tools/glslang/SPIRV/CMakeFiles/Export/lib64/cmake/SPIRVTargets.cmake")
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^()$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib64/cmake" TYPE FILE FILES "/wksp/int/KGLRefactor/karma/nbbuild/src/tools/glslang/SPIRV/CMakeFiles/Export/lib64/cmake/SPIRVTargets-noconfig.cmake")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/SPIRV" TYPE FILE FILES
    "/wksp/int/KGLRefactor/karma/src/tools/glslang/SPIRV/bitutils.h"
    "/wksp/int/KGLRefactor/karma/src/tools/glslang/SPIRV/spirv.hpp"
    "/wksp/int/KGLRefactor/karma/src/tools/glslang/SPIRV/GLSL.std.450.h"
    "/wksp/int/KGLRefactor/karma/src/tools/glslang/SPIRV/GLSL.ext.EXT.h"
    "/wksp/int/KGLRefactor/karma/src/tools/glslang/SPIRV/GLSL.ext.KHR.h"
    "/wksp/int/KGLRefactor/karma/src/tools/glslang/SPIRV/GlslangToSpv.h"
    "/wksp/int/KGLRefactor/karma/src/tools/glslang/SPIRV/hex_float.h"
    "/wksp/int/KGLRefactor/karma/src/tools/glslang/SPIRV/Logger.h"
    "/wksp/int/KGLRefactor/karma/src/tools/glslang/SPIRV/SpvBuilder.h"
    "/wksp/int/KGLRefactor/karma/src/tools/glslang/SPIRV/spvIR.h"
    "/wksp/int/KGLRefactor/karma/src/tools/glslang/SPIRV/doc.h"
    "/wksp/int/KGLRefactor/karma/src/tools/glslang/SPIRV/SpvTools.h"
    "/wksp/int/KGLRefactor/karma/src/tools/glslang/SPIRV/disassemble.h"
    "/wksp/int/KGLRefactor/karma/src/tools/glslang/SPIRV/GLSL.ext.AMD.h"
    "/wksp/int/KGLRefactor/karma/src/tools/glslang/SPIRV/GLSL.ext.NV.h"
    "/wksp/int/KGLRefactor/karma/src/tools/glslang/SPIRV/NonSemanticDebugPrintf.h"
    "/wksp/int/KGLRefactor/karma/src/tools/glslang/SPIRV/SPVRemapper.h"
    "/wksp/int/KGLRefactor/karma/src/tools/glslang/SPIRV/doc.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/glslang/SPIRV" TYPE FILE FILES
    "/wksp/int/KGLRefactor/karma/src/tools/glslang/SPIRV/bitutils.h"
    "/wksp/int/KGLRefactor/karma/src/tools/glslang/SPIRV/spirv.hpp"
    "/wksp/int/KGLRefactor/karma/src/tools/glslang/SPIRV/GLSL.std.450.h"
    "/wksp/int/KGLRefactor/karma/src/tools/glslang/SPIRV/GLSL.ext.EXT.h"
    "/wksp/int/KGLRefactor/karma/src/tools/glslang/SPIRV/GLSL.ext.KHR.h"
    "/wksp/int/KGLRefactor/karma/src/tools/glslang/SPIRV/GlslangToSpv.h"
    "/wksp/int/KGLRefactor/karma/src/tools/glslang/SPIRV/hex_float.h"
    "/wksp/int/KGLRefactor/karma/src/tools/glslang/SPIRV/Logger.h"
    "/wksp/int/KGLRefactor/karma/src/tools/glslang/SPIRV/SpvBuilder.h"
    "/wksp/int/KGLRefactor/karma/src/tools/glslang/SPIRV/spvIR.h"
    "/wksp/int/KGLRefactor/karma/src/tools/glslang/SPIRV/doc.h"
    "/wksp/int/KGLRefactor/karma/src/tools/glslang/SPIRV/SpvTools.h"
    "/wksp/int/KGLRefactor/karma/src/tools/glslang/SPIRV/disassemble.h"
    "/wksp/int/KGLRefactor/karma/src/tools/glslang/SPIRV/GLSL.ext.AMD.h"
    "/wksp/int/KGLRefactor/karma/src/tools/glslang/SPIRV/GLSL.ext.NV.h"
    "/wksp/int/KGLRefactor/karma/src/tools/glslang/SPIRV/NonSemanticDebugPrintf.h"
    "/wksp/int/KGLRefactor/karma/src/tools/glslang/SPIRV/SPVRemapper.h"
    "/wksp/int/KGLRefactor/karma/src/tools/glslang/SPIRV/doc.h"
    )
endif()

