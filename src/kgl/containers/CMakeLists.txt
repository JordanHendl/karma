set( KGL_CONTAINERS_SOURCES 
     BufferedStack.cpp
     List.cpp
   )
     
set( KGL_CONTAINERS_HEADERS
     BufferedStack.h
     List.h
   )

set( KGL_CONTAINERS_INCLUDE_DIRS
   )

set( KGL_CONTAINERS_LIBRARIES
    )

add_library               ( kgl_containers SHARED   ${KGL_CONTAINERS_SOURCES} ${KGL_CONTAINERS_HEADERS} )
target_include_directories( kgl_containers PRIVATE  ${KGL_CONTAINERS_INCLUDE_DIRS}                      )
target_link_libraries     ( kgl_containers PRIVATE  ${KGL_CONTAINERS_LIBRARIES}                         )

add_executable       ( kgl_containers_test test.cpp               )
target_link_libraries( kgl_containers_test PRIVATE kgl_containers )

