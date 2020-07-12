
set( KGL_VK_DATA_MODULE_SOURCES 
     Buffer.cpp
     Image.cpp
   )
     
set( KGL_VK_DATA_MODULE_HEADERS
     Buffer.h
     Image.h
   )

set( KGL_VK_DATA_MODULE_INCLUDE_DIRS
     ${DATA_BUS_DIR}
    )

set( KGL_VK_DATA_MODULE_LIBRARIES
     data_bus
   )

add_library               ( kgl_data SHARED   ${KGL_VK_DATA_MODULE_SOURCES} ${KGL_VK_DATA_MODULE_HEADERS} )
target_include_directories( kgl_data PRIVATE  ${KGL_VK_DATA_MODULE_INCLUDE_DIRS}                          )
target_link_libraries     ( kgl_data PRIVATE  ${KGL_VK_DATA_MODULE_LIBRARIES}                             )