
set( KGL_VK_TEXT2D_MODULE_SOURCES 
     Text2D.cpp
   )
     
set( KGL_VK_TEXT2D_MODULE_HEADERS
     Text2D.h
   )

set( KGL_VK_TEXT2D_MODULE_INCLUDE_DIRS
      ${DATA_BUS_DIR}
      ${KGL_NODE_DIR}
      ${Vulkan_INCLUDE_DIRS}
    )

set( KGL_VK_TEXT2D_MODULE_LIBRARIES
     kgl
     kgl_containers
     kgl_vk_module
     kgl_vk_context
     kgl_managers
     kgl_command
     karma_data_bus
     karma_profiling
     karma_log
     ${Vulkan_LIBRARIES}
   )

set( KGL_VK_TEXT2D_MODULE_DEPS
     kgl
     kgl_containers
     kgl_vk_module
     kgl_vk_context
     kgl_managers
     kgl_command
     karma_data_bus
     karma_profiling
     karma_log
   )

set( CMAKE_SHARED_LIBRARY_SUFFIX "${CMAKE_SHARED_LIBRARY_SUFFIX}.1" )

add_library               ( text2d SHARED   ${KGL_VK_TEXT2D_MODULE_SOURCES} ${KGL_VK_TEXT2D_MODULE_HEADERS} )
target_include_directories( text2d PRIVATE  ${KGL_VK_TEXT2D_MODULE_INCLUDE_DIRS}                            )
target_link_libraries     ( text2d PUBLIC  ${KGL_VK_TEXT2D_MODULE_LIBRARIES}                                )
ADD_DEPENDENCIES( text2d ${KGL_VK_TEXT2D_MODULE_DEPS} ) 

INSTALL( TARGETS text2d DESTINATION ${KARMA_MODULE_INSTALL_DIR} )
