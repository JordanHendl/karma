
set( DATABASE_SOURCES 
Database.cpp
)

set( DATABASE_HEADERS
  Database.cpp
)

set( DATABASE_INCLUDES
  ${DATA_BUS_DIR}
  ${CONFIGURATION_DIR}
)

set( DATABASE_LIBRARIES
  configuration
  data_bus
)

add_library               ( database SHARED ${DATABASE_SOURCES} ${DATABASE_HEADERS} )
target_link_libraries     ( database PRIVATE ${DATABASE_LIBRARIES}                  )
target_include_directories( database PRIVATE ${DATABASE_INCLUDES}                   )