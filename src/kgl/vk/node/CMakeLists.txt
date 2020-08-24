
set( KGL_VK_MODULE_SOURCES 
     Module.cpp
     Loader.cpp
     Graph.cpp
     Manager.cpp
   )
     
set( KGL_VK_MODULE_HEADERS
     Module.h
     Loader.h
     Manager.h
     Graph.h
   )

set( KGL_VK_MODULE_INCLUDE_DIRS
      ${DATA_BUS_DIR}
      ${CONFIGURATION_DIR}
      ${Vulkan_INCLUDE_DIR}
    )

set( KGL_VK_MODULE_LIBRARIES
     kgl_io
     kgl_vk_context
     stdc++fs
     karma_data_bus
     karma_configuration
     karma_log
     ${Vulkan_LIBRARY}
   )

add_library               ( kgl_vk_module SHARED   ${KGL_VK_MODULE_SOURCES} ${KGL_VK_MODULE_HEADERS} )
target_include_directories( kgl_vk_module PRIVATE  ${KGL_VK_MODULE_INCLUDE_DIRS}                     )
target_link_libraries     ( kgl_vk_module PUBLIC  ${KGL_VK_MODULE_LIBRARIES}                        )

INSTALL( TARGETS kgl_vk_module EXPORT kgl_lib DESTINATION ${KARMA_LIB_INSTALL_DIR} )