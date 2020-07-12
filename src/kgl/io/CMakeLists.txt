FIND_PACKAGE( SDL2 REQUIRED )
set( KGL_IO_SOURCES 
     ObjectLoader.cpp
     Input.cpp
   )

set( KGL_IO_HEADERS
     ObjectLoader.h
     Input.h
   )

set( KGL_IO_INCLUDE_DIRS
    
   )

set( KGL_IO_LIBRARIES
   )

add_library( kgl_input SHARED ${KGL_IO_SOURCES} ${KGL_IO_HEADERS} )


