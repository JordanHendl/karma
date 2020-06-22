set( SAISEI_GRAPHICS_HEADERS
      Manager.h
      StateManager.h
   )

set( SAISEI_GRAPHICS_SOURCES
      Manager.cpp
      StateManager.cpp
   )

set( SAISEI_GRAPHICS_INCLUDES
     ${SAISEI_GRAPHICS_DIR}
     ${SAISEI_CONFIG_DIR}
     ${SAISEI_IO_DIR}
   )

set( SAISEI_GRAPHICS_LIBRARIES
     casper
    )

add_library               ( saisei_manager SHARED  ${SAISEI_MANAGER_SOURCES} ${SAISEI_MANAGER_HEADERS} )
target_include_directories( saisei_manager PRIVATE ${SAISEI_MANAGER_INCLUDES}                          )
target_link_libraries     ( saisei_manager PRIVATE ${SAISEI_MANAGER_LIBRARIES}                         )
