#include "Window.h"
#include "Context.h"
#include "Device.h"
#include "Swapchain.h"
#include "Library.h"
#include "Semaphore.h"
#include "Surface.h"
#include "../node/Synchronization.h"
#include <Bus.h>
#include <Pool.h>
#include <PoolData.h>
#include <Signal.h>
#include <vulkan/vulkan.hpp>
#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_vulkan.h>
#include <string>
#include <stdexcept>
#include <iostream>
#include <vector>

namespace kgl
{
  namespace vk
  {
    struct WindowInformation
    {
      
    };

    struct WindowConf
    {
      bool        shown                ; ///< JHTODO
      bool        borderless           ; ///< JHTODO
      bool        focus                ; ///< JHTODO
      std::string title                ; ///< JHTODO
      std::string name                 ; ///< JHTODO
      unsigned    width                ; ///< JHTODO
      unsigned    height               ; ///< JHTODO
      bool        fullscreen           ; ///< JHTODO
      unsigned    max_frames_in_flight ;

      WindowConf()
      {
        this->max_frames_in_flight = 2 ;
      }
    };

    struct WindowData
    {
      typedef std::vector<::vk::Fence> Fences ;
      
      SDL_Window*                window        ; ///< JHTODO
      Fences                     fences        ; ///< TODO
      Surface                    surface       ;
      ::vk::PresentInfoKHR       info          ;
      ::kgl::vk::Synchronization sync          ;
      Device                     device        ;
      SwapChain                  chain         ;
      WindowConf                 conf          ;
      bool                       shown         ; ///< JHTODO
      bool                       borderless    ; ///< JHTODO
      bool                       focus         ; ///< JHTODO
      std::string                title         ; ///< JHTODO
      std::string                name          ; ///< JHTODO
      unsigned                   width         ; ///< JHTODO
      unsigned                   height        ; ///< JHTODO
      bool                       fullscreen    ; ///< JHTODO
      unsigned                   current_img   ;
      unsigned                   current_frame ;

      WindowData() ;

      void generateRenderPass() ;
      void createFramebuffers() ;
    };

    WindowData::WindowData()
    {
      this->window        = NULL  ;
      this->shown         = false ;
      this->borderless    = false ;
      this->focus         = false ;
      this->title         = ""    ;
      this->name          = ""    ;
      this->width         = 1280  ;
      this->height        = 1024  ;
      this->fullscreen    = false ;
      this->current_frame = 0     ;
    }

    Window::Window()
    { 
      this->win_data = new WindowData() ;
    }
  
    Window::Window( const Window& window )
    {
      this->win_data = new WindowData() ;
      
      *this->win_data = *window.win_data ;
    }

    void Window::operator=( const Window& window )
    {
      *this->win_data = *window.win_data ;
    }
    
    Window::~Window()
    { 
      delete this->win_data ;
    }
  
    void Window::initialize( const char* name, unsigned gpu,  unsigned width, unsigned height )
    { 
      ::data::module::Bus bus ;
      ::vk::FenceCreateInfo     fence_info ;
      
      fence_info.setFlags( ::vk::FenceCreateFlagBits::eSignaled ) ;

      data().window = SDL_CreateWindow( name, 500, 500, width, height, SDL_WINDOW_VULKAN | SDL_WINDOW_SHOWN ) ;
  
      data().surface.initialize( *data().window                ) ;
      data().device .initialize( data().surface.surface(), gpu ) ;
      data().chain  .initialize( data().surface, data().device ) ;
      data().sync   .initialize( data().device.device()        ) ;

      data().fences.resize( data().conf.max_frames_in_flight ) ;
      
      for( auto &fence : data().fences )
      {
        fence = data().device.device().createFence( fence_info, nullptr ) ;
      }

      data().info.setWaitSemaphoreCount( 1 ) ;
      data().info.setSwapchainCount    ( 1 ) ;
      
      bus( name, "::present" ).attach( this, &Window::present ) ;
    }
    
    bool Window::isInitialized() const
    {
      return data().window != NULL ;
    }
  
    //void Window::setTitle( const char* title )
    //{ 
    //  data().title = title ;
    //  SDL_SetWindowTitle( data().window, title ) ;
    //}
  
//    void Window::setSize( unsigned width, unsigned height )
//    { 
//      data().width  = width  ;
//      data().height = height ;
//      SDL_SetWindowSize( data().window, width, height ) ;
//    }

     void Window::pollEvents()
    {
      SDL_Event event ;
      ::data::module::Bus bus ;
      while( SDL_PollEvent( &event ) )
      {
        bus( "key_input" ).emit( event ) ;
      }
    }

    //void Window::setFullscreen( bool val )
    //{
    //  data().fullscreen = val ;
    //  SDL_SetWindowFullscreen( data().window, SDL_WINDOW_FULLSCREEN_DESKTOP ) ;
    //}

    const Device& Window::device() const
    {
      return data().device ;
    }

    SwapChain& Window::chain()
    {
      return data().chain ;
    }
    //void Window::reset()
    //{
    //  if( data().window ) SDL_DestroyWindow( data().window ) ;  
    //}

    const ::vk::SurfaceKHR Window::surface() const
    {
      return data().surface.surface() ;
    }
    
    unsigned Window::currentSwap() const
    {
      return data().current_img ;
    }
    
    void Window::present( const Synchronization& sync )
    {
      const ::vk::Queue        queue = data().device.graphics() ;
      const ::vk::SwapchainKHR chain = data().chain.chain()     ;
      ::data::module::Bus bus ;
      
      // Swap the input signal sems into this object's wait sems.
      data().sync.copy( sync ) ;

      data().info.setPImageIndices     ( &data().current_img       ) ;
      data().info.setSwapchainCount    ( 1                         ) ;
      data().info.setPSwapchains       ( &chain                    ) ;
      data().info.setWaitSemaphoreCount( data().sync.numWaitSems() ) ;
      data().info.setPWaitSemaphores   ( data().sync.waitSems()    ) ;

      queue.presentKHR( &data().info ) ;
      queue.waitIdle() ;
      data().current_frame-- ;
    }
    
    void Window::start()
    {
      const ::vk::Fence fence = data().fences.at( data().current_frame ) ;
      ::data::module::Bus bus   ;
      
      if( data().current_frame == 0 )
      {
        data().device.device().waitForFences( 1, &fence, true, UINT64_MAX ) ;
        data().device.device().resetFences  ( 1, &fence                   ) ;
        auto result = data().device.device().acquireNextImageKHR( data().chain.chain(), UINT64_MAX, data().sync.signalSem( 0 ), fence ) ;
        
        data().current_img = result.value ;
        data().current_frame++ ;
//        data().current_frame = data().current_frame + 1 % ( data().conf.max_frames_in_flight - 1 ) ;
        bus( "start" ).emit( data().sync ) ;
      }

      while( data().current_frame != 0 ) {} ;
    }
//    void Window::setName( const char* name )
//    {
//      data().name = name ;
//    }
    
//    void Window::setBorderless( bool val )
//    {
//      data().borderless = val ;
//      SDL_SetWindowBordered( data().window, val ? SDL_TRUE : SDL_FALSE ) ;
//    }
    
    WindowData& Window::data()
    { 
      return *this->win_data ;
    }
  
    const WindowData& Window::data() const
    { 
      return *this->win_data ;
    }
  }
}  
  