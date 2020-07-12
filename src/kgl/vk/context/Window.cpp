#include "Window.h"
#include "Context.h"
#include "Device.h"
#include "Swapchain.h"
#include "Library.h"
#include "Semaphore.h"
#include "Surface.h"
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
#include <vector>

namespace kgl
{
  namespace vk
  {
    struct WindowInformation
    {
      
    };
    struct VulkanInfo
    {
      typedef std::vector<Semaphore>   Semaphores   ;
      typedef std::vector<::vk::Fence> ImageFences  ;
      typedef std::vector<::vk::Fence> FlightFences ;

      ImageFences             img_fences    ;
      FlightFences            fences        ;
      Semaphores              image_sem     ;
      Semaphores              finish_sem    ;
      ::vk::Semaphore         wait_sem      ;
      ::vk::Semaphore         signal_sem    ;
      ::vk::PresentInfoKHR    present_info  ;
      ::vk::ClearValue        clear_color   ;
      ::vk::RenderPass        render_pass   ;
      uint32_t                image         ;
      unsigned                current_frame ;
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
        this->max_frames_in_flight = 5 ;
      }
    };

    struct WindowData
    {
//      data::module::Bus         bus        ; ///< JHTODO
//      data::module::Pool        pool       ;
      SDL_Window*               window     ; ///< JHTODO
      Surface                   surface    ;
      Device                    device     ;
      SwapChain                 chain      ;
//      VulkanInfo                info       ;
      WindowConf                conf       ;
      bool                      shown      ; ///< JHTODO
      bool                      borderless ; ///< JHTODO
      bool                      focus      ; ///< JHTODO
      std::string               title      ; ///< JHTODO
      std::string               name       ; ///< JHTODO
      unsigned                  width      ; ///< JHTODO
      unsigned                  height     ; ///< JHTODO
      bool                      fullscreen ; ///< JHTODO
      
      WindowData() ;

      void generateRenderPass() ;
      void createFramebuffers() ;
    };

    WindowData::WindowData()
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
    
    void WindowData::generateRenderPass()
    {
//      ::vk::AttachmentDescription desc         ;
//      ::vk::SubpassDescription    subpass_desc ;
//      ::vk::AttachmentReference   attach_ref   ;
//      ::vk::RenderPassCreateInfo  info         ;
//      ::vk::SubpassDependency     dependency   ;
//
//      desc.setFormat        (  static_cast<::vk::Format>( this->chain.format() ) ) ;
//      desc.setSamples       ( ::vk::SampleCountFlagBits::e1                    ) ;
//      desc.setLoadOp        ( ::vk::AttachmentLoadOp::eClear                   ) ;
//      desc.setStoreOp       ( ::vk::AttachmentStoreOp::eStore                  ) ;
//      desc.setStencilLoadOp ( ::vk::AttachmentLoadOp::eDontCare                ) ;
//      desc.setStencilStoreOp( ::vk::AttachmentStoreOp::eDontCare               ) ;
//      desc.setInitialLayout ( ::vk::ImageLayout::eUndefined                    ) ;
//      desc.setFinalLayout   ( ::vk::ImageLayout::ePresentSrcKHR                ) ;
//      
//      dependency.setSrcSubpass   ( VK_SUBPASS_EXTERNAL                                 ) ;
//      dependency.setDstSubpass   ( 0                                                   ) ;
//      dependency.setSrcStageMask ( ::vk::PipelineStageFlagBits::eColorAttachmentOutput ) ;
////      dependency.setSrcAccessMask( static_cast<::vk::AccessFlags>( 0 )                  ) ;
//      dependency.setDstStageMask ( ::vk::PipelineStageFlagBits::eColorAttachmentOutput ) ;
//      dependency.setDstAccessMask( ::vk::AccessFlagBits::eColorAttachmentWrite         ) ;
//      
//      attach_ref.setAttachment( 0                                          ) ;
//      attach_ref.setLayout    ( ::vk::ImageLayout::eColorAttachmentOptimal ) ;
//      
//      subpass_desc.setPipelineBindPoint   ( ::vk::PipelineBindPoint::eGraphics ) ;
//      subpass_desc.setColorAttachmentCount( 1                                  ) ;
//      subpass_desc.setPColorAttachments   ( &attach_ref                        ) ;
//      
//      info.setAttachmentCount( 1             ) ;
//      info.setPAttachments   ( &desc         ) ;
//      info.setSubpassCount   ( 1             ) ;
//      info.setPSubpasses     ( &subpass_desc ) ;
//      info.setDependencyCount( 1             ) ;
//      info.setPDependencies  ( &dependency   ) ;
//      info.setPNext          ( nullptr       ) ;
//      
//      
//      this->device.device().createRenderPass( &info, nullptr, &this->info.render_pass ) ;
    }

    void WindowData::createFramebuffers()
    {
//       chain.createFrameBuffers( this->info.render_pass, device ) ;
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
      data().window = SDL_CreateWindow( name, 500, 500, width, height, SDL_WINDOW_VULKAN | SDL_WINDOW_SHOWN ) ;
  
      data().surface.initialize( *data().window                ) ;
      data().device .initialize( data().surface.surface(), gpu ) ;
      data().chain  .initialize( data().surface, data().device ) ;
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

    void Window::present()
    {
//      const ::vk::Semaphore    sem   = data().info.signal_sem   ;
//      const ::vk::Queue        queue = data().device.graphics() ;
//      const ::vk::SwapchainKHR chain = data().chain.chain()     ;
//
//      data().info.present_info.setPWaitSemaphores   ( &sem                  ) ;
//      data().info.present_info.setPSwapchains       ( &chain                ) ;
//      data().info.present_info.setPImageIndices     ( &data().info.image    ) ;
//      data().info.present_info.setSwapchainCount    ( 1                     ) ;
//      data().info.present_info.setWaitSemaphoreCount( 1                     ) ;
//      
//      queue.presentKHR( &data().info.present_info ) ;
//      queue.waitIdle() ;
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

    const SwapChain& Window::chain() const
    {
      return data().chain ;
    }
    //void Window::reset()
    //{
    //  if( data().window ) SDL_DestroyWindow( data().window ) ;  
    //}
    
    const ::vk::RenderPass Window::renderPass() const
    {
//      return data().info.render_pass ;
    }

    const ::vk::SurfaceKHR Window::surface() const
    {
      return data().surface.surface() ;
    }

    //void Window::setName( const char* name )
    //{
    //  data().name = name ;
    //}
    
    //void Window::setBorderless( bool val )
    //{
    //  data().borderless = val ;
    //  SDL_SetWindowBordered( data().window, val ? SDL_TRUE : SDL_FALSE ) ;
    //}
    
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
  