#include "Event.h"
#include <SDL2/SDL.h>
#include <array>
#include <limits.h>

constexpr static auto typeConversion()
{
  std::array<::kgl::io::Event::Type, SDL_LASTEVENT> ret {{}} ;
  
  ret[ SDL_QUIT    ] = ::kgl::io::Event::Type::Quit    ;
  ret[ SDL_KEYDOWN ] = ::kgl::io::Event::Type::KeyDown ;
  
  return ret ;
}

constexpr static auto codeConversion()
{
  std::array<::kgl::io::Event::IOCode, 1024> ret {{}} ;
  
  ret[ SDLK_ESCAPE ] = ::kgl::io::Event::IOCode::Escape ;
  ret[ SDLK_RETURN ] = ::kgl::io::Event::IOCode::Enter  ;
  
  return ret ;
}
constexpr static auto modConversion()
{
  std::array<::kgl::io::Event::Modifier, UINT16_MAX> ret {{}} ;
  
  ret[ KMOD_LSHIFT ] = ::kgl::io::Event::Modifier::LeftShift  ;
  ret[ KMOD_RSHIFT ] = ::kgl::io::Event::Modifier::RightShift ;
  
  return ret ;
}

::kgl::io::Event& operator<<( ::kgl::io::Event& event, SDL_Event& sdl_event )
{
  static constexpr auto type_conversion_table = typeConversion() ;
  static constexpr auto code_conversion_table = codeConversion() ;
  static constexpr auto mod_conversion_table  = modConversion()  ;
  
  event.event_type = type_conversion_table[ sdl_event.type           ] ;
  event.key_code   = code_conversion_table[ sdl_event.key.keysym.sym ] ;
  event.modifier   = mod_conversion_table [ sdl_event.key.keysym.mod ] ;
  
  return event ;
}

namespace kgl
{
  namespace io
  {
    Event::Event()
    {
      this->event_type = Event::Type::None     ;
      this->key_code   = Event::IOCode::None   ;
      this->modifier   = Event::Modifier::None ;
    }
    
    Event::Type Event::type() const
    {
      return this->event_type ;
    }

    Event::IOCode Event::code() const
    {
      return this->key_code ;
    }

    Event::Modifier Event::mod() const
    {
      return this->modifier ;
    }
  }
}
