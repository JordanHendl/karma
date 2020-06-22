#include "casper_vulkan_command_buffer.h"
#include "casper_vulkan_device.h"
#include "casper_vulkan_swapchain.h"
#include "casper_vulkan_pipeline.h"
#include "casper_vulkan_semaphore.h"
#include <vulkan/vulkan.h>
#include <string>
#include <vector>
#include <map>

namespace casper
{
  namespace vulkan
  {
    typedef std::map<std::string, VkCommandPool> CommandBufferPool ;    
    static CommandBufferPool pool ;

    struct CommandBufferConf
    {
      unsigned max_frames_in_flight ;

      CommandBufferConf()
      {
        this->max_frames_in_flight = 2 ;
      }
    };

    struct CommandBufferData
    {
      typedef std::vector<VkRenderPassBeginInfo> RenderPasses   ;
      typedef std::vector<VkCommandBuffer>       CommandBuffers ;
      typedef std::vector<Semaphore>             Semaphores     ;
      typedef std::vector<VkFence>               ImageFences    ;
      typedef std::vector<VkFence>               FlightFences   ;
      typedef VkClearValue                       ClearColor     ;

      CommandBufferConf conf          ;
      ImageFences       img_fences    ;
      FlightFences      fences        ;
      VkPresentInfoKHR  present_info  ;
      Semaphores        image_sem     ;
      Semaphores        finish_sem    ;
      ClearColor        color         ;
      CommandBuffers    buffers       ;
      RenderPasses      render_info   ;
      VkClearValue      clear_color   ;
      Device            device        ;
      unsigned          current_frame ;
      uint32_t          image        ;

      CommandBufferData()
      {
        this->current_frame = 0 ;
      }
    } ;

    static VkCommandPool createCommandBufferPool( const Device& device, unsigned family, unsigned flags )
    {
      VkCommandPoolCreateInfo info     ;
      VkCommandPool           cmd_pool ;

      info                  = {}                                         ;
      info.sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO ;
      info.queueFamilyIndex = family                                     ;
      info.flags            = flags                                      ;

      if( vkCreateCommandPool( device.device(), &info, nullptr, &cmd_pool)  != VK_SUCCESS )
      {
        throw std::runtime_error(" Failed to create command buffer pool." ) ;
      }

      return cmd_pool ;
    }

    CommandBuffer::~CommandBuffer()
    {
      delete this->buffer_data ;
    }

    CommandBuffer::CommandBuffer()
    {
      this->buffer_data = new CommandBufferData() ;
    }

    void CommandBuffer::initialize( const Device& device, const Pipeline& pipeline, const SwapChain& chain )
    {
      VkCommandBufferAllocateInfo info       ;
      VkFenceCreateInfo           fence_info ;

      data().device = device ;
      
      data().finish_sem.resize( data().conf.max_frames_in_flight                 ) ;
      data().image_sem .resize( data().conf.max_frames_in_flight                 ) ;
      data().fences    .resize( data().conf.max_frames_in_flight                 ) ;
      data().img_fences.resize( chain.numBuffers(), VK_NULL_HANDLE               ) ;
      
      fence_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO ;
      fence_info.flags = VK_FENCE_CREATE_SIGNALED_BIT        ;
      fence_info.pNext = NULL                                ;

      for( unsigned i = 0; i < data().fences.size(); i++ ) 
      {
        vkCreateFence( device.device(), &fence_info, NULL, &data().fences.at( i ) ) ;
        data().image_sem .at( i ).initialize( device ) ;
        data().finish_sem.at( i ).initialize( device ) ;
      }

      if( pool.find( std::string( device.name() ) ) == pool.end() ) 
      {
        pool.insert( { device.name(), createCommandBufferPool( device, device.graphicsFamily(), 0 ) } ) ;
      }

      data().buffers    .resize( chain.numBuffers() ) ;
      data().render_info.resize( chain.numBuffers() ) ;

      info                    = {}                                             ;
      info.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO ;
      info.commandPool        = pool.at( device.name() )                       ;
      info.level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY                ;
      info.commandBufferCount = static_cast<uint32_t>( chain.numBuffers() )    ;

      if( vkAllocateCommandBuffers( device.device(), &info, data().buffers.data() ) != VK_SUCCESS )
      {
        std::string error = "Failed to allocate command buffers on device " ;
        error.append( std::string( device.name() ) ) ;
        throw std::runtime_error( error.c_str() ) ;
      }

      for( unsigned i = 0; i < chain.numBuffers(); i++ )
      {
        data().render_info[ i ].sType             = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO ;
        data().render_info[ i ].renderPass        = pipeline.renderPass()                    ;
        data().render_info[ i ].framebuffer       = chain.buffers()[ i ]                     ;
        data().render_info[ i ].renderArea.offset = { 0, 0 }                                 ;
        data().render_info[ i ].renderArea.extent = {chain.width(), chain.height()}          ;
        data().render_info[ i ].pClearValues      = &data().clear_color                      ;
        data().render_info[ i ].clearValueCount   = 1                                        ;
      }
    }

    void CommandBuffer::record()
    {

      VkCommandBufferBeginInfo info      ;
      VkRenderPassBeginInfo    pass_info ;

      for( unsigned i = 0; i < data().buffers.size(); i++ )
      {
        info                  = {}                                          ;
        info.sType            = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO ;
        info.flags            = 0                                           ;
        info.pInheritanceInfo = nullptr                                     ;

        if( vkBeginCommandBuffer( data().buffers.at( i ), &info ) != VK_SUCCESS )
        {
          throw std::runtime_error("Failed to begin recording command buffers.") ;
        }

        vkCmdBeginRenderPass( data().buffers.at( i ), &data().render_info[ i ], VK_SUBPASS_CONTENTS_INLINE ) ;
      }
    }

    const VkPresentInfoKHR& CommandBuffer::submit( const SwapChain& chain )
    {
      const VkPipelineStageFlags flags[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT } ;

      VkSubmitInfo        info         ;
      const VkSemaphore*  wait_sem     ;
      const VkSemaphore*  signal_sem   ;
      VkFence             fence        ;

      wait_sem   = data().image_sem .at( data().current_frame ).semaphore() ;
      signal_sem = data().finish_sem.at( data().current_frame ).semaphore() ;

      vkWaitForFences( data().device.device(), 1, &data().fences  .at( data().current_frame ), VK_TRUE, UINT64_MAX ) ;
      vkResetFences( data().device.device(), 1, &data().fences    .at( data().current_frame ) ) ;

      data().image =  data().image_sem.at( data().current_frame ).acquire( chain ) ;
      
      if( data().img_fences.at( data().image ) != VK_NULL_HANDLE )
      {
        vkWaitForFences( data().device.device(), 1, &data().img_fences.at( data().image ), VK_TRUE, UINT64_MAX ) ;
      }

      data().img_fences.at( data().image ) = data().fences.at( data().current_frame ) ;

      info.sType                = VK_STRUCTURE_TYPE_SUBMIT_INFO      ;
      info.pNext                = NULL                               ;
      info.waitSemaphoreCount   = 1                                  ;
      info.pWaitSemaphores      = wait_sem                           ;
      info.pWaitDstStageMask    = flags                              ;
      info.commandBufferCount   = 1                                  ;
      info.pCommandBuffers      = &data().buffers.at( data().image ) ;
      info.signalSemaphoreCount = 1                                  ;
      info.pSignalSemaphores    = signal_sem                         ;

      if( vkQueueSubmit( data().device.graphics(), 1, &info, data().fences.at( data().current_frame ) ) != VK_SUCCESS )
      {
        throw std::runtime_error("Failed to submit draw command buffer." ) ;
      }

      data().present_info.sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR ;
      data().present_info.waitSemaphoreCount = 1                                  ;
      data().present_info.pWaitSemaphores    = signal_sem                         ;
      data().present_info.swapchainCount     = 1                                  ;
      data().present_info.pSwapchains        = chain.chain()                      ;
      data().present_info.pImageIndices      = &data().image                      ;
      data().present_info.pResults           = NULL                               ;
      data().present_info.pNext              = NULL                               ;

      data().current_frame = ( data().current_frame + 1 ) % data().conf.max_frames_in_flight ;
      return data().present_info ;
    }

    void CommandBuffer::stop()
    {
      for( auto cmd : data().buffers )
      {
        vkCmdEndRenderPass( cmd ) ;
        if( vkEndCommandBuffer( cmd ) != VK_SUCCESS ) throw std::runtime_error( "Failed to record command buffer" ) ;
      }
    }

    void CommandBuffer::draw( unsigned count, const VkBuffer& buffer, const Pipeline& pipeline )
    {
      unsigned     it      ;
      VkDeviceSize offsets ;

      it = 0 ;
      for( auto cmd : data().buffers )
      {
        offsets = 0 ;
        pipeline.bind( *this, it ) ;
        vkCmdBindVertexBuffers( cmd, 0, 1, &buffer, &offsets ) ;
        vkCmdDraw( cmd, static_cast<uint32_t>( count ), 1, 0, 0 ) ;
      }
    }
    const VkCommandBuffer& CommandBuffer::buffer( unsigned id ) const
    {
      return id < data().buffers.size() ? data().buffers.at( 0 ) : data().buffers.at( id ) ;
    }
    
    CommandBufferData& CommandBuffer::data()
    {
      return *this->buffer_data ;
    }

    const CommandBufferData& CommandBuffer::data() const
    {
      return *this->buffer_data ;
    }
  }
}