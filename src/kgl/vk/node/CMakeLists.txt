
set( KGL_VK_MODULE_SOURCES 
     Module.cpp
     Loader.cpp
     Graph.cpp
     Manager.cpp
     Synchronization.cpp
   )
     
set( KGL_VK_MODULE_HEADERS
     Module.h
     Loader.h
     Manager.h
     Graph.h
     Synchronization.h
   )

set( KGL_VK_MODULE_INCLUDE_DIRS
      ${DATA_BUS_DIR}
      ${CONFIGURATION_DIR}
    )

set( KGL_VK_MODULE_LIBRARIES
     data_bus
     kgl_io
     stdc++fs
    configuration
   )

add_library               ( kgl_vk_module SHARED   ${KGL_VK_MODULE_SOURCES} ${KGL_VK_MODULE_HEADERS} )
target_include_directories( kgl_vk_module PRIVATE  ${KGL_VK_MODULE_INCLUDE_DIRS}                     )
target_link_libraries     ( kgl_vk_module PRIVATE  ${KGL_VK_MODULE_LIBRARIES}                        )