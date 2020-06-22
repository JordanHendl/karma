#include "casper_library.h"
#include <casper_vulkan.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>

namespace casper
{
  namespace library
  {

    static casper::Vulkan vulkan ;

    struct CasperLibrary
    {
      RENDERING_API current_api ;
      bool          initialized ;
      
      CasperLibrary()
      {
        this->current_api = CSP_API_NONE ;
        this->initialized = false    ;
      }
    };
    
    static struct CasperLibrary lib ;
    
    
    void setRenderingAPI( casper::library::RENDERING_API api )
    {
      lib.current_api = api ;
    }
    
    RENDERING_API renderingApi()
    {
      return lib.current_api ;
    }
    
    bool renderingInitialized()
    {
      return lib.initialized ;
    }

    void initializeAPI()
    { 
      if( !lib.initialized )
      {
        if( lib.current_api == CSP_API_VULKAN )
        {
          if( !vulkan.initialized() ) 
          {
            vulkan.initialize() ;
          }
        }
        
        lib.initialized = true ;
      }
    }
  }
}