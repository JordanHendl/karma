#include "casper_vulkan_buffer.h"
#include "casper_vulkan_error_handler.h"
#include "casper_vulkan_device.h"
#include <vulkan/vulkan.h>
#include <cstring>
namespace casper
{
  namespace vulkan
  {
    static VkBufferUsageFlags TypeToUsage( BufferType type )
    {
      if( type == BufferType::VERTEX ) return VK_BUFFER_USAGE_VERTEX_BUFFER_BIT ;

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
    

    struct BufferData
    {
      BufferType         type       ;
      VkBuffer           buffer     ;
      VkDeviceMemory     memory       ;
      Device             device     ;
      unsigned           element_sz ;
      unsigned           count      ;
    };

    BufferImpl::BufferImpl()
    {
      this->buffer_data = new BufferData() ;
    }

    BufferImpl::~BufferImpl()
    {
      delete this->buffer_data ;
    }

    void BufferImpl::allocate( const Device& device, unsigned sz )
    {
      VkMemoryAllocateInfo alloc_info   ;
      VkMemoryRequirements requirements ;

      vkGetBufferMemoryRequirements( device.device(), this->data().buffer, &requirements ) ;
      alloc_info                 = {} ;
      alloc_info.sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO                                                                           ;
      alloc_info.allocationSize  = requirements.size                                                                                                      ;
      alloc_info.memoryTypeIndex = findMemType( requirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,  device.physicalDevice() ) ;
      alloc_info.pNext           = NULL ;

      HANDLE_ERROR( vkAllocateMemory( device.device(), &alloc_info, NULL, &data().memory ) ) ;
      HANDLE_ERROR( vkBindBufferMemory( device.device(), data().buffer, data().memory, 0 ) ) ;
    }

    void BufferImpl::initializeBase( const Device& device, BufferType type, unsigned element_sz, unsigned count )
    {
      VkBufferCreateInfo info ;

      this->data().device     = device      ;
      this->data().count      = count       ;
      this->data().type       = type        ; 
      this->data().element_sz = element_sz ;

      info             = {}                                   ;
      info.sType       = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO ;
      info.size        = element_sz * count                   ;
      info.usage       = TypeToUsage( type )                  ;
      info.sharingMode = VK_SHARING_MODE_EXCLUSIVE            ;
      info.pNext       = NULL                                 ;
      HANDLE_ERROR( vkCreateBuffer( device.device(), &info, NULL, &this->data().buffer ) ) ;

      this->allocate( device, element_sz * count ) ;
    }

    const VkBuffer& BufferImpl::buffer() const
    {
      return data().buffer ;
    }

    unsigned BufferImpl::size() const
    {
      return data().count ;
    }

    void BufferImpl::copyToDevice( void* data, unsigned offset )
    {
      const unsigned sz = this->data().element_sz * this->data().count ;
      void* mapped_data ;

      vkMapMemory( this->data().device.device(), this->data().memory, 0, sz, 0, &mapped_data ) ;
      std::memcpy( mapped_data, data, (size_t)sz ) ;
      vkUnmapMemory( this->data().device.device(), this->data().memory ) ;
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