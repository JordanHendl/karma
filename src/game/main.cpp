#include "application/Game.h"
#include <iostream>

int main( int argc, char** argv )
{
  ::karma::Game game ;
  game.initialize() ;
  return game.run() ;
  return -1 ;
}