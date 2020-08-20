
#include "Window.h"
#include "Context.h"
#include "Device.h"
#include "Swapchain.h"
#include "Library.h"
#include "Semaphore.h"
#include "Surface.h"
#include "Synchronization.h"
#include "CommandBuffer.h"
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
#include <mutex>
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

    std::mutex                 fence_mutex   ;
    struct WindowData
    {
      typedef std::vector<::vk::Fence>                  Fences ;
      typedef std::array<::kgl::vk::Synchronization, 2> Syncs ;
      SDL_Window*                window        ; ///< JHTODO
      Fences                     fences        ; ///< TODO
      Syncs                      syncs         ;
      Surface                    surface       ;
      ::vk::PresentInfoKHR       info          ;
      ::vk::Semaphore            acquire_sem   ;
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
      unsigned                   sync_index    ;
      std::queue<::vk::Fence>    fence_queue   ;
      std::queue<unsigned>       images        ;

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
      this->sync_index    = 0     ;
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
  
    void Window::initialize( const char* name, unsigned gpu,  unsigned width, unsigned height, unsigned num_sems )
    { 
      ::data::module::Bus bus ;
      ::vk::FenceCreateInfo     fence_info ;
      ::vk::SemaphoreCreateInfo sem_info   ;
      fence_info.setFlags( ::vk::FenceCreateFlagBits::eSignaled ) ;
      
      karma::log::Log::output( "Creating window ", name, " with width: ", width, ", height: ", height, " on GPU ", gpu ) ;

      data().window = SDL_CreateWindow( name, 500, 500, width, height, SDL_WINDOW_VULKAN | SDL_WINDOW_SHOWN ) ;
      
      data().surface .initialize( *data().window                   ) ;
      data().device  .initialize( data().surface.surface(), gpu    ) ;
      data().chain   .initialize( data().surface, data().device    ) ;
      data().syncs[0].initialize( data().device.device(), num_sems ) ;
      data().syncs[1].initialize( data().device.device(), num_sems ) ;

      data().fences.resize( data().conf.max_frames_in_flight ) ;
      
      data().acquire_sem = data().device.device().createSemaphore( sem_info, nullptr ) ;

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
    
    static std::mutex img_mutex ;
    
    void Window::present( const Synchronization& sync )
    {
      const ::vk::Queue        queue = data().device.graphics() ;
      const ::vk::SwapchainKHR chain = data().chain.chain()     ;
      ::vk::Fence     fence ;
      unsigned        img   ;

      fence = sync.fence() ;
      
      fence_mutex.lock() ;
      data().fence_queue.push( fence ) ;
      fence_mutex.unlock() ;

      img_mutex.lock() ;
      img = data().images.front() ;
      data().images.pop() ;
      img_mutex.unlock() ;

      data().info.setPImageIndices     ( &img                 ) ;
      data().info.setSwapchainCount    ( 1                    ) ;
      data().info.setPSwapchains       ( &chain               ) ;
      data().info.setWaitSemaphoreCount( sync.numSignalSems() ) ;
      data().info.setPWaitSemaphores   ( sync.signalSems()    ) ;

      queueMutex().lock() ;
      queue.presentKHR( &data().info ) ;
      queueMutex().unlock() ;
      
      data().current_frame-- ;
    }
    
    void Window::start()
    {
      static const std::vector<::vk::PipelineStageFlags> flags( 100, ::vk::PipelineStageFlagBits::eAllCommands ) ;
      const ::vk::Queue queue = data().device.graphics() ;
      const unsigned    index = data().sync_index        ;
      const ::vk::Fence dummy                            ;
      ::vk::Fence         fence ;
      ::data::module::Bus bus   ;
      ::vk::SubmitInfo    info  ;
      
      if( data().current_frame < 2 )
      {
        if( data().fence_queue.size() != 0 )
        {
          fence_mutex.lock() ;
          fence = data().fence_queue.front() ;
          data().fence_queue.pop() ;
          fence_mutex.unlock() ;

          data().device.device().waitForFences(1, &fence, true, UINT64_MAX ) ;
        }
        
        auto result = data().device.device().acquireNextImageKHR( data().chain.chain(), 200, data().acquire_sem, dummy ) ;
        
        info.setWaitSemaphoreCount  ( 1                                     ) ;
        info.setPWaitSemaphores     ( &data().acquire_sem                   ) ;
        info.setSignalSemaphoreCount( data().syncs[ index ].numSignalSems() ) ;
        info.setPSignalSemaphores   ( data().syncs[ index ].signalSems()    ) ;
        info.setPWaitDstStageMask   ( flags.data()                          ) ;
      
        queueMutex().lock() ;
        queue.submit( 1, &info, nullptr ) ;
        queueMutex().unlock() ;

        img_mutex.lock() ;
        data().images.push( result.value ) ;
        img_mutex.unlock() ;

        data().current_img = result.value ;
        data().current_frame++ ;
        
        bus( "start" ).emit( data().syncs[ index ] ) ;

        if( index == 0 ) data().sync_index = 1 ;
        else             data().sync_index = 0 ;
      }
      
      while( data().current_frame >= 2 ) {} ;
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
  