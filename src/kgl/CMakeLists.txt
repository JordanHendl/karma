set( CASPER_VULKAN_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vulkan )

find_package( X11                )
find_package( PNG                )
find_package( Threads            )
find_package( Vulkan REQUIRED    )
find_package( SDL2   REQUIRED    )

set( KGL_SOURCES 
     Setup.cpp
     KGL_Interface.cpp
   )
      
set( KGL_HEADERS
     Setup.h
     KGL_Interface.h
   )

set( KGL_INCLUDE_DIRS
      ${SDL2_INCLUDE_DIRS}
      ${Vulkan_INCLUDE_DIRS}
      ${CMAKE_WORKING_DIRECTORY}/include
      ${DATA_BUS_DIR}
      ${CONFIGURATION_DIR}
   )

set( KGL_LIBRARIES
     ${SDL2_LIBRARIES}
     ${Vulkan_LIBRARIES}
     kgl_vk_context
     kgl_vk_module
     kgl_io
     kgl_managers
     kgl_database
     kgl_command
     karma_configuration
     karma_data_bus
     karma_log
    )

set( KGL_DEPS
     kgl_vk_context
     kgl_vk_module
     kgl_io
     kgl_managers
     kgl_database
     kgl_command
     karma_configuration
     karma_data_bus
     karma_log
    )

INCLUDE_DIRECTORIES( ${KGL_DIR} )
ADD_SUBDIRECTORY   ( database   )
ADD_SUBDIRECTORY   ( io         )
ADD_SUBDIRECTORY   ( managers   )
ADD_SUBDIRECTORY   ( vk         )
ADD_SUBDIRECTORY   ( library    )
ADD_SUBDIRECTORY   ( containers )
ADD_SUBDIRECTORY   ( animation  )
ADD_SUBDIRECTORY   ( cmd        )

ADD_LIBRARY               ( kgl SHARED  ${KGL_SOURCES} ${KGL_HEADERS} )
TARGET_LINK_LIBRARIES     ( kgl PUBLIC ${KGL_LIBRARIES}              )
TARGET_INCLUDE_DIRECTORIES( kgl PRIVATE ${KGL_INCLUDE_DIRS}           )
ADD_DEPENDENCIES          ( kgl ${KGL_DEPS} ) 
set( CMAKE_EXPORT_PACKAGE_REGISTRY ON )

INSTALL( TARGETS kgl EXPORT kgl_lib DESTINATION ${KARMA_LIB_INSTALL_DIR}          )
INSTALL( EXPORT kgl_lib DESTINATION ${KARMA_LIB_INSTALL_DIR} FILE KGLConfig.cmake )
INSTALL( FILES KGL_Interface.h DESTINATION ${KARMA_HEADER_INSTALL_DIR}            )