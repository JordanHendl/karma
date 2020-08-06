#include "Input.h"
#include "Event.h"
#include <string>
#include <SDL2/SDL_events.h>
#include "../../data/Bus.h"
#include "../../data/Signal.h"

namespace kgl
{
  namespace io
  {
    static constexpr const char* io_emit_name = "kgl::input" ;
    
    struct InputData
    {
      std::string str ;
      ::data::module::Bus bus ;
    };

    Input::Input()
    {
      this->input_data = new InputData() ;
    }

    Input::~Input()
    {
      delete this->input_data ;
    }

    void Input::poll()
    {
      SDL_Event        e     ;
      ::kgl::io::Event event ;

      while( SDL_PollEvent( &e ) != 0 )
      {
        event = ::kgl::io::Event() ;
        
        event << e ;
        
        data().bus( io_emit_name ).emit( event ) ;
      }
    }

    InputData& Input::data()
    {
      return *this->input_data ;
    }

    const InputData& Input::data() const
    {
      return *this->input_data ;
    }
  }
}