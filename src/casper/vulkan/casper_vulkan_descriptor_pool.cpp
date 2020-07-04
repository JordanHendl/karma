#include "casper_vulkan_descriptor_pool.h"
#include "casper_vulkan_device.h"
#include "casper_vulkan_window.h"
#include "casper_vulkan_pipeline.h"
#include "casper_vulkan_swapchain.h"
#include "casper_vulkan_shader.h"
#include "casper_vulkan_image.h"
#include "casper_vulkan_error_handler.h"
#include "casper_vulkan_buffer.h"
#include <shader/UWUShader.h>
#include <Bus.h>
#include <Signal.h>
#include <vulkan/vulkan.h>
#include <string>
#include <map>
#include <limits.h>
#include <vector>

namespace casper
{
  namespace vulkan
  {
    typedef std::map<std::string, VkDescriptorPool> PoolMap ;
    
    /**
     */
    static PoolMap pool_map  ;
    
    /**
     * 
     * @return 
     */
    static inline VkDescriptorType typeFromEnum( ::tools::shader::UniformType type ) ;
    
    /**
     * @param value
     * @param size
     * @param device
     * @param p_dev
     * @return 
     */
    static inline VkBuffer generateBuffer( void* value, unsigned size, VkDevice device, VkPhysicalDevice p_dev ) ;
    
    /**
     */
    struct UniformInformation
    {
      unsigned                     count   ;
      unsigned                     binding ;
      ::tools::shader::UniformType type    ;
      
      
      UniformInformation() ;
    };
    
    /**
     */
    struct DescriptorSetData
    {
      typedef std::map<std::string, UniformInformation> UniformMap ;

      std::vector<VkDescriptorSet> descriptors ;
      UniformMap*                  map_ptr     ;
      VkDevice                     device      ;
      VkPhysicalDevice             p_dev       ;

    };
    
    /**
     */
    struct DescriptorPoolData
    {
      typedef std::map<std::string, UniformInformation> UniformMap ;
      
      const VkDescriptorSetLayout* layout ;

      PoolMap::iterator      pool    ;
      UniformMap             uni_map ;
      VkDevice               device  ;
      VkPhysicalDevice       p_dev   ;
      /**
       * @param shader
       */
      void buildUniformData( const Shader& shader ) ;
    };

    VkDescriptorType typeFromEnum( ::tools::shader::UniformType type )
    {
      switch( type )
      {
        case ::tools::shader::UniformType::UBO     : return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER ; break ;
        case ::tools::shader::UniformType::SAMPLER : return VK_DESCRIPTOR_TYPE_SAMPLER        ; break ;
        default :                                    return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER ; break ;
      }
    } 
    
    UniformInformation::UniformInformation()
    {
      this->count   = 0                                  ;
      this->type    = ::tools::shader::UniformType::None ;
      this->binding = UINT_MAX                           ;
    }

    void DescriptorPoolData::buildUniformData( const Shader& shader )
    {
      UniformInformation uni_info ;

      this->uni_map.clear() ;
      this->layout = shader.layout() ;
      for( auto sh  = shader.begin(); sh != shader.end(); ++sh )
      {
        for( unsigned index = 0; index < sh.numUniforms(); index++ )
        {
          auto it = this->uni_map.find( sh.uniformName( index ) ) ;
          
          if( it == this->uni_map.end() )
          {
            uni_info.binding = sh.uniformBinding( index ) ;
            uni_info.type    = sh.uniformType   ( index ) ;
            uni_info.count   = 1                          ;

            this->uni_map.insert({ sh.uniformName( index ), uni_info } ) ;
          }
          else
          {
            it->second.count++ ;
          }
        }
      }
    }

    DescriptorSet::DescriptorSet()
    {
      this->descriptor_set_data = new DescriptorSetData() ;
    }

    DescriptorSet::~DescriptorSet()
    {
      delete this->descriptor_set_data ;
    }

    void DescriptorSet::setImage( const Image& image, const char* name ) const 
    {
      const auto iter = data().map_ptr->find( name ) ;

      if( iter != data().map_ptr->end() )
      {
        VkDescriptorImageInfo image_info ;
        VkWriteDescriptorSet   write_set   ;

        for( unsigned i = 0; i < data().descriptors.size(); i++ )
        {
          image_info.imageView   = image.view()                             ;
          image_info.sampler     = image.sampler()                          ;
          image_info.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL ;

          write_set.sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET    ;
          write_set.dstSet          = data().descriptors[ i ]                   ;
          write_set.dstBinding      = iter->second.binding                      ;
          write_set.dstArrayElement = 0                                         ;
          write_set.descriptorType  = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER ;
          write_set.descriptorCount = 1                                         ;
          write_set.pImageInfo      = &image_info                               ;
          write_set.pNext           = nullptr                                   ;

          vkUpdateDescriptorSets( data().device, 1, &write_set, 0, nullptr ) ;
        }
      }
    }

    void DescriptorSet::setBuffer( const Buffer& buffer, const char* name ) const 
    {
      const auto iter = data().map_ptr->find( name ) ;

      if( iter != data().map_ptr->end() )
      {
        VkDescriptorBufferInfo buffer_info ;
        VkWriteDescriptorSet   write_set   ;

        for( unsigned i = 0; i < data().descriptors.size(); i++ )
        {
          buffer_info.buffer = buffer.buffer()   ;
          buffer_info.offset = 0                 ;
          buffer_info.range  = buffer.byteSize() ;

          write_set.sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET ;
          write_set.dstSet          = data().descriptors[ i ]                ;
          write_set.dstBinding      = iter->second.binding                   ;
          write_set.dstArrayElement = 0                                      ;
          write_set.descriptorType  = typeFromEnum( iter->second.type )      ;
          write_set.descriptorCount = buffer.size()                          ;
          write_set.pBufferInfo     = &buffer_info                           ;

          vkUpdateDescriptorSets( data().device, 1, &write_set, 0, nullptr ) ;
        }
      }
    }
    
//    
//    void DescriptorSet::setBase( void* value, unsigned size, const char* name )
//    {
//      const auto iter = data().map_ptr->find( name ) ;
//      
//      if( iter != data().map_ptr->end() )
//      {
//        VkDescriptorBufferInfo buffer_info ;
//        VkWriteDescriptorSet   write_set   ;
//        
//        for( unsigned i = 0; i < data().descriptors.size(); i++ )
//        {
//          buffer_info.buffer = staging_buffer.buffer() ;
//          buffer_info.offset = 0                       ;
//          buffer_info.range  = size                    ;
//
//          write_set.sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET ;
//          write_set.dstSet          = data().descriptors[ i ]                ;
//          write_set.dstBinding      = iter->second.binding                   ;
//          write_set.dstArrayElement = 0                                      ;
//          write_set.descriptorType  = typeFromEnum( iter->second.type )      ;
//          write_set.descriptorCount = 1                                      ;
//          write_set.pBufferInfo     = &buffer_info                           ;
//
//          HANDLE_ERROR( vkUpdateDescriptorSets( data().device, 1, &write_set, 0, nullptr ) ) ;
//        }
//      }
//    }

    const VkDescriptorSet* DescriptorSet::operator[]( unsigned id ) const
    {
      return this->get( id ) ;
    }

    DescriptorSet::DescriptorSet( const DescriptorSet& set )
    {
      this->descriptor_set_data = new DescriptorSetData() ;
      *this->descriptor_set_data = *set.descriptor_set_data ;
    }

    void DescriptorSet::operator=( const DescriptorSet& set )
    {
      *this->descriptor_set_data = *set.descriptor_set_data ;
    }

    const VkDescriptorSet* DescriptorSet::get( unsigned id ) const
    {
      static VkDescriptorSet dummy ;
      return id < data().descriptors.size() ? &data().descriptors[ id ] : &dummy ;
    }

    DescriptorSetData& DescriptorSet::data()
    {
      return *this->descriptor_set_data ;
    }

    const DescriptorSetData& DescriptorSet::data() const
    {
      return *this->descriptor_set_data ;
    }

    DescriptorPool::DescriptorPool()
    {
      this->pool_data = new DescriptorPoolData() ;
    }

    DescriptorPool::~DescriptorPool()
    {
      delete this->pool_data ;
    }

    DescriptorSet DescriptorPool::makeDescriptorSet( unsigned count )
    {
      const std::vector<VkDescriptorSetLayout> layouts( count, *data().layout ) ;
      VkDescriptorSetAllocateInfo info ;
      DescriptorSet               set  ;

      info = {} ;
      info.sType              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO ;
      info.descriptorPool     = data().pool->second                            ;
      info.descriptorSetCount = count                                          ;
      info.pSetLayouts        = layouts.data()                                 ;
      info.pNext              = nullptr                                        ;

      set.data().descriptors.resize( count ) ;
      set.data().device  = data().device     ;
      set.data().map_ptr = &data().uni_map   ;

      HANDLE_ERROR( vkAllocateDescriptorSets( data().device, &info, &set.data().descriptors[0]) ) ;

      return set ;
    }

    void DescriptorPool::initialize( const Window& window, const Shader& shader, const char* pipeline_name )
    {
      const unsigned numFramebuffers = window.chain().numBuffers() ;

      VkDescriptorPoolCreateInfo        info    ;
      std::vector<VkDescriptorPoolSize> size    ;
      
     
      info = {} ;
      size = {} ;
      
      data().device = window.device().device() ;
      
      data().buildUniformData( shader ) ;
      for( auto desc : data().uni_map )
      {
        VkDescriptorPoolSize sz                               ;
        sz.type            = typeFromEnum( desc.second.type ) ;
        sz.descriptorCount = numFramebuffers                  ;
        size.push_back( sz )                                  ;
      }

      info.sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO ;
      info.poolSizeCount = size.size()                                   ;
      info.pPoolSizes    = size.data()                                   ;
      info.maxSets       = numFramebuffers                               ;

      if( pool_map.find( pipeline_name ) == pool_map.end() ) pool_map.insert( { std::string( pipeline_name ), VkDescriptorPool() } ) ;

      data().pool = pool_map.find( pipeline_name ) ;
      HANDLE_ERROR( vkCreateDescriptorPool( data().device, &info, nullptr, &data().pool->second ) ) ;
    }

    DescriptorPoolData& DescriptorPool::data()
    {
      return *this->pool_data ;
    }

    const DescriptorPoolData& DescriptorPool::data() const
    {
      return *this->pool_data ;
    }
  }
}