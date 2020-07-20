#include "Game.h"
#include "../../kgl/KGL_Interface.h"
#include <string>

namespace karma
{
  struct GameData
  {
    ::KGL_Interface interface ;
    std::string name ;
    int val ;
  };

  Game::Game()
  {
    this->game_data = new GameData() ;
  }

  Game::~Game()
  {
    delete this->game_data ;
  }
  
  void Game::initialize( const char* base_path )
  {
    data().interface.initialize( base_path ) ;
  }

  bool Game::run()
  {

    
    data().interface.loadPack ( "/usr/local/karma/active/files/test.krender" ) ;
            
//    data().interface.drawImage( img ) ;
    
    return 0 ;
  }

  GameData& Game::data()
  {
    return *this->game_data ;
  }

  const GameData& Game::data() const
  {
    return *this->game_data ;
  }
}