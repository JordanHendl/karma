set( KGL_CONTAINERS_SOURCES 
     BufferedStack.cpp
     List.cpp
     Layered.cpp
   )
     
set( KGL_CONTAINERS_HEADERS
     BufferedStack.h
     List.h
     Layered.h
   )

set( KGL_CONTAINERS_INCLUDE_DIRS
   )

set( KGL_CONTAINERS_LIBRARIES
    )

add_library               ( kgl_containers SHARED   ${KGL_CONTAINERS_SOURCES} ${KGL_CONTAINERS_HEADERS} )
target_include_directories( kgl_containers PRIVATE  ${KGL_CONTAINERS_INCLUDE_DIRS}                      )
target_link_libraries     ( kgl_containers PUBLIC  ${KGL_CONTAINERS_LIBRARIES}                         )

add_executable       ( kgl_containers_test test.cpp               )
target_link_libraries( kgl_containers_test PUBLIC kgl_containers )

EXPORT ( PACKAGE kgl_containers                                                  )
INSTALL( TARGETS kgl_containers EXPORT kgl_lib DESTINATION ${KARMA_LIB_INSTALL_DIR} )
INSTALL( TARGETS kgl_containers          DESTINATION ${KARMA_LIB_INSTALL_DIR}    )
INSTALL( FILES ${KGL_CONTAINERS_HEADERS} DESTINATION ${KARMA_HEADER_INSTALL_DIR} )

