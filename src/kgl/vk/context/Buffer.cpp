#include "Buffer.h"
#include "CommandBuffer.h"
#include "Context.h"
#include <vulkan/vulkan.hpp>

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
    
    /**
     * @param filter
     * @param props
     * @param gpu
     * @return 
     */
    static inline unsigned memoryType( unsigned filter, ::vk::MemoryPropertyFlags props, unsigned gpu ) ;

    struct BufferData
    {
      Buffer::Type           type       ; ///< TODO
      ::vk::Buffer           buffer     ; ///< TODO
      ::vk::DeviceMemory     memory     ; ///< TODO
      ::vk::Device           device     ; ///< TODO
      ::vk::Fence            fence      ; ///< TODO
      compute::CommandBuffer cmd_buff   ; ///< TODO
      unsigned               gpu        ; ///< TODO
      unsigned               element_sz ; ///< TODO
      unsigned               count      ; ///< TODO
      
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
       * @param props
       * @param buffer
       * @return 
       */
      ::vk::DeviceMemory allocate( ::vk::MemoryPropertyFlags props, ::vk::Buffer buffer ) ;
      
      /**
       * @param buffer
       * @param size
       */
      void copy( ::vk::Buffer buffer, unsigned size ) ;
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
    
    }

    ::vk::Buffer BufferData::createBuffer( ::vk::DeviceSize size, ::vk::BufferUsageFlags usage )
    {
      ::vk::BufferCreateInfo info   ;
      ::vk::Buffer           buffer ;

      info.setSize ( size  ) ;
      info.setUsage( usage ) ;
      info.setSharingMode( ::vk::SharingMode::eExclusive ) ;
      
      buffer = this->device.createBuffer( info, nullptr ) ;
      
      return buffer ;
    }

    ::vk::DeviceMemory BufferData::allocate( ::vk::MemoryPropertyFlags props, ::vk::Buffer buffer )
    {
      const ::kgl::vk::compute::Context context ;
      const ::vk::PhysicalDevice p_device = context.physicalDevice( this->gpu ) ;
      
      ::vk::MemoryRequirements req        ;
      ::vk::MemoryAllocateInfo alloc_info ;
      ::vk::DeviceMemory       memory     ;
      req = this->device.getBufferMemoryRequirements( buffer ) ;
      
      alloc_info.setAllocationSize( req.size )                                            ;
      alloc_info.setMemoryTypeIndex( findMemType( req.memoryTypeBits, props, p_device ) ) ;
      
      memory = device.allocateMemory( alloc_info, nullptr ) ;
      device.bindBufferMemory( buffer, memory, 0 ) ;

      return memory ;
    }

    void BufferData::copy( ::vk::Buffer buffer, unsigned size )
    {
      ::vk::BufferCopy region ;
      
      region.setSize( size ) ;

      this->cmd_buff.record() ;
      this->cmd_buff.buffer( 0 ).copyBuffer( buffer, this->buffer, 1, &region ) ;
      this->cmd_buff.stop() ;
      
      this->cmd_buff.submit() ;
    }

    BufferImpl::BufferImpl()
    {
      this->buffer_data = new BufferData() ;
    }

    BufferImpl::~BufferImpl()
    {
      delete this->buffer_data ;
    }

    void BufferImpl::initializeBase( unsigned device, unsigned type, unsigned element_sz, unsigned count )
    {
      const ::kgl::vk::compute::Context context ;
      const unsigned sz = element_sz * count ;
      ::vk::DeviceSize      size       ;
      ::vk::FenceCreateInfo fence_info ;

      fence_info.setFlags( ::vk::FenceCreateFlagBits::eSignaled ) ;
      size = sz ;
      data().fence = this->data().device.createFence( fence_info, nullptr ) ;
      
      this->data().count      = count                             ;
      this->data().type       = static_cast<Buffer::Type>( type ) ; 
      this->data().element_sz = element_sz                        ;
      this->data().device     = context.virtualDevice( device )   ;
      this->data().cmd_buff.initialize( device, 1, ::kgl::vk::BufferLevel::Primary ) ;
      this->data().cmd_buff.attach( data().fence ) ;
      this->data().buffer = this->data().createBuffer( size, ::vk::BufferUsageFlagBits::eTransferDst | TypeToUsage( data().type ) ) ;
      this->data().memory = this->data().allocate    ( ::vk::MemoryPropertyFlagBits::eDeviceLocal, this->data().buffer            ) ;
    }

    void BufferImpl::copyToDevice( const void* data, unsigned offset )
    {
    
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

    const ::vk::Buffer Buffer::buffer() const
    {
      return impl.data().buffer ;
    }

    void Buffer::operator=( const Buffer& buffer )
    {
    
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

    