#include "application/Game.h"
#include <iostream>

int main( int argc, char** argv )
{
  ::karma::Game game ;
  if( argc > 1 )
  {
    game.initialize( argv[ 1 ] ) ;
    return game.run() ;
  }
  std::cout << "Usage: " << argv[ 0 ] << " <game_install_path> " << std::endl  ;
  
  return -1 ;
}