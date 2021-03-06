
set( KGL_VK_SPRITESHEET_MODULE_SOURCES 
     SpriteSheet.cpp
   )
     
set( KGL_VK_SPRITESHEET_MODULE_HEADERS
     SpriteSheet.h
   )

set( KGL_VK_SPRITESHEET_MODULE_INCLUDE_DIRS
      ${DATA_BUS_DIR}
      ${KGL_NODE_DIR}
      ${Vulkan_INCLUDE_DIRS}
    )

set( KGL_VK_SPRITESHEET_MODULE_LIBRARIES
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

set( KGL_VK_SPRITESHEET_MODULE_DEPS
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

add_library               ( spritesheet SHARED   ${KGL_VK_SPRITESHEET_MODULE_SOURCES} ${KGL_VK_SPRITESHEET_MODULE_HEADERS} )
target_include_directories( spritesheet PRIVATE  ${KGL_VK_SPRITESHEET_MODULE_INCLUDE_DIRS}                                 )
target_link_libraries     ( spritesheet PUBLIC  ${KGL_VK_SPRITESHEET_MODULE_LIBRARIES} kgl_command                        )
ADD_DEPENDENCIES( spritesheet ${KGL_VK_SPRITESHEET_MODULE_DEPS} ) 

INSTALL( TARGETS spritesheet     DESTINATION ${KARMA_MODULE_INSTALL_DIR} )
