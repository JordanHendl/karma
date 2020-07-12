find_package( X11                )
find_package( PNG                )
find_package( Threads            )
find_package( Vulkan REQUIRED    )
find_package( SDL2 REQUIRED      )

set( KGL_VK_SOURCES 
     Context.cpp
     CommandBuffer.cpp
     Device.cpp
     Instance.cpp
     Library.cpp
     Uniform.cpp
     Descriptor.cpp
     PipelineConfig.cpp
     Shader.cpp
     RenderPass.cpp
     Pipeline.cpp
     Uniform.cpp
     Semaphore.cpp
     Swapchain.cpp
     Surface.cpp
     Validation.cpp
     Window.cpp
   )
     
set( KGL_VK_HEADERS
     Context.h
     CommandBuffer.h
     RenderPass.h
     Device.h
     Uniform.h
     Descriptor.h
     PipelineConfig.h
     Shader.h
     Pipeline.h
     Uniform.h
     Instance.h
     Library.h
     Semaphore.h
     Surface.h
     Swapchain.h
     Validation.h
     Window.h
   )

set( KGL_VK_INCLUDE_DIRS
      ${SDL2_INCLUDE_DIRS}
      ${Vulkan_INCLUDE_DIRS}
      ${TOOLS_DIR}
      ${DATA_BUS_DIR}
    )

set( KGL_VK_LIBRARIES
     ${SDL2_LIBRARIES}
     ${Vulkan_LIBRARIES}
     data_bus
     glsl_compiler
   )

add_library               ( kgl_vk_context SHARED   ${KGL_VK_SOURCES} ${KGL_VK_HEADERS} )
target_include_directories( kgl_vk_context PRIVATE  ${KGL_VK_INCLUDE_DIRS}              )
target_link_libraries     ( kgl_vk_context PRIVATE  ${KGL_VK_LIBRARIES}                 )