set( KGL_VK_PRESENT__MODULE_SOURCES 
     Present.cpp
   )
     
set( KGL_VK_PRESENT__MODULE_HEADERS
     Present.h
   )

set( KGL_VK_PRESENT__MODULE_INCLUDE_DIRS
      ${DATA_BUS_DIR}
      ${KGL_NODE_DIR}
      ${Vulkan_INCLUDE_DIR}
    )

set( KGL_VK_PRESENT__MODULE_LIBRARIES
     kgl_containers
     kgl_vk_module
     kgl_vk_context
     kgl_command
     kgl
     karma_data_bus
     karma_log
     karma_profiling
     ${Vulkan_LIBRARIES}
   )

set( KGL_VK_PRESENT__MODULE_DEPS
     kgl_containers
     kgl_vk_module
     kgl_vk_context
     kgl_command
     kgl
     karma_data_bus
     karma_log
     karma_profiling
   )

set( CMAKE_SHARED_LIBRARY_SUFFIX "${CMAKE_SHARED_LIBRARY_SUFFIX}.1" )

add_library               ( present SHARED   ${KGL_VK_PRESENT__MODULE_SOURCES} ${KGL_VK_PRESENT__MODULE_HEADERS} )
target_include_directories( present PRIVATE  ${KGL_VK_PRESENT__MODULE_INCLUDE_DIRS}                              )
target_link_libraries     ( present PUBLIC  ${KGL_VK_PRESENT__MODULE_LIBRARIES}                                 )
ADD_DEPENDENCIES          ( present          ${KGL_VK_PRESENT__MODULE_DEPS}                                      )

INSTALL( TARGETS present     DESTINATION ${KARMA_MODULE_INSTALL_DIR} )
