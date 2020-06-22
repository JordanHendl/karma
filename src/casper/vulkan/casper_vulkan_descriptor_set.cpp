#include "casper_vulkan_descriptor_set.h"
#include "casper_vulkan_device.h"
#include "casper_vulkan_error_handler.h"
#include <Bus.h>
#include <Signal.h>
#include <vulkan/vulkan.h>
#include <map>

namespace casper
{
  namespace vulkan
  {

    static std::map<unsigned, VkDescriptorPool> pool_map ;

    static VkDescriptorPoolCreateInfo createPoolInfo( unsigned num_frames )
    {

    }

    struct DescriptorSetData
    {
      casper::module::Bus bus    ;
      VkDevice            device ;

      void setDevice( const VkDevice& device )
      {
        this->device = device ;
      }
      
    };

    DescriptorSet::DescriptorSet()
    {

    }
    DescriptorSet::~DescriptorSet()
    {

    }

    void DescriptorSet::initialize( unsigned count, const VkDescriptorSetLayout* layout )
    {
      const unsigned id = data().bus.id ;

      VkDescriptorPoolCreateInfo        info ;
      std::vector<VkDescriptorPoolSize> size ;

      

      info               = {} ;
      // info.pPoolSizes    =  ;
      info.poolSizeCount = 1  ;
      if( pool_map.find( id ) == pool_map.end() ) pool_map.insert( { id, VkDescriptorPool() } ) ;

      HANDLE_ERROR( vkCreateDescriptorPool( data().device, pool_map[ id ]

    }

    void DescriptorSet::subscribe( unsigned id )
    {
      data().bus.setId( id ) ;
      
      data().bus[ "vk_device" ].attach( this->descriptor_set_data, &DescriptorSetData::setDevice ) ;
    }

    void DescriptorSet::bind( const char* command_buffer_name )
    {

    }

    DescriptorSetData& DescriptorSet::data()
    {
      return *this->descriptor_set_data ;
    }

    const DescriptorSetData& DescriptorSet::data() const
    {
      return *this->descriptor_set_data ;
    }
  }
}