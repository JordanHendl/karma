set( KARMA_CONFIG_HEADERS
      Configuration.h
      Parser.h
   )

set( KARMA_CONFIG_SOURCES
      Configuration.cpp
      Parser.cpp
   )

set( KARMA_CONFIG_INCLUDES 
     ${DATA_BUS_DIR}
     ${CONFIGURATION_DIR}
   )

set( KARMA_CONFIG_LIBRARIES 
     data_bus
   )

add_library               ( configuration SHARED  ${KARMA_CONFIG_SOURCES} ${KARMA_CONFIG_HEADERS} )
target_include_directories( configuration PRIVATE ${KARMA_CONFIG_INCLUDES}                        ) 
target_link_libraries     ( configuration PRIVATE ${KARMA_CONFIG_LIBRARIES}                       ) 

add_executable       ( config_test test.cpp      )
target_link_libraries( config_test configuration )