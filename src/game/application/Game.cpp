#include "Game.h"
#include "../../kgl/KGL_Interface.h"
#include "../../kal/KAL_Interface.h"
#include "../../kgl/cmd/DrawCommand.h"
#include "../../kgl/io/Event.h"
#include "../../data/Bus.h"
#include "../../config/Configuration.h"
#include "../../config/Parser.h"
#include "../../data/Signal.h"
#include "../../profiling/Timer.h"
#include "../../log/Log.h"
#include "../../kgl/containers/List.h"
#include "../../kgl/animation/Animation2D.h"
#include <vector>
#include <string>
#include <sstream>
#include <queue>
#include <map>
namespace karma
{
  struct GameData
  {
    ::karma::config::Configuration config ;
    
    ::kgl::List<::kgl::SheetCommand> cmds ;
    std::map<std::string, ::kgl::anim::Sprite2D> animations ;
    std::string curr_dir ;
    ::kgl::anim::Sprite2D curr_anim ;
    ::kgl::SheetCommand cmd           ;
    ::kgl::ImageCommand img           ;
    ::kgl::TextCommand  txt           ;
    ::kgl::Camera       camera        ;
    unsigned            sprite        ;
    karma::ms::Timer    timer         ;
    ::KGL_Interface     interface     ;
    ::KAL_Interface     kal_interface ;
    ::data::module::Bus bus           ;
    bool                running       ;
    std::string         base_path     ;
    
    unsigned width  ;
    unsigned height ;
    float xpos ;
    float ypos ;
    float rot  ;
    
    float xpos_2 ;
    float ypos_2 ;
    float zpos_2 ;
    float rot_2  ;
    bool pause = false ;

    void readInputs( const ::kgl::io::Event& event ) ;
    void loadMap() ;
    GameData()
    {
      this->running = false ;
      this->xpos    = 0.0f  ;
      this->ypos    = 0.0f  ;
      this->rot     = 0.0f  ;
      this->xpos_2  = 0.0f  ;
      this->ypos_2  = 0.0f  ;
      this->zpos_2  = 0.0f  ;
      this->rot_2   = 0.0f  ;
      this->sprite  = 0     ;
    }
  };
  
  void GameData::readInputs( const ::kgl::io::Event& event )
  {
    const float delta = 1.0f ;
    
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
          if( this->curr_dir != "LEFT" ) 
          {
            this->curr_dir = "LEFT" ;
            this->curr_anim.pause() ;
            this->curr_anim.reset() ;
            this->curr_anim = this->animations[ "fl_walk" ] ;
            this->curr_anim.start() ;
          }
          
          this->xpos -= delta  ;
          break ;
        case ::kgl::io::Event::IOCode::Right :
          this->xpos += delta  ;
          break ;
        case ::kgl::io::Event::IOCode::Up :
          if( this->curr_dir != "BACK" ) 
          {
            this->curr_dir = "BACK" ;
            this->curr_anim.pause() ;
            this->curr_anim.reset() ;
            this->curr_anim = this->animations[ "back_walk" ] ;
            this->curr_anim.start() ;
          }
          
          this->ypos -= delta ;
          break ;
        case ::kgl::io::Event::IOCode::Down :
          
        if( this->curr_dir != "FRONT" ) 
          {
            this->curr_dir = "FRONT" ;
            this->curr_anim.pause() ;
            this->curr_anim.reset() ;
            this->curr_anim = this->animations[ "front_walk" ] ;
            this->curr_anim.start() ;
          }
          this->ypos += delta ;
          break ;
        case ::kgl::io::Event::IOCode::Z :
          this->rot += delta ;
          break ;

        case ::kgl::io::Event::IOCode::Q :
          this->zpos_2 -= delta ;
          break ;
        case ::kgl::io::Event::IOCode::E :
          this->zpos_2 += delta ;
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
        case ::kgl::io::Event::IOCode::One :
          this->bus( "kal::music" ).emit<const char*>( 0, "pso" ) ;
          break ;
        case ::kgl::io::Event::IOCode::Two :
          this->bus( "kal::sound" ).emit<const char*>( 0, "magic" ) ;
          break ;
          
        case ::kgl::io::Event::IOCode::Tab :
          this->sprite++ ;
          break ;
          
        default: break ;
      }
    } 
 }

      
  void GameData::loadMap()
  {
    std::string key    ;
    std::vector<unsigned> tiles ;
    std::string path = ( this->base_path + "data/maps/test_map.json" ) ;
    this->config.initialize( path.c_str(), 0 ) ;
    
    for( auto entry = config.begin(); entry != config.end(); ++entry )
    {
      key = entry.key() ;
      
      if( key == "layers::data"   ) 
      {
        for( unsigned i = 0; i < entry.size(); i++ )
        {
          tiles.push_back( entry.number( i ) ) ;
        }
      }
      if( key == "layers::height" ) this->height = entry.number() ;
      if( key == "layers::width"  ) this->width  = entry.number() ;
    }
    
    ::kgl::SheetCommand cmd ;
    cmd.setSheet( "background" ) ;
    unsigned index ;
    
    index = 0 ;
    cmd.setWidth ( 32 ) ;
    cmd.setHeight( 32 ) ;
    
    this->cmds.initialize( this->width * this->height ) ;
    
    float off = 0 ;
    for( unsigned y = 0; y < this->height; y++ )
    {
      cmd.setPosY( (float) ( y * 8 ) ) ;
      off = y * 16 ;
      for( unsigned x = 0; x < this->width; x++ )
      {
        cmd.setPosX( (float) ( off + x * 16      ) ) ;
        cmd.setPosY( (float) cmd.posY() - ( 8 )    ) ;
        cmd.setIndex( tiles[ index ] - 1           ) ;
        this->cmds[ ( width - x ) + ( y * this->width ) ] = cmd ;
        index++ ;
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
  
  void Game::initialize()
  {
    data().interface.initialize() ;
    data().kal_interface.initialize() ;
    data().interface.setCurrentWindow( "Main" ) ;
    data().bus( "kgl::input" ).attach( this->game_data, &GameData::readInputs ) ;
    data().cmd.setWidth ( 32     ) ;
    data().cmd.setHeight( 32     ) ;
  }

  bool Game::run()
  {
    const unsigned SZ = 50 ;
    float fpss[ SZ ] ;
    
    unsigned index ;
    std::stringstream str ;
    data().running = true ;
    
    data().interface.loadPack( "krender/test.krender" ) ;
    data().loadMap() ;
    
    data().txt.setFont( "test_font" ) ;
    

    data().animations[ "fl_idle" ].initialize( "grandma", 0, 0, 500.f ) ;
    data().animations[ "fl_idle" ].insertAnimation( 0 ) ;
    
    data().animations[ "front_idle" ].initialize( "grandma", 0, 0, 500.f ) ;
    data().animations[ "front_idle" ].insertAnimation( 40 ) ;
    
    data().animations[ "back_idle" ].initialize( "grandma", 0, 0, 500.f ) ;
    data().animations[ "back_idle" ].insertAnimation( 80 ) ;
    
    data().animations[ "back_idle" ].initialize( "grandma", 0, 0, 500.f ) ;
    data().animations[ "back_idle" ].insertAnimation( 80 ) ;
    
    data().animations[ "bl_idle" ].initialize( "grandma", 0, 0, 500.f ) ;
    data().animations[ "bl_idle" ].insertAnimation( 120 ) ;
    
    data().animations[ "fl_walk" ].initialize( "grandma", 0, 0, 500.f ) ;
    data().animations[ "fl_walk" ].insertAnimation( 1 ) ;
    data().animations[ "fl_walk" ].insertAnimation( 2 ) ;
    
    data().animations[ "front_walk" ].initialize( "grandma", 0, 0, 500.f ) ;
    data().animations[ "front_walk" ].insertAnimation( 41 ) ;
    data().animations[ "front_walk" ].insertAnimation( 42 ) ;
    
    data().animations[ "back_walk" ].initialize( "grandma", 0, 0, 500.f ) ;
    data().animations[ "back_walk" ].insertAnimation( 81 ) ;
    data().animations[ "back_walk" ].insertAnimation( 82 ) ;
    
    data().animations[ "bl_walk" ].initialize( "grandma", 0, 0, 500.f ) ;
    data().animations[ "bl_walk" ].insertAnimation( 121 ) ;
    data().animations[ "bl_walk" ].insertAnimation( 122 ) ;
    
    data().curr_anim = data().animations[ "front_idle" ] ;
    data().bus( "instance_test::cmd" ).emit( data().cmds ) ;
    data().timer.initialize( "FRAME_TIME" ) ;
    
    index = 0 ;
    while( data().running )
    {
      data().timer.start() ;
      
      if( !data().pause )
      {
        float avg ;

        avg = 0 ;
        str = std::stringstream() ;

        for( unsigned i = 0; i < SZ; i++ ) avg += fpss[ i ] ;
        avg = avg / (static_cast<float>( SZ ) ) ;
        str.precision( 3 ) ;
        str << avg << " fps" ;
        
        data().camera.setPos( data().xpos_2, data().ypos_2, data().zpos_2 ) ;
        
        data().img.setImage   ( "test"            ) ;
        data().img.setPosX    ( data().xpos       ) ;
        data().img.setPosY    ( data().ypos       ) ;
        data().img.setRotation( data().rot        ) ;
        data().txt.setPosX    ( 0                 ) ;
        data().txt.setPosY    ( 5                 ) ;
        data().txt.setWidth   ( 0.25              ) ;
        data().txt.setHeight  ( 0.25              ) ;
        data().txt.setText    ( str.str().c_str() ) ;
        data().txt.setColorR  ( 1.0f              ) ;
        data().txt.setColorG  ( 1.0f              ) ;
        data().txt.setColorB  ( 1.0f              ) ;
        data().txt.setColorA  ( 0.5f              ) ;
        
        for( unsigned i = 0; i < 1600; i++ ) data().bus( "image::cmd"            ).emit( data().curr_anim.current( data().xpos, data().ypos, 0, data().rot ) ) ;
        data().bus( "text::cmd"             ).emit( data().txt    ) ;
        data().bus( "instance_test::camera" ).emit( data().camera ) ;
        data().bus( "image::camera"         ).emit( data().camera ) ;
        data().interface.start() ;
        
        data().timer.stop() ;
        fpss[ index % SZ ] = 1000.f / data().timer.time() ;
        karma::log::Log::output( data().timer.output(), "ms. In fps: ", fpss[ index % SZ ] ) ;
        
        index++ ;
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
