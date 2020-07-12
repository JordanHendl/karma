set( KGL_VK_RENDER_MODULE_MODULE_SOURCES 
     Render2D.cpp
   )
     
set( KGL_VK_RENDER_MODULE_MODULE_HEADERS
     Render2D.h
   )

set( KGL_VK_RENDER_MODULE_MODULE_INCLUDE_DIRS
      ${DATA_BUS_DIR}
      ${KGL_NODE_DIR}
    )

set( KGL_VK_RENDER_MODULE_MODULE_LIBRARIES
     data_bus
     kgl_vk_module
   )

add_library               ( render2D_module SHARED   ${KGL_VK_RENDER_MODULE_MODULE_SOURCES} ${KGL_VK_RENDER_MODULE_MODULE_HEADERS} )
target_include_directories( render2D_module PRIVATE  ${KGL_VK_RENDER_MODULE_MODULE_INCLUDE_DIRS}                                   )
target_link_libraries     ( render2D_module PRIVATE  ${KGL_VK_RENDER_MODULE_MODULE_LIBRARIES}                                      )