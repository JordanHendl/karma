#include "Manager.h"
#include "StateManager.h"
#include <graphics/GraphicsManager.h>
#include <io/InputManager.h>
#include <io/IO.h>
#include <config/Configuration.h>
#include <config/Parser.h>
#include <config/ArgParser.h>


const unsigned BUS_ID = 0 ;

namespace ss
{
  namespace man
  {
    struct ManagerData
    {
      /**
       */
      ss::graphics::Manager graphics ;
      
      /**
       */
      ss::io::InputManager input ;

      /**
       */
      ss::io::IOManager io ;

      /**
       */
      ss::man::StateManager state_manager ;

      /**
       */
      ss::config::Configuration config ;
    };

    GameManager::GameManager()
    {
      this->man_data = new ManagerData() ;      
    }

    GameManager::~GameManager()
    {
      delete this->man_data ;
    }

    void GameManager::initialize( int num_arg, const char** str_buff )
    {
      ss::config::ArgParser parser ;

      parser.parse( num_arg, str_buff ) ;
      
      data().graphics     .subscribe( BUS_ID ) ;
      data().input        .subscribe( BUS_ID ) ;
      data().config       .subscribe( BUS_ID ) ;
      data().state_manager.subscribe( BUS_ID ) ;

      data().config       .initialize( parser.path() ) ;

      data().graphics     .initialize() ;
      data().input        .initialize() ;
      data().state_manager.initialize() ;
    }

    int GameManager::start()
    {
      data().graphics     .kick() ;
      data().input        .kick() ;
      data().state_manager.kick() ;
    }

    ManagerData& GameManager::data()
    {
      return *this->man_data ;
    }

    const ManagerData& GameManager::data() const
    {
      return *this->man_data ;
    }
  }
}
