FIND_PACKAGE( SDL2 REQUIRED )
set( KGL_IO_SOURCES 
     ObjectLoader.cpp
     Input.cpp
     ImageLoader.cpp
     stb_image.cpp
   )

set( KGL_IO_HEADERS
     ObjectLoader.h
     Input.h
     ImageLoader.h
     stb_image.h
   )

set( KGL_IO_INCLUDE_DIRS
    
   )

set( KGL_IO_LIBRARIES
   )

add_library( kgl_io SHARED ${KGL_IO_SOURCES} ${KGL_IO_HEADERS} )


