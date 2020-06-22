#include "casper.h"
#include "window/casper_library.h"
#include <SDL2/SDL.h>
// things
namespace casper
{
  
  struct CasperData
  {
    
  };
  
  void casperInit()
  {
    SDL_Init( SDL_INIT_VIDEO ) ;
    casper::library::setRenderingAPI( casper::library::CSP_API_VULKAN ) ;
  }
  
  void casperShutdown()
  {
    SDL_Quit() ;
  }
}