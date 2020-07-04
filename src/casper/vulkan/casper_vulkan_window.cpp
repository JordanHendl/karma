#include "casper_vulkan_window.h"
#include "casper_vulkan_device.h"
#include "casper_vulkan_device_pool.h"
#include "casper_vulkan_swapchain.h"
#include "casper_vulkan_error_handler.h"
#include "casper_vulkan_semaphore.h"
#include "casper_vulkan_surface.h"
#include "casper_vulkan_command_buffer.h"
#include <Bus.h>
#include <Pool.h>
#include <PoolData.h>
#include <Signal.h>
#include <vulkan/vulkan.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_vulkan.h>
#include <string>
#include <stdexcept>
#include <vector>

namespace casper
{
  namespace vulkan
  {
    struct VulkanInfo
    {
      typedef std::vector<Semaphore> Semaphores     ;
      typedef std::vector<VkFence>   ImageFences    ;
      typedef std::vector<VkFence>   FlightFences   ;
      typedef VkClearValue           ClearColor     ;
      const VkSemaphore*  wait_sem      ;
      const VkSemaphore*  signal_sem    ;
      ImageFences         img_fences    ;
      FlightFences        fences        ;
      VkPresentInfoKHR    present_info  ;
      Semaphores          image_sem     ;
      Semaphores          finish_sem    ;
      ClearColor          color         ;
      VkClearValue        clear_color   ;
      const Device        device        ;
      uint32_t            image         ;
      VkRenderPass        render_pass   ;
      unsigned            current_frame ;
    };

    struct WindowConf
    {
      unsigned max_frames_in_flight ;

      WindowConf()
      {
        this->max_frames_in_flight = 5 ;
      }
    };

    struct WindowData
    {
      data::module::Bus         bus        ; ///< JHTODO
      data::module::Pool        pool       ;
      SDL_Window*               window     ; ///< JHTODO
      casper::vulkan::Surface   surface    ;
      casper::vulkan::Device    device     ;
      casper::vulkan::SwapChain chain      ;
      VulkanInfo                info       ;
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
      VkAttachmentDescription desc         ;
      VkSubpassDescription    subpass_desc ;
      VkAttachmentReference   attach_ref   ;
      VkRenderPassCreateInfo  info         ;
      VkSubpassDependency     dependency   ;

      desc                = {}                                            ;
      desc.format         = static_cast<VkFormat>( this->chain.format() ) ;
      desc.samples        = VK_SAMPLE_COUNT_1_BIT                         ;
      desc.loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR                   ;
      desc.storeOp        = VK_ATTACHMENT_STORE_OP_STORE                  ;
      desc.stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE               ;
      desc.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE              ;
      desc.initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED                     ;
      desc.finalLayout    = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR               ;

      dependency               = {}                                            ;
      dependency.srcSubpass    = VK_SUBPASS_EXTERNAL                           ;
      dependency.dstSubpass    = 0                                             ;
      dependency.srcStageMask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT ;
      dependency.srcAccessMask = 0                                             ;
      dependency.dstStageMask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT ;
      dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT          ;

      attach_ref = {}                                                  ;
      attach_ref.attachment = 0                                        ;
      attach_ref.layout     = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL ;

      subpass_desc                      = {}                              ;
      subpass_desc.pipelineBindPoint    = VK_PIPELINE_BIND_POINT_GRAPHICS ;
      subpass_desc.colorAttachmentCount = 1                               ;
      subpass_desc.pColorAttachments    = &attach_ref                     ;

      info                 = {}                                        ;
      info.sType           = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO ;
      info.attachmentCount = 1                                         ;
      info.pAttachments    = &desc                                     ;
      info.subpassCount    = 1                                         ;
      info.pSubpasses      = &subpass_desc                             ;
      info.dependencyCount = 1                                         ;
      info.pDependencies   = &dependency                               ;

      if( vkCreateRenderPass( this->device.device(), &info, NULL, &this->info.render_pass ) != VK_SUCCESS )
      {
        throw std::runtime_error( "Failed to create Vulkan Render Pass.") ;
      }
    }

    void WindowData::createFramebuffers()
    {
       chain.createFrameBuffers( this->info.render_pass, device ) ;
    }

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
      const VkInstance instance = data().pool[ "CASPER_VULKAN_INSTANCE" ].get<const VkInstance>() ;
      DevicePool        pool       ;
      VkFenceCreateInfo fence_info ;

      data().window = SDL_CreateWindow( name, 500, 500, width, height, SDL_WINDOW_VULKAN | SDL_WINDOW_SHOWN ) ;
  
      data().surface.initialize( *data().window                ) ;
      data().device.initialize ( data().surface, instance, gpu ) ;
      pool.setDevice( gpu, data().device ) ;
      data().chain.initialize  ( data().surface, data().device ) ;
      data().generateRenderPass() ;
      data().createFramebuffers() ;
      
      data().info.finish_sem.resize( data().conf.max_frames_in_flight          ) ;
      data().info.image_sem .resize( data().conf.max_frames_in_flight          ) ;
      data().info.fences    .resize( data().conf.max_frames_in_flight          ) ;
      data().info.img_fences.resize( data().chain.numBuffers(), VK_NULL_HANDLE ) ;
      
      fence_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO ;
      fence_info.flags = VK_FENCE_CREATE_SIGNALED_BIT        ;
      fence_info.pNext = NULL                                ;

      for( unsigned i = 0; i < data().info.fences.size(); i++ ) 
      {
        vkCreateFence( data().device.device(), &fence_info, NULL, &data().info.fences.at( i ) ) ;
        data().info.image_sem .at( i ).initialize( data().device ) ;
        data().info.finish_sem.at( i ).initialize( data().device ) ;
      }

      data().info.present_info.sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR ;
      data().info.present_info.waitSemaphoreCount = 1                                  ;
      data().info.present_info.swapchainCount     = 1                                  ;
      data().info.present_info.pResults           = NULL                               ;
      data().info.present_info.pNext              = NULL                               ;
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
  
    //void Window::setSize( unsigned width, unsigned height )
    //{ 
    //  data().width  = width  ;
    //  data().height = height ;
    //  SDL_SetWindowSize( data().window, width, height ) ;
    //}
    
    void Window::submit( const CommandBuffer& buffer )
    {
      
      const VkQueue       queue          = data().device.graphics()                         ;
      const VkPipelineStageFlags flags[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT } ;
      VkFence             fence        ;
      VkSubmitInfo        info         ;

      data().info.wait_sem   = data().info.image_sem .at( data().info.current_frame ).semaphore() ;
      data().info.signal_sem = data().info.finish_sem.at( data().info.current_frame ).semaphore() ;

      vkWaitForFences( data().device.device(), 1, &data().info.fences    .at( data().info.current_frame ), VK_TRUE, UINT64_MAX ) ;

      data().info.image =  data().info.image_sem.at( data().info.current_frame ).acquire( data().chain ) ;
      
      if( data().info.img_fences.at( data().info.image ) != VK_NULL_HANDLE )
      {
        fence = data().info.img_fences.at( data().info.image ) ;
        vkWaitForFences( data().device.device(), 1, &fence , VK_TRUE, UINT64_MAX ) ;
      }

      data().info.img_fences.at( data().info.image ) = data().info.fences.at( data().info.current_frame ) ;

      info.sType                = VK_STRUCTURE_TYPE_SUBMIT_INFO      ;
      info.pNext                = nullptr                            ;
      info.waitSemaphoreCount   = 1                                  ;
      info.pWaitSemaphores      = data().info.wait_sem               ;
      info.pWaitDstStageMask    = flags                              ;
      info.commandBufferCount   = 1                                  ;
      info.pCommandBuffers      = &buffer.buffer( data().info.image );
      info.signalSemaphoreCount = 1                                  ;
      info.pSignalSemaphores    = data().info.signal_sem             ;
      
      fence = data().info.fences.at( data().info.current_frame ) ;
      vkResetFences  ( data().device.device(), 1, &fence ) ;

      HANDLE_ERROR( vkQueueSubmit( queue, 1, &info, fence ) ) ;
      data().info.current_frame = ( data().info.current_frame + 1 ) % data().conf.max_frames_in_flight ;
    }

     void Window::pollEvents()
    {
      SDL_Event event ;
      while( SDL_PollEvent( &event ) )
      {
        data().bus[ "key_input" ].emit( event ) ;
      }
    }

    void Window::present()
    {
      const VkSemaphore* sem = data().info.signal_sem ;

      data().info.present_info.pWaitSemaphores    = sem                  ;
      data().info.present_info.pSwapchains        = data().chain.chain() ;
      data().info.present_info.pImageIndices      = &data().info.image   ;
      data().info.present_info.pNext              = nullptr              ;
      data().info.present_info.pImageIndices      = &data().info.image   ;
      data().info.present_info.waitSemaphoreCount = 1                    ;

      vkQueuePresentKHR( data().device.present(), &data().info.present_info ) ;
      vkQueueWaitIdle  ( data().device.present()                            ) ;
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
    
    const VkRenderPass& Window::pass() const
    {
      return data().info.render_pass ;
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
  