
set( KGL_DATABASE_SOURCES 
     Database.cpp
   )

set( KGL_DATABASE_HEADERS
     Database.h
   )

set( KGL_DATABASE_INCLUDES
     ${DATA_BUS_DIR}
     ${CONFIGURATION_DIR}
   )

set( KGL_DATABASE_LIBRARIES
     kgl_managers
     karma_data_bus
     karma_configuration
     karma_log
   )

add_library               ( kgl_database SHARED  ${KGL_DATABASE_SOURCES}  ${KGL_DATABASE_HEADERS} )
target_link_libraries     ( kgl_database PRIVATE ${KGL_DATABASE_LIBRARIES}                        )
target_include_directories( kgl_database PRIVATE ${KGL_DATABASE_INCLUDES}                         )