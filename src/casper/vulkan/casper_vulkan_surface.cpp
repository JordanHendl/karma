#include "casper_vulkan_surface.h"
#include "casper_vulkan_device.h"
#include "casper_vulkan_swapchain.h"
#include "casper_vulkan_device.h"
#include "casper_vulkan_semaphore.h"
#include <Pool.h>
#include <PoolData.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>
#include <vulkan/vulkan.h>
#include <stdio.h>
    #include <chrono>
    #include <thread>

namespace casper
{
  namespace vulkan
  {
    struct SurfaceData
    {
      SDL_Window*         window        ;
      VkSurfaceKHR        surface       ;
      VkQueue             present_queue ;
      data::module::Pool  pool          ;
      bool                found_queue   ;

      SurfaceData()
      {
        found_queue = false ;
      }
    };

    Surface::Surface()
    {
      this->surf_data = new SurfaceData() ;
    }

    void Surface::initialize( const SDL_Window& window )
    {
      data().window = const_cast<SDL_Window*>( &window ) ;

      if( SDL_Vulkan_CreateSurface( data().window , data().pool[ "CASPER_VULKAN_INSTANCE" ].get<VkInstance>(), &data().surface ) != SDL_TRUE )
      {
        printf(" ERROR MAKING SURFACE \n" ) ;
      }
    }

    VkSurfaceKHR Surface::surface() const 
    {
      return data().surface ;
    }

    const SDL_Window* Surface::window() const
    {
      return data().window ;
    }

    unsigned Surface::width() const
    {
      int w ;
      int h ;

      SDL_GetWindowSize( data().window, &w, &h ) ;
      return w ;
    }

    unsigned Surface::height() const
    {
      int w ;
      int h ;

      SDL_GetWindowSize( data().window, &w, &h ) ;
      return h ;
    }

    void Surface::present( const VkPresentInfoKHR& present_info )
    {
      //vkQueuePresentKHR( manager.device( name() ).present(), &present_info ) ;
      // std::this_thread::sleep_for(std::chrono::milliseconds(2000));

      //vkQueueWaitIdle( manager.device( name() ).present() ) ;
    }

    const char* Surface::name() const
    {
      return SDL_GetWindowTitle( data().window ) ;
    }

    SurfaceData& Surface::data()
    {
      return *this->surf_data ;
    }

    const SurfaceData& Surface::data() const
    {
      return *this->surf_data ;
    }

    Surface::~Surface()
    {
      delete this->surf_data ;
    }

  }
}