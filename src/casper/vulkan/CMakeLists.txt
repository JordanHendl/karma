find_package( X11                )
find_package( PNG                )
find_package( Threads            )
find_package( Vulkan REQUIRED    )
find_package( SDL2 REQUIRED      )

set( CASPER_VULKAN_SOURCES 
     casper_vulkan.cpp
     casper_vulkan_command_buffer.cpp
     casper_vulkan_device.cpp
     casper_vulkan_surface.cpp
     casper_vulkan_swapchain.cpp
     casper_vulkan_pipeline.cpp
     casper_vulkan_surface_manager.cpp
     casper_vulkan_2d_vertex.cpp
     casper_vulkan_validation_layer.cpp
     casper_vulkan_semaphore.cpp
     casper_vulkan_buffer.cpp
     casper_vulkan_error_handler.cpp
     casper_vulkan_descriptor_pool.cpp
   )
      
set( CASPER_VULKAN_HEADERS
     casper_vulkan.h
     casper_vulkan_command_buffer.h
     casper_vulkan_device.h
     casper_vulkan_surface.h
     casper_vulkan_swapchain.h
     casper_vulkan_pipeline.h
     casper_vulkan_surface_manager.h
     casper_vulkan_vertex.h
     casper_vulkan_2d_vertex.h
     casper_vulkan_validation_layer.h
     casper_vulkan_semaphore.h
     casper_vulkan_buffer.h
     casper_vulkan_error_handler.h
     casper_vulkan_descriptor_pool.h
   )

set( CASPER_VULKAN_INCLUDE_DIRS
      ${SDL2_INCLUDE_DIRS}
      ${Vulkan_INCLUDE_DIRS}
      ${CASPER_TOOLS_SHADER_DIR}
      ${DATA_BUS_DIR}
   )

set( CASPER_VULKAN_LIBRARIES
     ${SDL2_LIBRARIES}
     ${Vulkan_LIBRARIES}
     data_bus
     casper_shader
    )

add_library               ( casper_vulkan SHARED ${CASPER_VULKAN_SOURCES} ${CASPER_VULKAN_HEADERS} )
target_include_directories( casper_vulkan PRIVATE  ${CASPER_VULKAN_INCLUDE_DIRS}                   )
target_link_libraries     ( casper_vulkan PRIVATE  ${CASPER_VULKAN_LIBRARIES}                      )
