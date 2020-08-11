set( MODULE_NAME Render2D ) 

set( KGL_VK_RENDER_MODULE_MODULE_SOURCES 
     Render2D.cpp
   )
     
set( KGL_VK_RENDER_MODULE_MODULE_HEADERS
     Render2D.h
   )

set( KGL_VK_RENDER_MODULE_MODULE_INCLUDE_DIRS
      ${DATA_BUS_DIR}
      ${KGL_NODE_DIR}
      ${Vulkan_INCLUDE_DIR}
    )

set( KGL_VK_RENDER_MODULE_MODULE_LIBRARIES
     kgl_containers
     kgl_vk_module
     kgl_vk_context
     kgl_managers
     kgl
     karma_data_bus
     karma_profiling
     karma_log
     ${Vulkan_LIBRARIES}
   )

add_library               ( render2D SHARED   ${KGL_VK_RENDER_MODULE_MODULE_SOURCES} ${KGL_VK_RENDER_MODULE_MODULE_HEADERS} )
target_include_directories( render2D PRIVATE  ${KGL_VK_RENDER_MODULE_MODULE_INCLUDE_DIRS}                                   )
target_link_libraries     ( render2D PRIVATE  ${KGL_VK_RENDER_MODULE_MODULE_LIBRARIES}                                      )
