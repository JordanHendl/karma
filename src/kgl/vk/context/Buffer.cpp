#include "Buffer.h"
#include "CommandBuffer.h"
#include "Context.h"
#include <vulkan/vulkan.hpp>
#include <iostream>
#include <vector>
#include <queue>
#include <limits.h>
namespace kgl
{
  namespace vk
  {
    /**
     * @param type
     * @return 
     */
    static ::vk::BufferUsageFlags TypeToUsage( Buffer::Type type ) ;
    
    /**
     * @param filter
     * @param flag
     * @param device
     * @return 
     */
    static uint32_t findMemType( uint32_t filter, ::vk::MemoryPropertyFlags flag, ::vk::PhysicalDevice device ) ;
    
    struct BufferData
    {
      Buffer::Type           type           ; ///< TODO
      ::vk::Buffer           buffer         ; ///< TODO
      ::vk::DeviceMemory     memory         ; ///< TODO
      ::vk::Buffer           staging_buffer ;
      ::vk::DeviceMemory     staging_mem    ;
      ::vk::Device           device         ; ///< TODO
      ::vk::Fence            fence          ; ///< TODO
      compute::CommandBuffer cmd_buff       ; ///< TODO
      bool                   host_local     ;
      unsigned               gpu            ; ///< TODO
      unsigned               element_sz     ; ///< TODO
      unsigned               count          ; ///< TODO

      std::queue<::vk::Buffer>       temp_buffs ;
      std::queue<::vk::DeviceMemory> temp_mems  ;
      /** TODO
       */
      BufferData() ;
      
      /**
       * @param size
       * @param usage
       * @return 
       */
      ::vk::Buffer createBuffer( ::vk::DeviceSize size, ::vk::BufferUsageFlags usage ) ;
      
      /**
       * @param data
       */
      void operator=( const BufferData& data ) ;

      /**
       * @param props
       * @param buffer
       * @return 
       */
      ::vk::DeviceMemory allocate( ::vk::MemoryPropertyFlags props, ::vk::Buffer buffer ) ;
      
      /**
       * @param buffer
       * @param size
       */
      void copy( ::vk::Buffer buffer, unsigned size, ::vk::CommandBuffer& buff, unsigned offset = 0 ) ;
      
      /**
       * @param buffer
       * @param size
       */
      void copy( ::vk::Buffer buffer, unsigned size, unsigned offset = 0 ) ;
    };

    ::vk::BufferUsageFlags TypeToUsage( Buffer::Type type )
    {
      if( type == Buffer::Type::VERTEX  ) return ::vk::BufferUsageFlagBits::eVertexBuffer  ;
      if( type == Buffer::Type::INDEX   ) return ::vk::BufferUsageFlagBits::eIndexBuffer   ;
      if( type == Buffer::Type::UNIFORM ) return ::vk::BufferUsageFlagBits::eUniformBuffer ;
      if( type == Buffer::Type::SSBO    ) return ::vk::BufferUsageFlagBits::eStorageBuffer ;

      return ::vk::BufferUsageFlagBits::eUniformBuffer ;
    }

    uint32_t findMemType( uint32_t filter, ::vk::MemoryPropertyFlags flag, ::vk::PhysicalDevice device )
    {
      ::vk::PhysicalDeviceMemoryProperties mem_prop ;
      
      mem_prop = device.getMemoryProperties() ;
      for( unsigned i = 0; i < mem_prop.memoryTypeCount; i++ )
      {
        if( filter & ( 1 << i ) && (mem_prop.memoryTypes[ i ].propertyFlags & flag ) == flag )
        {
          
          return i ;
        }
      }
      return 0 ;
    }
        
    BufferData::BufferData()
    {
      this->count      = 0                  ;
      this->element_sz = 0                  ;
      this->gpu        = UINT_MAX           ;
      this->host_local = false              ;
      this->type       = Buffer::Type::SSBO ;
    }

    void BufferData::operator=( const BufferData& data )
    {
      this->device     = data.device     ;
      this->buffer     = data.buffer     ;
      this->cmd_buff   = data.cmd_buff   ;
      this->gpu        = data.gpu        ;
      this->type       = data.type       ;
      this->count      = data.count      ;
      this->element_sz = data.element_sz ;
      this->memory     = data.memory     ;
      this->fence      = data.fence      ;
    }

    ::vk::Buffer BufferData::createBuffer( ::vk::DeviceSize size, ::vk::BufferUsageFlags usage )
    {
      const ::kgl::vk::compute::Context context ;
      const ::vk::Device                device   = context.virtualDevice ( this->gpu ) ;
      ::vk::BufferCreateInfo info   ;
      ::vk::Buffer           buffer ;

      info.setSize ( size  ) ;
      info.setUsage( usage ) ;
      info.setSharingMode( ::vk::SharingMode::eExclusive ) ;
      
      buffer = device.createBuffer( info, nullptr ) ;
      
      return buffer ;
    }

    ::vk::DeviceMemory BufferData::allocate( ::vk::MemoryPropertyFlags props, ::vk::Buffer buffer )
    {
      const ::kgl::vk::compute::Context context ;
      const ::vk::PhysicalDevice p_device = context.physicalDevice( this->gpu ) ;
      const ::vk::Device         device   = context.virtualDevice ( this->gpu ) ;
      ::vk::MemoryRequirements req        ;
      ::vk::MemoryAllocateInfo alloc_info ;
      ::vk::DeviceMemory       memory     ;
      req = device.getBufferMemoryRequirements( buffer ) ;
      
      
      alloc_info.setAllocationSize( req.size )                                            ;
      alloc_info.setMemoryTypeIndex( findMemType( req.memoryTypeBits, props, p_device ) ) ;

      memory = device.allocateMemory( alloc_info, nullptr ) ;
      device.bindBufferMemory( buffer, memory, 0 ) ;

      return memory ;
    }

    void BufferData::copy( ::vk::Buffer buffer, unsigned size, ::vk::CommandBuffer& buff, unsigned offset )
    {
      ::kgl::vk::compute::Context context ;
      ::vk::BufferCopy          region       ;
      ::vk::MemoryBarrier       barrier      ;
      ::vk::BufferMemoryBarrier buff_barrier ;
      ::vk::DependencyFlags     flags        ;
      
      region.setSize     ( size   ) ;
      region.setSrcOffset( offset ) ;
      region.setDstOffset( offset ) ;
      
      barrier.setSrcAccessMask           ( ::vk::AccessFlagBits::eMemoryWrite | ::vk::AccessFlagBits::eMemoryRead ) ;
      barrier.setDstAccessMask           ( ::vk::AccessFlagBits::eMemoryRead | ::vk::AccessFlagBits::eVertexAttributeRead ) ;
      buff_barrier.setSrcAccessMask      ( ::vk::AccessFlagBits::eMemoryWrite | ::vk::AccessFlagBits::eMemoryRead ) ;
      buff_barrier.setDstAccessMask      ( ::vk::AccessFlagBits::eMemoryRead | ::vk::AccessFlagBits::eVertexAttributeRead ) ;
      buff_barrier.setBuffer             ( this->buffer                       ) ;
      buff_barrier.setSize               ( size                               ) ;
      buff_barrier.setSrcQueueFamilyIndex( context.computeFamily( this->gpu ) ) ;
      buff_barrier.setDstQueueFamilyIndex( context.computeFamily( this->gpu ) ) ;
      buff.copyBuffer( buffer, this->buffer, 1, &region ) ;
      buff.pipelineBarrier( ::vk::PipelineStageFlagBits::eAllCommands, ::vk::PipelineStageFlagBits::eAllCommands, flags, 0, nullptr, 1, &buff_barrier, 0, nullptr ) ;
    }
    
    void BufferData::copy( ::vk::Buffer buffer, unsigned size, unsigned offset )
    {
      ::kgl::vk::compute::Context context ;
      ::vk::BufferCopy          region       ;
      ::vk::MemoryBarrier       barrier      ;
      ::vk::BufferMemoryBarrier buff_barrier ;
      ::vk::DependencyFlags     flags        ;

      region.setSize     ( size   ) ;
      region.setSrcOffset( offset ) ;
      region.setDstOffset( offset ) ;
      
      barrier.setSrcAccessMask           ( ::vk::AccessFlagBits::eMemoryWrite ) ;
      barrier.setDstAccessMask           ( ::vk::AccessFlagBits::eMemoryRead | ::vk::AccessFlagBits::eVertexAttributeRead ) ;
      buff_barrier.setSrcAccessMask      ( ::vk::AccessFlagBits::eMemoryWrite ) ;
      buff_barrier.setDstAccessMask      ( ::vk::AccessFlagBits::eMemoryRead | ::vk::AccessFlagBits::eVertexAttributeRead ) ;
      buff_barrier.setBuffer             ( this->buffer                       ) ;
      buff_barrier.setSize               ( size                               ) ;
      buff_barrier.setSrcQueueFamilyIndex( context.computeFamily( this->gpu ) ) ;
      buff_barrier.setDstQueueFamilyIndex( context.computeFamily( this->gpu ) ) ;
      this->cmd_buff.record() ;
      this->cmd_buff.buffer( 0 ).copyBuffer( buffer, this->buffer, 1, &region ) ;
      this->cmd_buff.buffer( 0 ).pipelineBarrier( ::vk::PipelineStageFlagBits::eAllGraphics, ::vk::PipelineStageFlagBits::eAllGraphics, flags, 0, nullptr, 1, &buff_barrier, 0, nullptr ) ;
      this->cmd_buff.stop() ;
      this->cmd_buff.submit() ;
    }

    BufferImpl::BufferImpl()
    {
      this->buffer_data = new BufferData() ;
    }

    BufferImpl::~BufferImpl()
    {
//      delete this->buffer_data ;
    }

    void BufferImpl::initializeBase( unsigned device, unsigned type, unsigned element_sz, bool host_local, unsigned count )
    {
      const ::kgl::vk::compute::Context context ;
      const unsigned sz = element_sz * count ;
      ::vk::DeviceSize             size         ;
      ::vk::FenceCreateInfo        fence_info   ;
      ::vk::BufferUsageFlags       buffer_flags ;
      ::vk::MemoryPropertyFlags    mem_flags    ;

      size = sz ;

      data().device     = context.virtualDevice( device ) ;
      data().gpu        = device                          ;
      data().host_local = host_local                      ;

      this->data().count      = count                             ;
      this->data().type       = static_cast<Buffer::Type>( type ) ; 
      this->data().element_sz = element_sz                        ;
      this->data().device     = context.virtualDevice( device )   ;
      this->data().cmd_buff.initialize( device, 1, ::kgl::vk::BufferLevel::Primary ) ;
      
      if( host_local )
      {
        buffer_flags = ::vk::BufferUsageFlagBits::eTransferDst | TypeToUsage( data().type ) | ::vk::BufferUsageFlagBits::eTransferSrc  ;
        mem_flags    = ::vk::MemoryPropertyFlagBits::eHostCoherent | ::vk::MemoryPropertyFlagBits::eHostVisible                        ;
      }
      else
      {
        buffer_flags = ::vk::BufferUsageFlagBits::eTransferDst | TypeToUsage( data().type )                     ;
        mem_flags    = ::vk::MemoryPropertyFlagBits::eHostCoherent | ::vk::MemoryPropertyFlagBits::eHostVisible ;
        
        this->data().staging_buffer = this->data().createBuffer( size, ::vk::BufferUsageFlagBits::eTransferSrc ) ;
        this->data().staging_mem    = this->data().allocate    ( ::vk::MemoryPropertyFlagBits::eHostCoherent | ::vk::MemoryPropertyFlagBits::eHostVisible, this->data().staging_buffer ) ;
      }
      
      this->data().buffer = this->data().createBuffer( size     , buffer_flags        ) ;
      this->data().memory = this->data().allocate    ( mem_flags, this->data().buffer ) ;
    }

    void BufferImpl::copyToDevice( const void* data, unsigned count, unsigned offset )
    {
      const unsigned sz = this->data().element_sz * ( count == 0 ? this->data().count : count ) ;
      ::vk::MemoryMapFlags flag           ;
      void*                mapped_data    ;
      
      if( this->data().host_local )
      {
        this->data().device.mapMemory( this->data().memory, offset, ::vk::DeviceSize( sz ), flag, &mapped_data ) ;
        std::memcpy( mapped_data, data, (size_t)sz                                                             ) ;
        this->data().device.unmapMemory( this->data().memory                                                   ) ;
      }
      else
      {
        this->data().device.mapMemory( this->data().staging_mem, offset, ::vk::DeviceSize( sz ), flag, &mapped_data ) ;
        std::memcpy( mapped_data, data, (size_t)sz                                                             ) ;
        this->data().device.unmapMemory( this->data().staging_mem                                                   ) ;
        
        this->data().copy( this->data().staging_buffer, sz, offset ) ;
      }
    }
    
    bool Buffer::isInitialized() const
    {
      return this->impl.data().count != 0 ;
    }
    
    BufferData& BufferImpl::data()
    {
      return *this->buffer_data ;
    }

    const BufferData& BufferImpl::data() const
    {
      return *this->buffer_data ;
    }

    Buffer::Buffer()
    {
    
    }

    Buffer::~Buffer()
    {
    
    }
    
    void Buffer::copy( const Buffer& buffer )
    {
      const unsigned this_size   = impl.data().element_sz        * impl.data().count                           ;
      const unsigned input_size  = buffer.impl.data().element_sz * buffer.impl.data().count                    ;
      const unsigned needed_size = impl.data().count + ( ( input_size - this_size ) / impl.data().element_sz ) ;
      
      if( this_size < input_size )
      {
        this->reset() ;
        impl.initializeBase( impl.data().gpu, impl.data().type, impl.data().element_sz, impl.data().host_local, needed_size ) ;
      }
      
      impl.data().copy( buffer.buffer(), input_size ) ;
    }
    
    void Buffer::reset() 
    {
      impl.data().device.destroy( impl.data().buffer ) ;
      impl.data().device.free   ( impl.data().memory ) ;
    }

    const ::vk::Buffer Buffer::buffer() const
    {
      return impl.data().buffer ;
    }

    Buffer::Type Buffer::type() const
    {
      return impl.data().type ;
    }
    
    unsigned Buffer::byteSize() const
    {
      return impl.data().count * impl.data().element_sz ;
    }

    unsigned Buffer::elementSize() const
    {
      return impl.data().element_sz ;
    }
  }
}

    
