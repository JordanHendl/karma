
set( KAL_DATABASE_SOURCES 
     Database.cpp
   )

set( KAL_DATABASE_HEADERS
     Database.h
   )

set( KAL_DATABASE_INCLUDES
     ${DATA_BUS_DIR}
     ${CONFIGURATION_DIR}
   )

set( KAL_DATABASE_LIBRARIES
     karma_data_bus
     karma_configuration
     karma_log
   )

add_library               ( kal_database SHARED  ${KAL_DATABASE_SOURCES}  ${KAL_DATABASE_HEADERS} )
target_link_libraries     ( kal_database PUBLIC ${KAL_DATABASE_LIBRARIES}                         )
target_include_directories( kal_database PRIVATE ${KAL_DATABASE_INCLUDES}                         )

INSTALL( TARGETS kal_database EXPORT kgl_lib DESTINATION ${KARMA_LIB_INSTALL_DIR} )
EXPORT ( PACKAGE kal_database                                                     )
INSTALL( TARGETS kal_database DESTINATION ${KARMA_LIB_INSTALL_DIR}                )