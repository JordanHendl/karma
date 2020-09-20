#define VULKAN_HPP_NO_EXCEPTIONS
#define VULKAN_HPP_ASSERT
#define assert 
#include "Window.h"
#include "Context.h"
#include "Device.h"
#include "Swapchain.h"
#include "Library.h"
#include "Semaphore.h"
#include "Surface.h"
#include "Synchronization.h"
#include "CommandBuffer.h"
#include "profiling/Timer.h"
#include "kgl/containers/Layered.h"
#include <Bus.h>
#include <Pool.h>
#include <PoolData.h>
#include <Signal.h>
#include <log/Log.h>
#include <vulkan/vulkan.hpp>
#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_vulkan.h>
#include <string>
#include <stdexcept>
#include <iostream>
#include <vector>
#include <array>
#include <queue>
#include <chrono>
#include <mutex>
#include <thread>
#include <map>

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
      typedef std::vector<::vk::Fence>                                Fences ;
      typedef kgl::containers::Layered<::kgl::vk::Synchronization, 3> Syncs  ;
      
      std::map<unsigned, karma::ms::Timer> frame_timers ;
      SDL_Window*                window         ; ///< JHTODO
      Fences                     fences         ; ///< TODO
      Syncs                      syncs          ;
      Syncs                      acquire_sem    ;
      Surface                    surface        ;
      ::vk::PresentInfoKHR       info           ;
      Device                     device         ;
      SwapChain                  chain          ;
      WindowConf                 conf           ;
      bool                       shown          ; ///< JHTODO
      bool                       borderless     ; ///< JHTODO
      bool                       focus          ; ///< JHTODO
      std::string                title          ; ///< JHTODO
      std::string                name           ; ///< JHTODO
      unsigned                   width          ; ///< JHTODO
      unsigned                   height         ; ///< JHTODO
      bool                       fullscreen     ; ///< JHTODO
      unsigned                   current_img    ;
      unsigned                   current_frame  ;
      std::queue<::vk::Fence>    fence_queue    ;
      ::vk::Fence                acquire_fence  ;
      std::queue<unsigned>       images         ;
      bool                       swapchain_bad  ;
      unsigned                   frames_to_skip ;
      unsigned                   counter        ;
      unsigned                   num_frames     ;
      std::mutex                 img_mutex      ;
      std::mutex                 fence_mutex    ;
      bool                       debug          ;
      
      WindowData() ;
      void setDebug( bool debug ) ;
      WindowData& operator=( const WindowData& data ) ;
    };

    WindowData::WindowData()
    {
      this->counter        = 0     ;
      this->window         = NULL  ;
      this->shown          = false ;
      this->borderless     = false ;
      this->focus          = false ;
      this->title          = ""    ;
      this->name           = ""    ;
      this->width          = 1280  ;
      this->height         = 1024  ;
      this->fullscreen     = false ;
      this->current_frame  = 0     ;
      this->swapchain_bad  = false ;
      this->frames_to_skip = 0     ;
    }
    
    WindowData& WindowData::operator=( const WindowData& data )
    {
      this->window        = data.window        ;
      this->fences        = data.fences        ;
      this->syncs         = data.syncs         ;
      this->acquire_sem   = data.acquire_sem   ;
      this->surface       = data.surface       ;
      this->info          = data.info          ;
      this->device        = data.device        ;
      this->chain         = data.chain         ;
      this->conf          = data.conf          ;
      this->shown         = data.shown         ;
      this->borderless    = data.borderless    ;
      this->focus         = data.focus         ;
      this->title         = data.title         ;
      this->name          = data.name          ;
      this->width         = data.width         ;
      this->height        = data.height        ;
      this->fullscreen    = data.fullscreen    ;
      this->current_img   = data.current_img   ;
      this->current_frame = data.current_frame ;
      this->fence_queue   = data.fence_queue   ;
      this->acquire_fence = data.acquire_fence ;
      
      return *this ;
    }

    void WindowData::setDebug( bool debug )
    {
      this->debug = debug ;
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
  
    void Window::initialize( const char* name, const char* display_name, unsigned gpu, unsigned width, unsigned height, bool resizable, bool borderless, unsigned num_sems )
    { 
      ::data::module::Bus       bus        ;
      ::vk::FenceCreateInfo     fence_info ;
      ::vk::SemaphoreCreateInfo sem_info   ;
      int                       flags      ;
      
      flags = SDL_WINDOW_VULKAN | SDL_WINDOW_SHOWN ;
      
      if( width == 0 && height == 0 && borderless )
      {
        flags |= SDL_WINDOW_FULLSCREEN_DESKTOP ;
      }
      else if( width == 0 && height == 0 )
      {
        flags |= SDL_WINDOW_FULLSCREEN ;
      }
      else if( resizable )
      {
        flags |= SDL_WINDOW_RESIZABLE ;
      }
      
      fence_info.setFlags( ::vk::FenceCreateFlagBits::eSignaled ) ;
      
      data().name = name ;
      
      karma::log::Log::output( "Creating window ", name, " with width: ", width, ", height: ", height, " on GPU ", gpu ) ;
      
      data().window = SDL_CreateWindow( name, 500, 500, width, height, flags ) ;
      
      this->setTitle( display_name ) ;
      data().surface              .initialize( *data().window                   ) ;
      data().device               .initialize( data().surface.surface(), gpu    ) ;
      data().chain                .initialize( data().surface, data().device    ) ;
      data().syncs      .seek( 0 ).initialize( data().device.device(), num_sems ) ;
      data().syncs      .seek( 1 ).initialize( data().device.device(), num_sems ) ;
      data().syncs      .seek( 2 ).initialize( data().device.device(), num_sems ) ;
      data().acquire_sem.seek( 0 ).initialize( data().device.device(), 1        ) ;
      data().acquire_sem.seek( 1 ).initialize( data().device.device(), 1        ) ;
      data().acquire_sem.seek( 2 ).initialize( data().device.device(), 1        ) ;

      data().num_frames = data().chain.numBuffers() ;

      data().fences.resize( data().conf.max_frames_in_flight ) ;
      
      for( auto &fence : data().fences )
      {
        fence = data().device.device().createFence( fence_info, nullptr ).value ;
      }
      
      data().acquire_fence = data().device.device().createFence( fence_info, nullptr ).value ;
      
      data().info.setWaitSemaphoreCount( 1 ) ;
      data().info.setSwapchainCount    ( 1 ) ;
      
      bus( name, "::debug"   ).attach( this->win_data, &WindowData::setDebug ) ;
      bus( name, "::present" ).attach( this          , &Window::present      ) ;
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
      ::data::module::Bus bus ;
      while( SDL_PollEvent( &event ) )
      {
        bus( "key_input" ).emit( event ) ;
      }
    }

    void Window::setFullscreen( bool val )
    {
      data().fullscreen = val ;
      SDL_SetWindowFullscreen( data().window, SDL_WINDOW_FULLSCREEN_DESKTOP ) ;
    }

    const Device& Window::device() const
    {
      return data().device ;
    }

    SwapChain& Window::chain()
    {
      return data().chain ;
    }
    
    void Window::reset()
    {
      if( data().window ) SDL_DestroyWindow( data().window ) ;  
    }

    const ::vk::SurfaceKHR Window::surface() const
    {
      return data().surface.surface() ;
    }
    
    unsigned Window::currentSwap() const
    {
      return data().current_img ;
    }
    
    static const std::vector<::vk::PipelineStageFlags> flags( 100, ::vk::PipelineStageFlagBits::eAllCommands ) ;
      
    void Window::present( const Synchronization& sync )
    {

      if( data().debug )
      {
        data().frame_timers[ data().counter ].stop() ;
        karma::log::Log::output( "KGL Frame ", data().counter, " Render Time: ", data().frame_timers[ data().counter ].time(), "ms | ", 1000.f / data().frame_timers[ data().counter ].time(), " FPS. " ) ;
        data().counter++ ;
        data().counter %= data().num_frames ;
      }
      
      const ::vk::Queue        queue = data().device.graphics() ;
      const ::vk::SwapchainKHR chain = data().chain.chain()     ;
      ::vk::SubmitInfo info  ;
      ::vk::Fence      fence ;
      unsigned         img   ;

      fence = sync.fence() ;
      
      data().fence_mutex.lock() ;
      data().fence_queue.push( fence ) ;
      data().fence_mutex.unlock() ;
      
      if( data().swapchain_bad )
      {
        data().device.device().waitIdle() ;
        info.setWaitSemaphoreCount( sync.numSignalSems() ) ;
        info.setPWaitSemaphores   ( sync.signalSems()    ) ;
        info.setPWaitDstStageMask(  flags.data()         ) ;
        
        queueMutex().lock() ;
        queue.submit( 1, &info, ::vk::Fence() ) ;
        queueMutex().unlock() ;
        data().device.device().waitIdle() ;
      }
      else
      {
        data().img_mutex.lock() ;
        img = data().images.front() ;
        data().images.pop() ;
        data().img_mutex.unlock() ;
        
        data().info.setPImageIndices     ( &img                 ) ;
        data().info.setSwapchainCount    ( 1                    ) ;
        data().info.setPSwapchains       ( &chain               ) ;
        data().info.setWaitSemaphoreCount( sync.numSignalSems() ) ;
        data().info.setPWaitSemaphores   ( sync.signalSems()    ) ;
  
        queueMutex().lock() ;
        queue.presentKHR( &data().info ) ;
        queueMutex().unlock() ;
      }  
      
      data().current_frame-- ;
    }
    
    void Window::start()
    {
      auto              wait_sem = data().acquire_sem.value().signalSem( 0 ) ;
      const ::vk::Queue queue    = data().device.graphics()                  ;
      const ::vk::Fence dummy                                                ;
      ::vk::Fence         fence ;
      ::data::module::Bus bus   ;
      ::vk::SubmitInfo    info  ;
      
      if( data().current_frame < data().chain.numBuffers() - 1 )
        {
        if( data().fence_queue.size() >= data().chain.numBuffers() )
        {
          data().fence_mutex.lock() ;
          fence = data().fence_queue.front() ;
          data().fence_queue.pop() ;
          data().fence_mutex.unlock() ;

          data().device.device().waitForFences( 1, &fence, true, UINT64_MAX ) ;
        }
        
        while( this->data().swapchain_bad ) {} ;
        auto result = data().device.device().acquireNextImageKHR( data().chain.chain(), UINT64_MAX, wait_sem, dummy ) ;
        
        if( result.result == ::vk::Result::eErrorOutOfDateKHR || result.result == ::vk::Result::eSuboptimalKHR )
        {
          this->data().swapchain_bad = true ;
          while( data().current_frame > 1 ) {} ;
          data().device.device().waitIdle() ;
          while( !data().images.empty() ) data().images.pop() ;
          data().chain.reset() ;
          data().chain.initialize( data().surface, data().device ) ;
          
          bus( this->data().name.c_str(), "::resize" ).emit( data().chain.width(), data().chain.height() ) ;
          this->data().swapchain_bad = false ;
          return ;
        }
        
        info.setWaitSemaphoreCount  ( 1                                          ) ;
        info.setPWaitSemaphores     ( &wait_sem                                  ) ;
        info.setSignalSemaphoreCount( data().syncs.value().numSignalSems()       ) ;
        info.setPSignalSemaphores   ( data().syncs.value().signalSems()          ) ;
        info.setPWaitDstStageMask   ( flags.data()                               ) ;
        
        queueMutex().lock() ;
        queue.submit( 1, &info, nullptr ) ;
        queueMutex().unlock() ;

        data().img_mutex.lock() ;
        data().images.push( result.value ) ;
        data().img_mutex.unlock() ;

        data().current_img = result.value ;
        data().current_frame++ ;
        
        if( data().debug ) data().frame_timers[ data().current_img ].start() ;
        bus( "current_swap" ).emit( data().current_img   ) ;
        bus( "start"        ).emit( data().syncs.value() ) ;
        
        data().syncs      .swap() ;
        data().acquire_sem.swap() ;
    }
//      while( data().current_frame >= 2 ) { std::this_thread::sleep_for( std::chrono::milliseconds( 5 ) ) ; } ;
    }
    
    void Window::setBorderless( bool val )
    {
      data().borderless = val ;
      SDL_SetWindowBordered( data().window, val ? SDL_TRUE : SDL_FALSE ) ;
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
}  
  