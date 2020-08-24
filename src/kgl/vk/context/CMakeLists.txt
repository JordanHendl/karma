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
     Synchronization.cpp
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
     Synchronization.h
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
     karma_data_bus
     karma_uwushader
     karma_log
   )

SET( KGL_VK_DEPS
     karma_data_bus
     karma_uwushader
     karma_log
   )

ADD_LIBRARY               ( kgl_vk_context SHARED   ${KGL_VK_SOURCES} ${KGL_VK_HEADERS} )
TARGET_INCLUDE_DIRECTORIES( kgl_vk_context PRIVATE  ${KGL_VK_INCLUDE_DIRS}              )
TARGET_LINK_LIBRARIES     ( kgl_vk_context PUBLIC  ${KGL_VK_LIBRARIES}                 )
ADD_DEPENDENCIES          ( kgl_vk_context          ${KGL_VK_DEPS}                      )

INSTALL( TARGETS kgl_vk_context EXPORT kgl_lib DESTINATION ${KARMA_LIB_INSTALL_DIR} )
INSTALL( TARGETS kgl_vk_context DESTINATION ${KARMA_LIB_INSTALL_DIR}                )