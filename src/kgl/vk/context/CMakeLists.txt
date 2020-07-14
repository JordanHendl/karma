FIND_PACKAGE( X11                )
FIND_PACKAGE( PNG                )
FIND_PACKAGE( Threads            )
FIND_PACKAGE( Vulkan REQUIRED    )
FIND_PACKAGE( SDL2 REQUIRED      )

SET( KGL_VK_SOURCES 
     Context.cpp
     CommandBuffer.cpp
     Buffer.cpp
     Device.cpp
     Instance.cpp
     Library.cpp
     Uniform.cpp
     Descriptor.cpp
     PipelineConfig.cpp
     Image.cpp
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
     
SET( KGL_VK_HEADERS
     Context.h
     CommandBuffer.h
     Buffer.h
     RenderPass.h
     Device.h
     Uniform.h
     Image.h
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

SET( KGL_VK_INCLUDE_DIRS
      ${SDL2_INCLUDE_DIRS}
      ${Vulkan_INCLUDE_DIRS}
      ${TOOLS_DIR}
      ${DATA_BUS_DIR}
    )

SET( KGL_VK_LIBRARIES
     ${SDL2_LIBRARIES}
     ${Vulkan_LIBRARIES}
     data_bus
     glsl_compiler
   )

ADD_LIBRARY               ( kgl_vk_context SHARED   ${KGL_VK_SOURCES} ${KGL_VK_HEADERS} )
TARGET_INCLUDE_DIRECTORIES( kgl_vk_context PRIVATE  ${KGL_VK_INCLUDE_DIRS}              )
TARGET_LINK_LIBRARIES     ( kgl_vk_context PRIVATE  ${KGL_VK_LIBRARIES}                 )