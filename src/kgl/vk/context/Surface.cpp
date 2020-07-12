#include "Surface.h"
#include "Device.h"
#include "Swapchain.h"
#include "Context.h"
#include "Device.h"
#include "Semaphore.h"
#include <Pool.h>
#include <PoolData.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>
#include <vulkan/vulkan.hpp>
#include <stdio.h>
#include <chrono>
#include <thread>

namespace kgl
{
  namespace vk
  {
    struct SurfaceData
    {
      SDL_Window*         window        ;
      ::vk::SurfaceKHR    surface       ;
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

    Surface::Surface( const Surface& surface )
    {
      this->surf_data = new SurfaceData() ;
      *this->surf_data = *surface.surf_data ;
    }

    void Surface::operator=( const Surface& surface )
    {
      *this->surf_data = *surface.surf_data ;
    }
    
    void Surface::initialize( const SDL_Window& window )
    {
      const auto instance = ::kgl::vk::instance() ;
      data().window = const_cast<SDL_Window*>( &window ) ;
      if( SDL_Vulkan_CreateSurface( data().window , instance, reinterpret_cast<VkSurfaceKHR*>( &data().surface ) ) != SDL_TRUE )
      {
        printf(" ERROR MAKING SURFACE \n" ) ;
      }
    }

    const ::vk::SurfaceKHR Surface::surface() const 
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

//    void Surface::present( const VkPresentInfoKHR& present_info )
//    {
      //vkQueuePresentKHR( manager.device( name() ).present(), &present_info ) ;
      // std::this_thread::sleep_for(std::chrono::milliseconds(2000));

      //vkQueueWaitIdle( manager.device( name() ).present() ) ;
//    }

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