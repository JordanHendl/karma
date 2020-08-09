cmake_minimum_required( VERSION 3.8.2 )


#Build Options
option( OPEPRATING_SYSTEM "The Operating system to build for."                                 "LINUX" )
option( BUILD_DOC         "Whether or not documentation should be build"                       OFF     ) 
option( BUILD_UWUMAKER    "Whether or not to build the GLSLANG library & UWUMaker executable." ON      ) 
option( MAJOR_VERSION     "The major component of the version"                                 0       ) 
option( MINOR_VERSION     "The minor component of the version"                                 1       ) 
option( PATCH_VERSION     "The patch component of the version"                                 0       ) 
set( CMAKE_CXX_STANDARD 17 )

if( OPERATING_SYSTEM STREQUAL "WINDOWS" )

    MESSAGE( "BUILDING FOR WINDOWS" )
    SET( CMAKE_SYSTEM_NAME Windows)

    # which compilers to use for C and C++
    SET( CMAKE_C_COMPILER   /usr/bin/x86_64-w64-mingw32-gcc )
    SET( CMAKE_CXX_COMPILER /usr/bin/x86_64-w64-mingw32-g++ )

    # here is the target environment located
    SET(CMAKE_FIND_ROOT_PATH  /usr/i586-mingw32msvc /home/alex/mingw-install )

    # adjust the default behaviour of the FIND_XXX() commands:
    # search headers and libraries in the target environment, search
    # programs in the host environment
#    set( CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER )
#    set( CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY  )
#    set( CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY  )

endif( OPERATING_SYSTEM STREQUAL "WINDOWS" )

project( karma LANGUAGES CXX )

#Application information.
set( ENGINE_NAME     "Karma"                       )
set( VENDOR          "Quiet"                       ) 
set( INSTALL_SUMMARY "Karma Engine - Installation" )
set( INSTALL_DIR     "/usr/local/karma"            )
set( LIB_INSTALL_DIR "/usr/local/karma/lib64"      )
set( BIN_INSTALL_DIR "/usr/local/karma/bin"        )
set( INC_INSTALL_DIR "/usr/local/karma/include"    )

#C++/C Compiler Flags.
set(CMAKE_C_FLAGS   "-g -fPIC -Werror -Wall")
set(CMAKE_CXX_FLAGS "-g -std=c++17 -lstdc++fs -fPIC -Werror -Wall")

# Set output directories.
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib  )
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib  )
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin  )
set(DOCUMENTATION_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/docs )

#Create output directory for documentation.
file( MAKE_DIRECTORY ${DOCUMENTATION_OUTPUT_DIRECTORY} )

#CPack options
set(CPACK_GENERATOR NSIS)
set(CPACK_PACKAGE_NAME                ${ENGINE_NAME}     )
set(CPACK_PACKAGE_VENDOR              ${VENDOR}          )
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY ${INSTALL_SUMMARY} )
set(CPACK_PACKAGE_VERSION_MAJOR       ${MAJOR_VERSION}   )
set(CPACK_PACKAGE_VERSION_MINOR       ${MINOR_VERSION}   )
set(CPACK_PACKAGE_VERSION_PATCH       ${PATCH_VERSION}   )
set(CPACK_PACKAGE_INSTALL_DIRECTORY )

set( PROJECT_SRCS       ${CMAKE_WORKING_DIRECTORY}/src         )
set( CASPER_DIR         ${CMAKE_WORKING_DIRECTORY}/src/casper  )
set( VKTEST_DIR         ${CMAKE_WORKING_DIRECTORY}/src/vk_test )
set( DOXYGEN_CONFIG_DIR ${CMAKE_CURRENT_SOURCE_DIR}/doxygen    )

find_package(Doxygen)

add_subdirectory( src )
