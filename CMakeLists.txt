cmake_minimum_required( VERSION 3.8.2 )

project( casper LANGUAGES CXX )

set( CMAKE_CXX_STANDARD 17 )
find_package( SDL2 REQUIRED )
include_directories(${SDL2_INCLUDE_DIRS})
set(CMAKE_C_FLAGS   "-g -std=c++17 -lstdc++fs -fPIC ")
set(CMAKE_CXX_FLAGS "-g -std=c++17 -lstdc++fs -fPIC ")

set( PROJ_INCLUDE_DIR ${CMAKE_WORKING_DIRECTORY}/include )
set( PROJ_LIBRARY_DIR ${CMAKE_WORKING_DIRECTORY}/lib     )

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )

set( PROJECT_SRCS ${CMAKE_WORKING_DIRECTORY}/src )
set( CASPER_DIR ${CMAKE_WORKING_DIRECTORY}/src/casper )
set( VKTEST_DIR ${CMAKE_WORKING_DIRECTORY}/src/vk_test )

add_subdirectory( src )
