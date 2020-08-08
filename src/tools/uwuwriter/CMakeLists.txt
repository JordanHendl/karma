
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

add_library               ( uwuwriter SHARED   ${TOOLS_UWUWRITER_SOURCES} ${TOOLS_UWUWRITER_HEADERS} )
target_include_directories( uwuwriter PRIVATE  ${TOOLS_UWUWRITER_INCLUDE_DIRS}                       )
target_link_libraries     ( uwuwriter PRIVATE  ${TOOLS_UWUWRITER_LIBRARIES}                          )

