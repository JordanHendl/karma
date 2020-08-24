set( SAISEI_MANAGER_HEADERS
      Manager.h
      StateManager.h
   )

set( SAISEI_MANAGER_SOURCES
      Manager.cpp
      StateManager.cpp
   )

set( SAISEI_MANAGER_INCLUDES
     ${SAISEI_GRAPHICS_DIR}
     ${SAISEI_CONFIG_DIR}
     ${SAISEI_IO_DIR}
   )

set( SAISEI_MANAGER_LIBRARIES
     saise_graphics
     saise_config
     saise_io
    )

add_library               ( saisei_manager SHARED  ${SAISEI_MANAGER_SOURCES} ${SAISEI_MANAGER_HEADERS} )
target_include_directories( saisei_manager PRIVATE ${SAISEI_MANAGER_INCLUDES}                          )
target_link_libraries     ( saisei_manager PUBLIC ${SAISEI_MANAGER_LIBRARIES}                         )
