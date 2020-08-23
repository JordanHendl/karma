
set( KGL_VK_INSTANCED_SPRITESHEET_MODULE_SOURCES 
     InstancedSpriteSheet.cpp
   )
     
set( KGL_VK_INSTANCED_SPRITESHEET_MODULE_HEADERS
     InstancedSpriteSheet.h
   )

set( KGL_VK_INSTANCED_SPRITESHEET_MODULE_INCLUDE_DIRS
      ${DATA_BUS_DIR}
      ${KGL_NODE_DIR}
      ${Vulkan_INCLUDE_DIRS}
    )

set( KGL_VK_INSTANCED_SPRITESHEET_MODULE_LIBRARIES
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

set( CMAKE_SHARED_LIBRARY_SUFFIX "${CMAKE_SHARED_LIBRARY_SUFFIX}.1" )

add_library               ( instanced_spritesheet SHARED   ${KGL_VK_INSTANCED_SPRITESHEET_MODULE_SOURCES} ${KGL_VK_INSTANCED_SPRITESHEET_MODULE_HEADERS} )
target_include_directories( instanced_spritesheet PRIVATE  ${KGL_VK_INSTANCED_SPRITESHEET_MODULE_INCLUDE_DIRS}                                           )
target_link_libraries     ( instanced_spritesheet PRIVATE  ${KGL_VK_INSTANCED_SPRITESHEET_MODULE_LIBRARIES} kgl_command                                  )
