set( DRIVER_SOURCES
     main.cpp
   )
     
set( DRIVER_HEADERS
   )

set( DRIVER_INCLUDE_DIRS

   )

set( DRIVER_LIBRARIES
     karma_game
   )

add_subdirectory( application ) 
add_executable            ( game_driver ${DRIVER_SOURCES} ${DRIVER_HEADERS} )
target_include_directories( game_driver PRIVATE  ${DRIVER_INCLUDE_DIRS}     )
target_link_libraries     ( game_driver PRIVATE  ${DRIVER_LIBRARIES}        )