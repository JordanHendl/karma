#include "Game.h"
#include "../../kgl/KGL_Interface.h"
#include "../../kgl/DrawCommand.h"
#include "../../kgl/io/Event.h"
#include "../../data/Bus.h"
#include "../../data/Signal.h"
#include <string>

namespace karma
{
  struct GameData
  {
    ::kgl::ImageCommand cmd       ;
    ::KGL_Interface     interface ;
    ::data::module::Bus bus       ;
    bool                running   ;
    
    float xpos ;
    float ypos ;
    float rot  ;

    void readInputs( const ::kgl::io::Event& event ) ;
    
    GameData()
    {
      this->running = false ;
    }
  };
  
  void GameData::readInputs( const ::kgl::io::Event& event )
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
          
        case ::kgl::io::Event::IOCode::Left :
          this->xpos -= 5.f ;
          break ;
        case ::kgl::io::Event::IOCode::Right :
          this->xpos += 5.f ;
          break ;
        case ::kgl::io::Event::IOCode::Up :
          this->ypos -= 5.f ;
          break ;
        case ::kgl::io::Event::IOCode::Down :
          this->ypos += 5.f ;
          break ;
        case ::kgl::io::Event::IOCode::Z :
          this->rot += 5.f ;
          break ;
      }
    }
  }

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
    data().interface.setCurrentWindow( "Main" ) ;
    data().bus( "kgl::input" ).attach( this->game_data, &GameData::readInputs ) ;
    data().cmd.setWidth ( 502     ) ;
    data().cmd.setHeight( 502     ) ;
  }

  bool Game::run()
  {
    data().running = true ;
    
    data().interface.loadPack ( "/usr/local/karma/active/krender/test.krender" ) ;

    
    while( data().running )
    {
      data().cmd.setImage ( "test"  ) ;
      data().cmd.setPosX    ( data().xpos ) ;
      data().cmd.setPosY    ( data().ypos ) ;
      data().cmd.setRotation( data().rot  ) ;
      
      data().bus( "graph1::cmd" ).emit( data().cmd ) ;
      
      data().cmd.setImage ( "test_2"  ) ;
      data().cmd.setPosX    ( data().xpos + 200 ) ;
      data().cmd.setPosY    ( data().ypos + 200 ) ;
      data().cmd.setRotation( data().rot  + 90  ) ;
      
      data().bus( "graph1::cmd" ).emit( data().cmd ) ;
      
      data().interface.start() ;
      data().interface.pollEvents() ;
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