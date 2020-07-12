#include "casper_vulkan_buffer.h"
#include "casper_vulkan_error_handler.h"
#include "casper_vulkan_command_buffer.h"
#include "casper_vulkan_device_pool.h"
#include "Device.h"
#include <vulkan/vulkan.h>
#include <cstring>
#include <stdlib.h>

namespace casper
{
  namespace vulkan
  {
    static VkBufferUsageFlags TypeToUsage( BufferType type )
    {
      if( type == BufferType::VERTEX  ) return VK_BUFFER_USAGE_VERTEX_BUFFER_BIT  ;
      if( type == BufferType::INDEX   ) return VK_BUFFER_USAGE_INDEX_BUFFER_BIT   ;
      if( type == BufferType::UNIFORM ) return VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT ;
      if( type == BufferType::SSBO    ) return VK_BUFFER_USAGE_STORAGE_BUFFER_BIT ;

      return 0 ;
    }

    static uint32_t findMemType( uint32_t filter, VkMemoryPropertyFlags flag, VkPhysicalDevice device )
    {
      VkPhysicalDeviceMemoryProperties mem_prop ;
      
      vkGetPhysicalDeviceMemoryProperties( device, &mem_prop ) ;
      for( unsigned i = 0; i < mem_prop.memoryTypeCount; i++ )
      {
        if( filter & ( 1 << i ) && (mem_prop.memoryTypes[ i ].propertyFlags & flag ) == flag )
        {
          return i ;
        }
      }
      return 0 ;
    }
    
    static inline unsigned memoryType( unsigned filter, VkMemoryPropertyFlags props, unsigned gpu ) ;

    struct BufferData
    {
      BufferType         type       ;
      VkBuffer           buffer     ;
      VkDeviceMemory     memory     ;
      CommandBuffer      cmd_buff   ;
      bool               compute    ;
      unsigned           gpu        ;
      unsigned           element_sz ;
      unsigned           count      ;
      
      /**
       * @param format
       * @param old_layout
       * @param new_layout
       */
      VkBuffer createBuffer( VkDeviceSize size, VkBufferUsageFlags usage ) ;
      
      /**
       * 
       * @param buffer
       * @param sz
       */
      void copy( VkBuffer buffer, unsigned sz ) ;
      
      
      /**
       * @param properties
       * @param buffer
       * @return 
       */
      VkDeviceMemory allocateMemory( VkMemoryPropertyFlags properties, VkBuffer& buffer ) ;

      /**
       */
      BufferData() ;
    };
    
    unsigned memoryType( unsigned filter, VkMemoryPropertyFlags props, unsigned gpu )
    {
      DevicePool                       pool     ;
      VkPhysicalDeviceMemoryProperties mem_prop ;
      
      vkGetPhysicalDeviceMemoryProperties( pool.device( gpu ).physicalDevice(), &mem_prop ) ;
      
      for( unsigned index = 0; index < mem_prop.memoryTypeCount; index++ )
      {
        if( ( filter & ( 1 << index ) ) && ( mem_prop.memoryTypes[ index ].propertyFlags & props ) == props )
        {
          return index ;
        }
      }
      
      return 0 ;
    }

    BufferData::BufferData()
    {
      
    }
    
    void BufferData::copy( VkBuffer buffer, unsigned sz )
    {
      VkBufferCopy region ;
      
      region      = {} ;
      region.size = sz ;

      cmd_buff.record() ;
      vkCmdCopyBuffer( this->cmd_buff.buffer( 0 ), buffer, this->buffer, 1, &region ) ;
      cmd_buff.stop() ;

      if( this->compute ) cmd_buff.submit( CommandBuffer::SubmitionType::COMPUTE ) ;
      else                cmd_buff.submit( CommandBuffer::SubmitionType::GRAPHICS) ;
    }

    VkBuffer BufferData::createBuffer( VkDeviceSize size, VkBufferUsageFlags usage )
    {
      VkBufferCreateInfo info   ;
      DevicePool         pool   ;
      VkBuffer           buffer ;

      info       = {} ;
      info.sType       = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO ;
      info.size        = size                                 ;
      info.usage       = usage                                ;
      info.sharingMode = VK_SHARING_MODE_EXCLUSIVE            ;
      info.pNext       = nullptr                              ;
      
      HANDLE_ERROR( vkCreateBuffer( pool.device( this->gpu ).device(), &info, nullptr, &buffer ) ) ;
      
      return buffer ;
    }
    
    VkDeviceMemory BufferData::allocateMemory( VkMemoryPropertyFlags properties, VkBuffer& buffer )
    {
      VkMemoryRequirements req        ;
      VkMemoryAllocateInfo alloc_info ;
      VkDeviceMemory       memory     ;
      DevicePool           pool       ;

      alloc_info = {} ;
      vkGetBufferMemoryRequirements( pool.device( this->gpu ).device(), buffer, &req ) ;
      alloc_info.sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO                  ;
      alloc_info.allocationSize  = req.size                                                ;
      alloc_info.memoryTypeIndex = memoryType( req.memoryTypeBits, properties, this->gpu ) ;
      alloc_info.pNext           = nullptr                                                 ;
      
      HANDLE_ERROR( vkAllocateMemory( pool.device( this->gpu ).device(), &alloc_info, nullptr, &memory ) ) ;
      HANDLE_ERROR( vkBindBufferMemory( pool.device( this->gpu ).device(), buffer, memory, 0           ) ) ;
      
      return memory ;
    }

    BufferImpl::BufferImpl()
    {
      this->buffer_data = new BufferData() ;
    }

    BufferImpl::~BufferImpl()
    {
      delete this->buffer_data ;
    }

    void BufferImpl::allocate( unsigned gpu, unsigned sz )
    {
//      const DevicePool pool                        ;
//      const Device     device = pool.device( gpu ) ;
//
//      VkMemoryAllocateInfo alloc_info   ;
//      VkMemoryRequirements requirements ;
//
//      vkGetBufferMemoryRequirements( device.device(), this->data().buffer, &requirements ) ;
//      alloc_info                 = {} ;
//      alloc_info.sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO                                                                    ;
//      alloc_info.allocationSize  = requirements.size                                                                                         ;
//      alloc_info.memoryTypeIndex = findMemType( requirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,  device.physicalDevice() ) ;
//      alloc_info.pNext           = NULL                                                                                                      ;
//
//      HANDLE_ERROR( vkAllocateMemory( device.device(), &alloc_info, NULL, &data().memory ) ) ;
//      HANDLE_ERROR( vkBindBufferMemory( device.device(), data().buffer, data().memory, 0 ) ) ;
    }

    void BufferImpl::initializeBase( unsigned gpu, BufferType type, unsigned element_sz, unsigned count, bool compute )
    {
      const unsigned sz = element_sz * count ;

      this->data().count      = count       ;
      this->data().type       = type        ; 
      this->data().element_sz = element_sz  ;
      this->data().compute    = compute     ;
      this->data().cmd_buff.initCompute( gpu, 1 ) ;
      
      this->data().buffer = this->data().createBuffer  ( sz, VK_BUFFER_USAGE_TRANSFER_DST_BIT | TypeToUsage( type ) ) ;
      this->data().memory = this->data().allocateMemory( VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, this->data().buffer   ) ;
    }
    
    unsigned BufferImpl::byteSize() const
    {
      return data().count * data().element_sz ;
    }
    
    BufferType BufferImpl::type() const
    {
      return data().type ;
    }

    const VkBuffer& BufferImpl::buffer() const
    {
      return data().buffer ;
    }

    unsigned BufferImpl::size() const
    {
      return data().count ;
    }

    void BufferImpl::copyToDevice( const void* data, unsigned offset )
    {
      const DevicePool pool                                     ;
      const Device     device = pool.device( this->data().gpu ) ;
      const unsigned sz = this->data().element_sz * this->data().count ;
      VkBuffer       staging_buffer ;
      VkDeviceMemory staging_mem    ;
      void*          mapped_data    ;
      
      staging_buffer = this->data().createBuffer  ( sz, VK_BUFFER_USAGE_TRANSFER_SRC_BIT                                                       ) ;
      staging_mem    = this->data().allocateMemory( VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, staging_buffer ) ;

      vkMapMemory( device.device(), staging_mem, 0, sz, 0, &mapped_data ) ;
      std::memcpy( mapped_data, data, (size_t)sz                        ) ;
      vkUnmapMemory( device.device(), staging_mem                       ) ;
      
      this->data().copy( staging_buffer, sz ) ;
      
      vkDestroyBuffer( device.device(), staging_buffer, nullptr ) ;
      vkFreeMemory   ( device.device(), staging_mem   , nullptr ) ;
    }

    const VkBuffer& Buffer::buffer() const
    {
      return this->impl.buffer() ;
    }

    unsigned Buffer::size() const
    {
      return this->impl.size() ;
    }

    unsigned Buffer::elementSize() const
    {
      return this->impl.data().element_sz ;
    }

    unsigned Buffer::byteSize() const
    {
      return this->impl.byteSize() ;
    }

    BufferType Buffer::type() const
    {
      return this->impl.type() ;
    }

    const BufferData& BufferImpl::data() const
    {
      return *this->buffer_data ;
    }

    BufferData& BufferImpl::data()
    {
      return *this->buffer_data ;
    }
  }
}
