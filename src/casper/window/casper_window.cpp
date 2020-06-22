#include "casper_window.h"
#include "casper_library.h"
#include "casper_vulkan.h"
#include <Bus.h>
#include <Signal.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_vulkan.h>
#include <string>
#include <stdexcept>
#include <vector>

namespace casper
{
  
  struct WindowData
  {
    data::module::Bus   bus        ; ///< JHTODO
    SDL_Window*         window     ; ///< JHTODO
    bool                shown      ; ///< JHTODO
    bool                borderless ; ///< JHTODO
    bool                focus      ; ///< JHTODO
    std::string         title      ; ///< JHTODO
    std::string         name       ; ///< JHTODO
    unsigned            width      ; ///< JHTODO
    unsigned            height     ; ///< JHTODO
    bool                fullscreen ; ///< JHTODO
    
    WindowData()
    {
      this->window     = NULL  ;
      this->shown      = false ;
      this->borderless = false ;
      this->focus      = false ;
      this->title      = ""    ;
      this->name       = ""    ;
      this->width      = 1280  ;
      this->height     = 1024  ;
      this->fullscreen = false ;
    }
  };
  
  Window::Window()
  { 
    this->win_data = new WindowData() ;
  }

  Window::~Window()
  { 
    delete this->win_data ;
  }

  void Window::initialize( const char* name, unsigned gpu,  unsigned width, unsigned height )
  { 
    data().window = SDL_CreateWindow( name, 500, 500, width, height, SDL_WINDOW_VULKAN | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE ) ;

    data().bus[ "SDL_WINDOW" ].emit< const SDL_Window*>( gpu, data().window ) ;
  }
  
  bool Window::isInitialized() const
  {
    return data().window != NULL ;
  }

  void Window::setTitle( const char* title )
  { 
    data().title = title ;
    SDL_SetWindowTitle( data().window, title ) ;
  }

  void Window::setSize( unsigned width, unsigned height )
  { 
    data().width  = width  ;
    data().height = height ;
    SDL_SetWindowSize( data().window, width, height ) ;
  }
  
  void Window::pollEvents()
  {
    SDL_Event event ;
    while( SDL_PollEvent( &event ) )
    {
      ;
    }
  }
  void Window::setFullscreen( bool val )
  {
    data().fullscreen = val ;
    SDL_SetWindowFullscreen( data().window, SDL_WINDOW_FULLSCREEN_DESKTOP ) ;
  }

  void Window::reset()
  {
    if( data().window ) SDL_DestroyWindow( data().window ) ;  
  }
  
  void Window::setName( const char* name )
  {
    data().name = name ;
  }
  
  void Window::setBorderless( bool val )
  {
    data().borderless = val ;
    SDL_SetWindowBordered( data().window, val ? SDL_TRUE : SDL_FALSE ) ;
  }
  
  void Window::subscribe( unsigned id )
  {
    // register setters with publishers.
  }
  
  WindowData& Window::data()
  { 
    return *this->win_data ;
  }

  const WindowData& Window::data() const
  { 
    return *this->win_data ;
  }

}
