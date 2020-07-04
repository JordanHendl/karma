#include "casper_vulkan_command_buffer.h"
#include "casper_vulkan_window.h"
#include "casper_vulkan_device_pool.h"
#include "casper_vulkan_error_handler.h"
#include "casper_vulkan_device.h"
#include "casper_vulkan_buffer.h"
#include "casper_vulkan_semaphore.h"
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

    static VkQueue getQueue( casper::vulkan::CommandBuffer::SubmitionType type, unsigned gpu )
    {
      static VkQueue dummy ;
      DevicePool     pool  ;

      switch( type )
      {
        case CommandBuffer::SubmitionType::COMPUTE  : return pool.device( gpu ).compute () ;
        case CommandBuffer::SubmitionType::GRAPHICS : return pool.device( gpu ).graphics() ;
        case CommandBuffer::SubmitionType::PRESENT  : return pool.device( gpu ).present () ;
        default                                     : return dummy                         ;
      };
    }
    struct CommandBufferData
    {
      typedef std::vector<VkRenderPassBeginInfo> RenderPasses   ;
      typedef std::vector<VkCommandBuffer>       CommandBuffers ;
      typedef VkClearValue                       ClearColor     ;

      ClearColor        color       ;
      CommandBuffers    buffers     ;
      RenderPasses      render_info ;
      VkClearValue      clear_color ;
      unsigned          gpu         ;
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
    
    void CommandBuffer::initCompute( unsigned gpu, unsigned num_buffers )
    {
      DevicePool                  device_pool ;
      VkCommandBufferAllocateInfo info        ;
      VkFenceCreateInfo           fence_info  ;
      
      const Device device = device_pool.device( gpu ) ;
      
      if( pool.find( std::string( device.name() ) ) == pool.end() ) 
      {
        pool.insert( { device.name(), createCommandBufferPool( device, device.graphicsFamily(), VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT ) } ) ;
      }
      
      data().gpu = gpu ;
      data().buffers.resize( num_buffers ) ;
      
      info                    = {}                                             ;
      info.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO ;
      info.commandPool        = pool.at( device.name() )                       ;
      info.level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY                ;
      info.commandBufferCount = static_cast<uint32_t>( num_buffers )           ;

      if( vkAllocateCommandBuffers( device.device(), &info, data().buffers.data() ) != VK_SUCCESS )
      {
        std::string error = "Failed to allocate command buffers on device " ;
        error.append( std::string( device.name() ) ) ;
        throw std::runtime_error( error.c_str()    ) ;
      }
    }

    void CommandBuffer::initRender( const Window& window )
    {
      VkCommandBufferAllocateInfo info       ;
      VkFenceCreateInfo           fence_info ;

      if( pool.find( std::string( window.device().name() ) ) == pool.end() ) 
      {
        pool.insert( { window.device().name(), createCommandBufferPool( window.device(), window.device().graphicsFamily(), VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT ) } ) ;
      }

      data().gpu = 0 ;
      data().buffers    .resize( window.chain().numBuffers() ) ;
      data().render_info.resize( window.chain().numBuffers() ) ;

      info                    = {}                                                   ;
      info.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO       ;
      info.commandPool        = pool.at( window.device().name() )                    ;
      info.level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY                      ;
      info.commandBufferCount = static_cast<uint32_t>( window.chain().numBuffers() ) ;

      if( vkAllocateCommandBuffers( window.device().device(), &info, data().buffers.data() ) != VK_SUCCESS )
      {
        std::string error = "Failed to allocate command buffers on device " ;
        error.append( std::string( window.device().name() ) ) ;
        throw std::runtime_error( error.c_str() ) ;
      }
      
      data().clear_color = {0.0f, 0.5f, 0.0f, 1.0f } ;
      for( unsigned i = 0; i < window.chain().numBuffers(); i++ )
      {
        data().render_info[ i ].sType             = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO ;
        data().render_info[ i ].renderPass        = window.pass()                            ;
        data().render_info[ i ].framebuffer       = window.chain().buffers()[ i ]            ;
        data().render_info[ i ].renderArea.offset = { 0, 0 }                                 ;
        data().render_info[ i ].renderArea.extent = {window.chain().width(), window.chain().height()} ;
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

        if( !data().render_info.empty() ) vkCmdBeginRenderPass( data().buffers.at( i ), &data().render_info[ i ], VK_SUBPASS_CONTENTS_INLINE ) ;
      }
    }
    
    void CommandBuffer::submit( SubmitionType type )
    {
      const DevicePool pool ;
      const Device device = pool.device( data().gpu ) ;
      VkSubmitInfo info ;
      info                    = {}                            ;
      info.sType              = VK_STRUCTURE_TYPE_SUBMIT_INFO ;
      info.commandBufferCount = data().buffers.size()         ;
      info.pCommandBuffers    = data().buffers.data()         ;
      
      HANDLE_ERROR( vkQueueSubmit( getQueue( type, data().gpu ), 1, &info, VK_NULL_HANDLE ) ) ;
      vkQueueWaitIdle( getQueue( type, data().gpu ) ) ;
    }

    void CommandBuffer::stop()
    {
      for( auto cmd : data().buffers )
      {
        if( !data().render_info.empty() )  vkCmdEndRenderPass( cmd ) ;
        HANDLE_ERROR( vkEndCommandBuffer( cmd ) ) ;
      }
    }

    void CommandBuffer::drawIndexed( const Buffer& index, const Buffer& vert )
    {
      VkDeviceSize offsets ;

      for( auto cmd : data().buffers )
      {
        offsets = 0 ;
        vkCmdBindVertexBuffers( cmd, 0, 1, &vert.buffer(), &offsets          ) ;
        vkCmdBindIndexBuffer  ( cmd, index.buffer(), 0, VK_INDEX_TYPE_UINT32 ) ;
        vkCmdDrawIndexed( cmd, static_cast<uint32_t>( index.size() ), 1, 0, 0, 0 ) ;
      }
    }

    void CommandBuffer::draw( const Buffer& buffer )
    {
      VkDeviceSize offsets ;

      for( auto cmd : data().buffers )
      {
        offsets = 0 ;
        vkCmdBindVertexBuffers( cmd, 0, 1, &buffer.buffer(), &offsets ) ;
        vkCmdDraw( cmd, static_cast<uint32_t>( buffer.size() / 4 ), 1, 0, 0 ) ;
      }
    }

    const VkCommandBuffer& CommandBuffer::buffer( unsigned id ) const
    {
      return id < data().buffers.size() ? data().buffers.at( id ) : data().buffers.at( 0 ) ;
    }

    const VkCommandBuffer* CommandBuffer::bufferPtr( unsigned id ) const
    {
      return id < data().buffers.size() ? &data().buffers.at( id ) : &data().buffers.at( 0 ) ;
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