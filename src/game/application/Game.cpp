#include "Game.h"
#include "../../kgl/KGL_Interface.h"
#include "../../kgl/cmd/DrawCommand.h"
#include "../../kgl/io/Event.h"
#include "../../data/Bus.h"
#include "../../data/Signal.h"
#include "../../profiling/Timer.h"
#include "../../log/Log.h"
#include <string>

namespace karma
{
  struct GameData
  {
    ::kgl::SheetCommand cmd       ;
    ::kgl::ImageCommand img       ;
    unsigned            sprite    ;
    karma::ms::Timer    timer     ;
    ::KGL_Interface     interface ;
    ::data::module::Bus bus       ;
    bool                running   ;
    std::string         base_path ;
    float xpos ;
    float ypos ;
    float rot  ;
    
    float xpos_2 ;
    float ypos_2 ;
    float rot_2  ;
    bool pause = false ;

    void readInputs( const ::kgl::io::Event& event ) ;
    
    GameData()
    {
      this->running = false   ;
      this->xpos    = 0.0f    ;
      this->ypos    = 0.0f    ;
      this->rot     = 0.0f    ;
      this->xpos_2  = 200.0f  ;
      this->ypos_2  = 200.0f  ;
      this->rot_2   = 0.0f    ;
      this->sprite  = 0       ;
    }
  };
  
  void GameData::readInputs( const ::kgl::io::Event& event )
  {
    const float delta = 10.0f ;
    
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
        case ::kgl::io::Event::IOCode::Space :
          this->pause = !this->pause ;
          break ;
        case ::kgl::io::Event::IOCode::Left :
          this->xpos -= delta ;
          break ;
        case ::kgl::io::Event::IOCode::Right :
          this->xpos += delta ;
          break ;
        case ::kgl::io::Event::IOCode::Up :
          this->ypos -= delta ;
          break ;
        case ::kgl::io::Event::IOCode::Down :
          this->ypos += delta ;
          break ;
        case ::kgl::io::Event::IOCode::Z :
          this->rot += delta ;
          break ;

        case ::kgl::io::Event::IOCode::A :
          this->xpos_2 -= delta ;
          break ;
        case ::kgl::io::Event::IOCode::D :
          this->xpos_2 += delta ;
          break ;
        case ::kgl::io::Event::IOCode::W :
          this->ypos_2 -= delta ;
          break ;
        case ::kgl::io::Event::IOCode::S :
          this->ypos_2 += delta ;
          break ;
        case ::kgl::io::Event::IOCode::X :
          this->rot_2 -= delta ;
          break ;
          
        case ::kgl::io::Event::IOCode::Tab :
          this->sprite++ ;
          break ;
          
        default: break ;
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
    data().cmd.setWidth ( 32     ) ;
    data().cmd.setHeight( 32     ) ;
    data().base_path = base_path ;
  }

  bool Game::run()
  {
    data().running = true ;
    
    data().interface.loadPack ( (data().base_path + "krender/test.krender").c_str() ) ;
    
    data().timer.initialize( "FRAME_TIME" ) ;
    while( data().running )
    {
      data().timer.start() ;
      
      if( !data().pause )
      {
        data().img.setImage   ( "test"        ) ;
        data().img.setPosX    ( data().xpos   ) ;
        data().img.setPosY    ( data().ypos   ) ;
        data().img.setRotation( data().rot    ) ;


        data().cmd.setSheet   ( "background"  ) ;
        data().cmd.setPosX    ( data().xpos_2 ) ;
        data().cmd.setPosY    ( data().ypos_2 ) ;
        data().cmd.setRotation( data().rot_2  ) ;
        data().cmd.setIndex   ( data().sprite ) ;

        data().bus( "sprite::cmd" ).emit( data().cmd ) ;
        data().bus( "image::cmd"  ).emit( data().img ) ;
      
        data().interface.start() ;
        
        data().timer.stop() ;
        
        karma::log::Log::output( data().timer.output(), " in fps: ", 1000.0 / ( data().timer.time() * 60.0 ) ) ;
      }
      data().interface.pollEvents() ;
    }
    
    data().interface.shutdown() ;
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
