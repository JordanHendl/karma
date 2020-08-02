#include "Game.h"
#include "../../kgl/KGL_Interface.h"
#include "../../kgl/io/Event.h"
#include "../../data/Bus.h"
#include "../../data/Signal.h"
#include <string>

namespace karma
{
  struct GameData
  {
    ::KGL_Interface     interface ;
    ::data::module::Bus bus       ;
    bool                running   ;
    
    void quit( const ::kgl::io::Event& event )
    {
      if( event.type() == ::kgl::io::Event::Type::Quit )
      {
        this->running = false ;
      }
      
      if( event.type() == ::kgl::io::Event::Type::KeyDown )
      {
        switch( event.code() )
        {
          case ::kgl::io::Event::IOCode::Escape :
              this->running = false ;
            break ;
        }
      }
    }
    
    GameData()
    {
      this->running = false ;
    }
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
    
    data().bus( "kgl::input" ).attach( this->game_data, &GameData::quit ) ;
  }

  bool Game::run()
  {
    data().running = true ;
    
    data().interface.loadPack ( "/usr/local/karma/active/krender/test.krender" ) ;

    while( data().running )
    {
      data().interface.pollEvents() ;
      
      data().bus( "graph1::image" ).emit<const char*>( "test" ) ;
      data().bus( "graph1::xpos"  ).emit( 0.0f   ) ;
      data().bus( "graph1::ypos"  ).emit( 0.0f   ) ;
    }
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