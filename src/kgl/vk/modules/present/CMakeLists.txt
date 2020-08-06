set( KGL_VK_PRESENT__MODULE_SOURCES 
     Present.cpp
   )
     
set( KGL_VK_PRESENT__MODULE_HEADERS
     Present.h
   )

set( KGL_VK_PRESENT__MODULE_INCLUDE_DIRS
      ${DATA_BUS_DIR}
      ${KGL_NODE_DIR}
    )

set( KGL_VK_PRESENT__MODULE_LIBRARIES
     data_bus
     kgl_vk_module
     kgl
   )

add_library               ( present SHARED   ${KGL_VK_PRESENT__MODULE_SOURCES} ${KGL_VK_PRESENT__MODULE_HEADERS} )
target_include_directories( present PRIVATE  ${KGL_VK_PRESENT__MODULE_INCLUDE_DIRS}                              )
target_link_libraries     ( present PRIVATE  ${KGL_VK_PRESENT__MODULE_LIBRARIES}                                 )