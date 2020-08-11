
set( DATA_BUS_SOURCES 
      Bus.cpp 
      Signal.cpp
      Pool.cpp
      PoolData.cpp
   )
      
set( DATA_BUS_HEADERS
      Bus.h
      Signal.h
      Pool.h
      PoolData.h
   )

set( DATA_BUS_INCLUDES
   )

set( DATA_BUS_LIBRARIES
    )

add_library( karma_data_bus SHARED ${DATA_BUS_SOURCES} ${DATA_BUS_HEADERS} )

add_executable       ( karma_data_bus_test main.cpp          )
target_link_libraries( karma_data_bus_test PRIVATE  karma_data_bus )

