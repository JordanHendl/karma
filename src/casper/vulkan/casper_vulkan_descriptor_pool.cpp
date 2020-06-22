#include "casper_vulkan_descriptor_pool.h"
#include "casper_vulkan_device.h"
#include "casper_vulkan_error_handler.h"
#include <Bus.h>
#include <Signal.h>
#include <vulkan/vulkan.h>
#include <string>
#include <map>
#include <vector>

namespace casper
{
  namespace vulkan
  {
    typedef std::map<std::string, VkDescriptorPool>    PoolMap                                                   ;
    typedef unsigned                                   CountArray[ DescriptorPool::DescriptorType::TOTAL_TYPES ] ;
    typedef std::map<std::string, CountArray>          PipeCountMap                                              ;

    static PoolMap      pool_map  ;
    static PipeCountMap count_map ;

    static VkDescriptorType typeFromEnum( DescriptorPool::DescriptorType type )
    {
      switch( type )
      {
        case DescriptorPool::DescriptorType::UNIFORM : return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER ; break ;
        case DescriptorPool::DescriptorType::SAMPLER : return VK_DESCRIPTOR_TYPE_SAMPLER        ; break ;
        default : return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER ; break ;
      }
    } 
    DescriptorPool::DescriptorPool()
    {

    }
    DescriptorPool::~DescriptorPool()
    {

    }

    void DescriptorPool::initialize( const Device& device, const char* pipeline_name, unsigned num_layouts, const VkDescriptorSetLayout* layout )
    {
      VkDescriptorPoolCreateInfo        info ;
      std::vector<VkDescriptorPoolSize> size ;
      // CountMap                          map  ;
      

      info               = {} ;
      size               = {} ;

      for( unsigned i = 0; i < num_layouts; i++ )
      {
        VkDescriptorPoolSize sz ;
        // sz.type = layout[ i ].
        // size.type = typeFromEnum(  layout[i].) ;
        //  size.at
      }

      // info.pPoolSizes    =  ;

      info.poolSizeCount = 1  ;

      if( pool_map.find( pipeline_name ) == pool_map.end() ) pool_map.insert( { std::string( pipeline_name ), VkDescriptorPool() } ) ;

      // HANDLE_ERROR( vkCreateDescriptorPool( data().device, pool_map[ id ]

    }

    void DescriptorPool::setAmount( const char* name, DescriptorPool::DescriptorType type, unsigned count )
    {
      count_map[ name ][ type ] =  count ;
    }

  }
}