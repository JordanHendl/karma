
set( TOOLS_UWUWRITER_SOURCES 
     UWUWriter.cpp
   )
     
set( TOOLS_UWUWRITER_HEADERS
     UWUWriter.h
   )

set( TOOLS_UWUWRITER_INCLUDE_DIRS
     ${TOOLS_DIR}
   )

set( TOOLS_UWUWRITER_LIBRARIES
     glslang
     SPIRV
     stdc++fs
    )

add_library               ( karma_uwuwriter SHARED   ${TOOLS_UWUWRITER_SOURCES} ${TOOLS_UWUWRITER_HEADERS} )
target_include_directories( karma_uwuwriter PRIVATE  ${TOOLS_UWUWRITER_INCLUDE_DIRS}                       )
target_link_libraries     ( karma_uwuwriter PUBLIC  ${TOOLS_UWUWRITER_LIBRARIES}                          )

