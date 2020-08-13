cmake_minimum_required( VERSION 3.8.0 )


#Build Options
option( WERROR         "Whether warnings should be treated as errors."                      ON  )
option( BUILD_DOC      "Whether or not documentation should be build"                       OFF ) 
option( BUILD_UWUMAKER "Whether or not to build the GLSLANG library & UWUMaker executable." ON  ) 

set ( MAJOR_VERSION 0 )
set ( MINOR_VERSION 1 )
set ( PATCH_VERSION 0 )

set( CMAKE_CXX_STANDARD 17 )


MESSAGE( "Warnings as Errors  : ${WERROR}"                                          ) 
MESSAGE( "Build Documentation : ${BUILD_DOC}"                                       ) 
MESSAGE( "Build UWUMaker      : ${BUILD_UWUMAKER}"                                  ) 
MESSAGE( "Version             : ${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION}" ) 

if( OPERATING_SYSTEM STREQUAL "WINDOWS" )

    MESSAGE( "BUILDING FOR WINDOWS" )

    # which compilers to use for C and C++
    set(CMAKE_C_FLAGS   "-g -fPIC")
    set(CMAKE_CXX_FLAGS "-g -std=c++17 -lstdc++fs -fPIC")

else()
    MESSAGE( " -Building for non-windows systems." )

    set(CMAKE_C_FLAGS   "-g -fPIC -Werror -Wall")
    set(CMAKE_CXX_FLAGS "-g -std=c++17 -lstdc++fs -fPIC -Werror -Wall")

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
