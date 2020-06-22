#include "casper_vulkan_surface_manager.h"
#include "casper_vulkan_surface.h"
#include <Bus.h>
#include <Signal.h>
#include <vulkan/vulkan.h>
#include <map>
#include <SDL2/SDL.h>
#include <string>
#include <memory>
namespace casper
{
  namespace vulkan
  {
    typedef std::unique_ptr<Surface>        pSurface   ;
    typedef std::map<std::string, pSurface> SurfaceMap ;
    struct SurfaceManagerStage
    {
      SurfaceManagerStage() ;
    };

    static data::module::Bus   bus ;
    static SurfaceMap          map ;

    static void addSurface( unsigned gpu, const SDL_Window* window )
    {
      std::string   title   ;

      title = std::string( SDL_GetWindowTitle( const_cast<SDL_Window*>( window ) ) ) ;
      map.insert( { title, std::make_unique<Surface>() } ) ;

      map.at( title )->initialize( gpu, *window ) ;
    }

    static void removeSurface( const char* name )
    {
      map.erase( name ) ;
    }

    static SurfaceManagerStage stage ;

    SurfaceManagerStage::SurfaceManagerStage()
    {
      bus[ "SDL_WINDOW"         ].attach( &addSurface    ) ;
      bus[ "SDL_WINDOW_DESTROY" ].attach( &removeSurface ) ;
    }
    
    SurfaceManager::SurfaceManager()
    {

    }

    SurfaceManager::~SurfaceManager()
    {

    }

    Surface dummy ;

    Surface& SurfaceManager::surface( const char* name )
    {
      if( map.find( name ) != map.end() )
      {
        return *map.at( std::string( name ) ) ;
      }
      else
      {
        return dummy ;
      }
    }
  }
}