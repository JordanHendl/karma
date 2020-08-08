#include "Vulkan.h"
#include "../vk/context/Context.h"
#include "vk/context/CommandBuffer.h"
#define VULKAN_HPP_NO_EXCEPTIONS
#include <vulkan/vulkan.hpp>

  namespace kgl
  {
    namespace vk
    {
      using CommandBuffer = ::kgl::vk::compute::CommandBuffer ;

      uint32_t memType( uint32_t filter, ::vk::MemoryPropertyFlags flag, ::vk::PhysicalDevice device )
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
          
      struct VulkanData
      {
        ::kgl::vk::compute::Context context ;
      };

      kgl::vk::Vulkan::MemoryFlags::MemoryFlags()
      {
        using Flags = ::vk::MemoryPropertyFlagBits ;
        this->flag = static_cast<unsigned>( ( Flags::eHostCoherent | Flags::eHostVisible ) ) ;
      }

      unsigned kgl::vk::Vulkan::MemoryFlags::val()
      {
        return this->flag ;
      }
      
      Vulkan::MemoryFlags& Vulkan::MemoryFlags::operator=( unsigned flags )
      {
        this->flag = ( flags ) ;
        
        return *this ;
      }
      
      Vulkan::BufferFlags::BufferFlags()
      {
        using Flags = ::vk::BufferUsageFlagBits ;
        this->flag = static_cast<unsigned>( ( Flags::eTransferSrc | Flags::eTransferDst ) ) ;
      }
      
      unsigned Vulkan::BufferFlags::val()
      {
        return this->flag ;
      }
      
      Vulkan::BufferFlags& Vulkan::BufferFlags::operator=( unsigned flags )
      {
        this->flag = flags ;
        
        return *this ;
      }

      void Vulkan::initialize()
      {
        if( !::kgl::vk::isInitialized() ) ::kgl::vk::initialize() ;
      }

      unsigned Vulkan::deviceCount()
      {
        ::kgl::vk::compute::Context context ;

        return context.deviceCount() ;
      }

      bool Vulkan::deviceValid( unsigned gpu )
      {
        ::kgl::vk::compute::Context context ;

        return context.validDevice( gpu ) ;
      }

      unsigned Vulkan::convertError( unsigned error )
      {
        return 0 ;
      }

      Vulkan::Vulkan()
      {
        this->vulkan_data = new VulkanData() ;
      }

      Vulkan::~Vulkan()
      {
        delete this->vulkan_data ;
      }
      
      void Vulkan::copyTo( Vulkan::Buffer* src, Vulkan::Buffer* dst, unsigned gpu, unsigned amt )
      {
        CommandBuffer buffer ;
        ::vk::BufferCopy region ;
        ::vk::DeviceSize offset ;

        buffer.initialize( gpu, 1 ) ;
        offset = 0 ;
        region.setSize     ( amt    ) ;
        region.setSrcOffset( offset ) ;
        region.setDstOffset( offset ) ;

        buffer.record() ;
        buffer.buffer(0).copyBuffer( *src, *dst, 1, &region ) ;
        buffer.stop() ;
        buffer.wait() ;
        buffer.reset() ;
      }

      void Vulkan::copyTo( Vulkan::Data src, Vulkan::Memory* dst, unsigned gpu, unsigned amt )
      {
        const auto device = data().context.virtualDevice( gpu ) ;
        ::vk::DeviceSize     offset ;
        ::vk::MemoryMapFlags flag   ;
        void*                mem    ;
        
        offset = 0 ;

        device.mapMemory  ( *dst, offset, amt, flag, &mem        ) ;
        memcpy            ( mem, src, static_cast<size_t>( amt ) ) ;
        device.unmapMemory( *dst                                 ) ;
      }
      
      void Vulkan::copyTo( Vulkan::Memory* src, Vulkan::Data dst, unsigned gpu, unsigned amt )
      {
        const auto device = data().context.virtualDevice( gpu ) ;
        ::vk::DeviceSize     offset ;
        ::vk::MemoryMapFlags flag   ;
        void*                mem    ;
        
        offset = 0 ;
        device.mapMemory  ( *src, offset, amt, flag, &mem        ) ;
        memcpy            ( dst, mem, static_cast<size_t>( amt ) ) ;
        device.unmapMemory( *src                                 ) ;
      }
      
      void Vulkan::free( Vulkan::Buffer* buff, Vulkan::Memory* mem, unsigned gpu )
      {
        const auto device = data().context.virtualDevice( gpu ) ;
        
        device.free   ( *mem  ) ;
        device.destroy( *buff ) ;
        
        delete( buff ) ;
        delete( mem  ) ;
        
        buff = nullptr ;
        mem  = nullptr ;
      }
      
      Vulkan::Memory* Vulkan::createMemory( Buffer* buffer, unsigned gpu, MemoryFlags flags )
      {
        const ::vk::MemoryPropertyFlags flag = static_cast<::vk::MemoryPropertyFlags>( flags.val() ) ;
        const auto device       = data().context.virtualDevice ( gpu ) ;
        const auto p_device     = data().context.physicalDevice( gpu ) ;
        Vulkan::Memory          *mem    ;
        ::vk::MemoryAllocateInfo info   ;
        ::vk::MemoryRequirements req    ;
        ::vk::DeviceSize         offset ;

        mem    = new Vulkan::Memory()                          ;
        req    = device.getBufferMemoryRequirements( *buffer ) ;
        offset = 0                                             ;

        info.setAllocationSize ( req.size                                      ) ;
        info.setMemoryTypeIndex( memType( req.memoryTypeBits, flag, p_device ) ) ;
        
        *mem = device.allocateMemory( info, nullptr ).value ;
        
        device.bindBufferMemory( *buffer, *mem, offset ) ;
        
        return mem ;
      }
      
      Vulkan::Memory* Vulkan::createMemory( Buffer* buffer, unsigned gpu )
      {
        MemoryFlags flags ;
        
        return this->createMemory( buffer, gpu, flags ) ;
      }

      Vulkan::Buffer* Vulkan::createBuffer( unsigned size, unsigned gpu, Vulkan::BufferFlags buff_flags )
      {
        ::vk::BufferUsageFlags flag = static_cast<::vk::BufferUsageFlags>( buff_flags.val() ) ;
        const auto device       = data().context.virtualDevice( gpu ) ;
        const auto sharing_mode = ::vk::SharingMode::eExclusive       ;
        Vulkan::Buffer        *buff ;
        ::vk::BufferCreateInfo info ;
        
        buff = new Vulkan::Buffer() ;

        info.setSize       ( size         ) ;
        info.setUsage      ( flag         ) ;
        info.setSharingMode( sharing_mode ) ;
        
        *buff = device.createBuffer( info, nullptr ).value ;
        
        return buff ;
      }

      Vulkan::Buffer* Vulkan::createBuffer( unsigned size, unsigned gpu )
      {
        Vulkan::BufferFlags buff_flag ;
        
        return this->createBuffer( size, gpu, buff_flag ) ;
      }
      
      
      const VulkanData& Vulkan::data() const
      {
        return *this->vulkan_data ;
      }
      
      VulkanData& Vulkan::data()
      {
        return *this->vulkan_data ;
      }
    }
  }