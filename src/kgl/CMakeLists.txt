set( CASPER_VULKAN_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vulkan )

find_package( X11                )
find_package( PNG                )
find_package( Threads            )
find_package( Vulkan REQUIRED    )
find_package( SDL2   REQUIRED    )

set( KGL_SOURCES 
     KGL_Interface.cpp
     DrawCommand.cpp
     Setup.cpp
   )
      
set( KGL_HEADERS
     KGL_Interface.h
     DrawCommand.h
     Setup.h
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
     data_bus
     kgl_vk_context
     kgl_vk_module
     kgl_managers
     kgl_database
     configuration
    )

INCLUDE_DIRECTORIES( ${KGL_DIR} )
ADD_SUBDIRECTORY   ( database   )
ADD_SUBDIRECTORY   ( io         )
ADD_SUBDIRECTORY   ( managers   )
ADD_SUBDIRECTORY   ( vk         )
ADD_SUBDIRECTORY   ( library    )

ADD_LIBRARY               ( kgl SHARED  ${KGL_SOURCES} ${KGL_HEADERS} )
TARGET_LINK_LIBRARIES     ( kgl PRIVATE ${KGL_LIBRARIES}              )
TARGET_INCLUDE_DIRECTORIES( kgl PRIVATE ${KGL_INCLUDE_DIRS}           )
