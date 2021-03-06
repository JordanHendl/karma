cmake_minimum_required( VERSION 3.8.0 )
project( karma LANGUAGES CXX )

#Build Options
option( BUILD_DOC      "Whether or not documentation should be build"                       OFF ) 
option( BUILD_UWUMAKER "Whether or not to build the GLSLANG library & UWUMaker executable." OFF ) 
option( RELEASE        "Whether or not to build with optimizations for release."            OFF ) 
option( WERROR         "Whether warnings should be treated as errors."                      ON  )

set ( MAJOR_VERSION 0 )
set ( MINOR_VERSION 1 )
set ( PATCH_VERSION 0 )

set( CMAKE_CXX_STANDARD 17 )
set(CMAKE_SKIP_BUILD_RPATH FALSE)


MESSAGE( "Warnings as Errors  : ${WERROR}"                                          ) 
MESSAGE( "Build Documentation : ${BUILD_DOC}"                                       ) 
MESSAGE( "Build UWUMaker      : ${BUILD_UWUMAKER}"                                  ) 
MESSAGE( "Release             : ${RELEASE}"                                         ) 
MESSAGE( "Version             : ${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION}" ) 

if( WERROR STREQUAL "OFF" )

    MESSAGE( "-Compiling without warnings as errors." )

    # which compilers to use for C and C++
    set(CMAKE_C_FLAGS   "-fPIC" )
    set(CMAKE_CXX_FLAGS "-std=c++17 -lstdc++fs -fPIC")

else()
    MESSAGE( "-Compiling with warnings as errors." )

    set(CMAKE_C_FLAGS   "-fPIC -Werror -Wall"                       )
    set(CMAKE_CXX_FLAGS "-std=c++17 -lstdc++fs -fPIC -Werror -Wall ")

endif( WERROR STREQUAL "OFF" )

if( RELEASE STREQUAL "ON" )
    MESSAGE( "-Building for release" )
    set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -O3"   )
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3" )
else()
    MESSAGE( "-Building for debug" )
    set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -g"   )
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g" )
endif( RELEASE STREQUAL "ON" )


#Application information.
set( ENGINE_NAME              "Karma"                       )
set( VENDOR                   "Overcaster"                  ) 
set( INSTALL_SUMMARY          "Karma Engine - Installation" )
set( INSTALL_DIR              "./karma"                     )
set( LIB_INSTALL_DIR          "./karma/lib64"               )
set( BIN_INSTALL_DIR          "./karma/bin"                 )
set( INC_INSTALL_DIR          "./karma/include"             )
set( KARMA_LIB_INSTALL_DIR    "karma/lib"                   )
set( KARMA_HEADER_INSTALL_DIR "karma/include"               )
set( KARMA_MODULE_INSTALL_DIR "karma/module"                )
set( KARMA_UWU_INSTALL_DIR    "karma/uwu"                   )
set( KARMA_INSTALL_DIR        "karma"                       )

set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/" )

# Set output directories.
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib  )
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib  )
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin  )
set(DOCUMENTATION_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/docs )

#Create output directory for documentation.
file( MAKE_DIRECTORY ${DOCUMENTATION_OUTPUT_DIRECTORY} )



#CPack options
set( CPACK_PACKAGE_NAME                ${ENGINE_NAME}     )
set( CPACK_SOURCE_GENERATOR "TGZ;ZIP"                     )
set( CPACK_PACKAGE_VENDOR              ${VENDOR}          )
set( CPACK_PACKAGE_DESCRIPTION_SUMMARY ${INSTALL_SUMMARY} )
set( CPACK_PACKAGE_VERSION_MAJOR       ${MAJOR_VERSION}   )
set( CPACK_PACKAGE_VERSION_MINOR       ${MINOR_VERSION}   )
set( CPACK_PACKAGE_VERSION_PATCH       ${PATCH_VERSION}   )
set( CPACK_PACKAGE_INSTALL_DIRECTORY   "./"               )
set( CMAKE_INSTALL_PREFIX              "./"               ) 

set( PROJECT_SRCS       ${CMAKE_WORKING_DIRECTORY}/src      )
set( DOXYGEN_CONFIG_DIR ${CMAKE_CURRENT_SOURCE_DIR}/doxygen )


set( CMAKE_INSTALL_RPATH FALSE )
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/karma/lib")

set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
list(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_INSTALL_PREFIX}/karma/lib" isSystemDir)
if("${isSystemDir}" STREQUAL "-1")
    set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
endif("${isSystemDir}" STREQUAL "-1")

include(CPack)
find_package( Doxygen )

add_subdirectory( src )

install( FILES setup.json             DESTINATION ${KARMA_INSTALL_DIR} ) 
install( FILES kgl_configuration.json DESTINATION ${KARMA_INSTALL_DIR} ) 
install( FILES database.json          DESTINATION ${KARMA_INSTALL_DIR} ) 
install( DIRECTORY uwu                DESTINATION ${KARMA_INSTALL_DIR} )