FIND_PACKAGE( SDL2 REQUIRED )
set( KGL_IO_SOURCES 
     ObjectLoader.cpp
     Input.cpp
     Event.cpp
     Font.cpp
     ImageLoader.cpp
     stb_image.cpp
     stb_truetype.cpp
   )

set( KGL_IO_HEADERS
     ObjectLoader.h
     Input.h
     Event.h
     Font.h
     ImageLoader.h
     stb_image.h
     stb_truetype.h
   )

set( KGL_IO_INCLUDE_DIRS
    ${DATA_BUS_DIR}
    ${SDL2_INCLUDE_DIRS}
   )

set( KGL_IO_LIBRARIES
     ${SDL2_LIBRARIES}
     karma_data_bus
   )


add_library               ( kgl_io SHARED ${KGL_IO_SOURCES} ${KGL_IO_HEADERS} )
target_link_libraries     ( kgl_io PUBLIC ${KGL_IO_LIBRARIES}                )
target_include_directories( kgl_io PRIVATE ${KGL_IO_INCLUDE_DIRS}             )

INSTALL( TARGETS kgl_io EXPORT kgl_lib DESTINATION ${KARMA_LIB_INSTALL_DIR} )
EXPORT ( PACKAGE kgl_io                                      )
INSTALL( TARGETS kgl_io DESTINATION ${KARMA_LIB_INSTALL_DIR} )