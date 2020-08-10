set( KARMA_GAME_SOURCES
     Game.cpp
   )
     
set( KARMA_GAME_HEADERS
     Game.h
   )

set( KARMA_GAME_INCLUDE_DIRS
      ${DATA_BUS_DIR}
    )

set( KARMA_GAME_LIBRARIES
     kgl
     kgl_io
     karma_data_bus
   )

add_library               ( karma_game SHARED   ${KARMA_GAME_SOURCES} ${KARMA_GAME_HEADERS} )
target_include_directories( karma_game PRIVATE  ${KARMA_GAME_INCLUDE_DIRS}                  )
target_link_libraries     ( karma_game PRIVATE  ${KARMA_GAME_LIBRARIES}                     )
