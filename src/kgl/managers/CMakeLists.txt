set( KGL_MANAGERS_SOURCES 
     AssetManager.cpp
     CommandManager.cpp
   )

set( KGL_MANAGERS_HEADERS
     AssetManager.h
     CommandManager.h
   )

set( KGL_MANAGERS_INCLUDES
     ${DATA_BUS_DIR}
     ${CONFIGURATION_DIR}
     ${Vulkan_INCLUDE_DIR}
   )

set( KGL_MANAGERS_LIBRARIES
     karma_configuration
     karma_data_bus
     karma_log
     kgl_vk_context
     kgl_io
     ${Vulkan_LIBRARIES}
   )

add_library               ( kgl_managers SHARED  ${KGL_MANAGERS_SOURCES}  ${KGL_MANAGERS_HEADERS} )
target_link_libraries     ( kgl_managers PRIVATE ${KGL_MANAGERS_LIBRARIES}                        )
target_include_directories( kgl_managers PRIVATE ${KGL_MANAGERS_INCLUDES}                         )
