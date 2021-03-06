
set( KGL_VK_COMBINER_MODULE_SOURCES 
     Combiner.cpp
   )
     
set( KGL_VK_COMBINER_MODULE_HEADERS
     Combiner.h
   )

set( KGL_VK_COMBINER_MODULE_INCLUDE_DIRS
      ${DATA_BUS_DIR}
      ${KGL_NODE_DIR}
      ${Vulkan_INCLUDE_DIRS}
    )

set( KGL_VK_COMBINER_MODULE_LIBRARIES
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

set( KGL_VK_COMBINER_MODULE_DEPS
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

add_library               ( combiner SHARED   ${KGL_VK_COMBINER_MODULE_SOURCES} ${KGL_VK_COMBINER_MODULE_HEADERS} )
target_include_directories( combiner PRIVATE  ${KGL_VK_COMBINER_MODULE_INCLUDE_DIRS}                              )
target_link_libraries     ( combiner PUBLIC  ${KGL_VK_COMBINER_MODULE_LIBRARIES}                                 )
ADD_DEPENDENCIES          ( combiner          ${KGL_VK_COMBINER_MODULE_DEPS}                                      )

INSTALL( TARGETS combiner     DESTINATION ${KARMA_MODULE_INSTALL_DIR} )