#include "Event.h"
#include <SDL2/SDL.h>
#include <array>
#include <limits.h>

constexpr static auto typeConversion()
{
  std::array<::kgl::io::Event::Type, SDL_LASTEVENT> ret {{}} ;
  
  ret[ SDL_QUIT    ] = ::kgl::io::Event::Type::Quit    ;
  ret[ SDL_KEYDOWN ] = ::kgl::io::Event::Type::KeyDown ;
  ret[ SDL_KEYUP   ] = ::kgl::io::Event::Type::KeyUp   ;
  
  return ret ;
}

constexpr static auto codeConversion( uint32_t sdl_key )
{
  switch( sdl_key ) 
  {
    case SDLK_a      : return ::kgl::io::Event::IOCode::A      ;
    case SDLK_b      : return ::kgl::io::Event::IOCode::B      ;
    case SDLK_c      : return ::kgl::io::Event::IOCode::C      ;
    case SDLK_d      : return ::kgl::io::Event::IOCode::D      ;
    case SDLK_e      : return ::kgl::io::Event::IOCode::E      ;
    case SDLK_f      : return ::kgl::io::Event::IOCode::F      ;
    case SDLK_g      : return ::kgl::io::Event::IOCode::G      ;
    case SDLK_h      : return ::kgl::io::Event::IOCode::H      ;
    case SDLK_i      : return ::kgl::io::Event::IOCode::I      ;
    case SDLK_j      : return ::kgl::io::Event::IOCode::J      ;
    case SDLK_k      : return ::kgl::io::Event::IOCode::K      ;
    case SDLK_l      : return ::kgl::io::Event::IOCode::L      ;
    case SDLK_m      : return ::kgl::io::Event::IOCode::M      ;
    case SDLK_n      : return ::kgl::io::Event::IOCode::N      ;
    case SDLK_o      : return ::kgl::io::Event::IOCode::O      ;
    case SDLK_p      : return ::kgl::io::Event::IOCode::P      ;
    case SDLK_q      : return ::kgl::io::Event::IOCode::Q      ;
    case SDLK_r      : return ::kgl::io::Event::IOCode::R      ;
    case SDLK_s      : return ::kgl::io::Event::IOCode::S      ;
    case SDLK_t      : return ::kgl::io::Event::IOCode::T      ;
    case SDLK_u      : return ::kgl::io::Event::IOCode::U      ;
    case SDLK_v      : return ::kgl::io::Event::IOCode::V      ;
    case SDLK_w      : return ::kgl::io::Event::IOCode::W      ;
    case SDLK_x      : return ::kgl::io::Event::IOCode::X      ;
    case SDLK_y      : return ::kgl::io::Event::IOCode::Y      ;
    case SDLK_z      : return ::kgl::io::Event::IOCode::Z      ;
    case SDLK_0      : return ::kgl::io::Event::IOCode::Zero   ;
    case SDLK_1      : return ::kgl::io::Event::IOCode::One    ;
    case SDLK_2      : return ::kgl::io::Event::IOCode::Two    ;
    case SDLK_3      : return ::kgl::io::Event::IOCode::Three  ;
    case SDLK_4      : return ::kgl::io::Event::IOCode::Four   ;
    case SDLK_5      : return ::kgl::io::Event::IOCode::Five   ;
    case SDLK_6      : return ::kgl::io::Event::IOCode::Six    ;
    case SDLK_7      : return ::kgl::io::Event::IOCode::Seven  ;
    case SDLK_8      : return ::kgl::io::Event::IOCode::Eight  ;
    case SDLK_9      : return ::kgl::io::Event::IOCode::Nine   ;
    case SDLK_ESCAPE : return ::kgl::io::Event::IOCode::Escape ;
    case SDLK_RETURN : return ::kgl::io::Event::IOCode::Enter  ;
    case SDLK_LEFT   : return ::kgl::io::Event::IOCode::Left   ;
    case SDLK_RIGHT  : return ::kgl::io::Event::IOCode::Right  ;
    case SDLK_UP     : return ::kgl::io::Event::IOCode::Up     ;
    case SDLK_DOWN   : return ::kgl::io::Event::IOCode::Down   ;
    case SDLK_SPACE  : return ::kgl::io::Event::IOCode::Space  ;
    default          : return ::kgl::io::Event::IOCode::None   ;
  }
  
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
  static constexpr auto mod_conversion_table  = modConversion()  ;
  
  event.event_type = type_conversion_table[ sdl_event.type           ] ;
  event.key_code   = codeConversion( sdl_event.key.keysym.sym        ) ;
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
