FIND_PACKAGE( SDL2 REQUIRED )
set( KGL_IO_SOURCES 
     ObjectLoader.cpp
     Input.cpp
     Event.cpp
     ImageLoader.cpp
     stb_image.cpp
   )

set( KGL_IO_HEADERS
     ObjectLoader.h
     Input.h
     Event.h
     ImageLoader.h
     stb_image.h
   )

set( KGL_IO_INCLUDE_DIRS
    ${DATA_BUS_DIR}
    ${SDL2_INCLUDE_DIRS}
   )

set( KGL_IO_LIBRARIES
     data_bus
     ${SDL2_LIBRARIES}
   )

add_library( kgl_io SHARED ${KGL_IO_SOURCES} ${KGL_IO_HEADERS} )


