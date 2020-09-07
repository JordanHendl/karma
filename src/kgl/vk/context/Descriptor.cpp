#include "Descriptor.h"
#include "Context.h"
#include "CommandBuffer.h"
#include "Shader.h"
#include "Uniform.h"
#include "Buffer.h"
#include "Image.h"
#include "../../../tools/shader/UWUShader.h"
#include <vulkan/vulkan.hpp>
#include <map>

namespace kgl
{
  namespace vk
  {
    typedef std::map<std::string, ::vk::DescriptorPool> PoolMap ;
    
    static PoolMap pool_map ;
    
    
    static inline ::vk::DescriptorType typeFromEnum( ::tools::shader::UniformType type ) ;
    
    struct UniformInformation
    {
      unsigned                     count   ;
      unsigned                     binding ;
      ::tools::shader::UniformType type    ;
      
      
      UniformInformation() ;
    };

    struct DescriptorSetData
    {
      struct DescriptorBuffer
      {
        unsigned                     count   ;
        unsigned                     binding ;
        ::tools::shader::UniformType type    ;
        ::kgl::vk::Buffer            buffer  ;
        ::kgl::vk::Image             image   ;
      };
      
      typedef std::map<std::string, UniformInformation> UniformMap ;
      
      std::vector<::vk::DescriptorSet> descriptors ;
      
      UniformMap                       map      ;
      ::vk::Device                     device   ;
      ::vk::PhysicalDevice             p_device ;
      ::vk::DescriptorPool             pool     ;
      unsigned                         gpu      ;
      
      void buildBufferMap( UniformMap& map ) ;
      void setBuffer( ::kgl::vk::Buffer& buffer, DescriptorBuffer& uni ) ;
      void setBuffer( const ::kgl::vk::Buffer& buffer, UniformInformation& uni ) ;
      void setImage( ::kgl::vk::Image& buffer, DescriptorBuffer& uni ) ;
      void setImage( const ::kgl::vk::Image& buffer, UniformInformation& uni ) ;
    }; 
    
    struct DescriptorPoolData
    {
      typedef std::map<std::string, UniformInformation> UniformMap ;
      
      ::vk::DescriptorSetLayout layout ;
      
      ::vk::DescriptorPool   pool    ;
      UniformMap             uni_map ;
      ::vk::Device           device  ;
      ::vk::PhysicalDevice   p_dev   ;
      unsigned               gpu     ;

      void buildUniformData( const ::tools::shader::UWUShader& shader ) ;
      void buildUniformData( const Shader& shader ) ;
    };
    
    ::vk::DescriptorType typeFromEnum( ::tools::shader::UniformType type )
    {
      ::vk::DescriptorType ret_type ;
      switch( type )
      {
        case ::tools::shader::UniformType::UBO     : ret_type =  ::vk::DescriptorType::eUniformBuffer        ; break ;
        case ::tools::shader::UniformType::SAMPLER : ret_type =  ::vk::DescriptorType::eCombinedImageSampler ; break ;
        case ::tools::shader::UniformType::SSBO    : ret_type =  ::vk::DescriptorType::eStorageBuffer        ; break ;
        default :                                    ret_type =  ::vk::DescriptorType::eUniformBuffer        ; break ;
      }
      
      return ret_type ;
    } 
    
    void DescriptorSetData::setBuffer( ::kgl::vk::Buffer& buffer, DescriptorBuffer& uni )
    {
      ::vk::DescriptorBufferInfo info  ;
      ::vk::WriteDescriptorSet   write ;
      
      for( unsigned i = 0; i < this->descriptors.size(); i++ )
      {
        info.setBuffer( buffer.buffer()   ) ;
        info.setOffset( 0                 ) ;
        info.setRange ( buffer.byteSize() ) ;

        write.setDstSet         ( this->descriptors[ i ]   ) ;
        write.setDstBinding     ( uni.binding              ) ;
        write.setDstArrayElement( 0                        ) ;
        write.setDescriptorType ( typeFromEnum( uni.type ) ) ;
        write.setDescriptorCount( 1                        ) ;
        write.setPBufferInfo    ( &info                    ) ;
        
        this->device.updateDescriptorSets( 1, &write, 0, nullptr ) ;
      }
    }

    void DescriptorSetData::setBuffer( const ::kgl::vk::Buffer& buffer, UniformInformation& uni )
    {
      ::vk::DescriptorBufferInfo info  ;
      ::vk::WriteDescriptorSet   write ;
      
      for( unsigned i = 0; i < this->descriptors.size(); i++ )
      {
        info.setBuffer( buffer.buffer()   ) ;
        info.setOffset( 0                 ) ;
        info.setRange ( buffer.byteSize() ) ;
        
        write.setDstSet         ( this->descriptors[ i ]   ) ;
        write.setDstBinding     ( uni.binding              ) ;
        write.setDstArrayElement( 0                        ) ;
        write.setDescriptorType ( typeFromEnum( uni.type ) ) ;
        write.setDescriptorCount( 1                        ) ;
        write.setPBufferInfo    ( &info                    ) ;
        
        this->device.updateDescriptorSets( 1, &write, 0, nullptr ) ;
      }
    }
    
    void DescriptorSetData::setImage( ::kgl::vk::Image& image, DescriptorBuffer& uni )
    {
      const auto layout = image.layout() ;
      ::vk::DescriptorImageInfo info  ;
      ::vk::WriteDescriptorSet  write ;
      
      for( unsigned i = 0; i < this->descriptors.size(); i++ )
      {
        info.setImageView  ( image.view()    ) ;
        info.setSampler    ( image.sampler() ) ;
        info.setImageLayout( layout          ) ;

        write.setDstSet         ( this->descriptors[ i ]   ) ;
        write.setDstBinding     ( uni.binding              ) ;
        write.setDstArrayElement( 0                        ) ;
        write.setDescriptorType ( typeFromEnum( uni.type ) ) ;
        write.setDescriptorCount( 1                        ) ;
        write.setPImageInfo     ( &info                    ) ;
        
        this->device.updateDescriptorSets( 1, &write, 0, nullptr ) ;
      }
    }

    void DescriptorSetData::setImage( const ::kgl::vk::Image& image, UniformInformation& uni )
    {
      const auto layout = ::vk::ImageLayout::eGeneral ;
      
      ::vk::DescriptorImageInfo info  ;
      ::vk::WriteDescriptorSet  write ;
      
      for( unsigned i = 0; i < this->descriptors.size(); i++ )
      {
        info.setImageView  ( image.view()    ) ;
        info.setSampler    ( image.sampler() ) ;
        info.setImageLayout( layout          ) ;
        
        write.setDstSet         ( this->descriptors[ i ]   ) ;
        write.setDstBinding     ( uni.binding              ) ;
        write.setDstArrayElement( 0                        ) ;
        write.setDescriptorType ( typeFromEnum( uni.type ) ) ;
        write.setDescriptorCount( 1                        ) ;
        write.setPImageInfo     ( &info                    ) ;
        
        this->device.updateDescriptorSets( 1, &write, 0, nullptr ) ;
      }

    }

    void DescriptorSetData::buildBufferMap( UniformMap& map )
    {
      this->map = map ;
    }
    
    UniformInformation::UniformInformation()
    {
      this->binding = 0 ;
      this->count   = 0 ;
    }
    DescriptorSet::DescriptorSet()
    {
      this->set_data = new DescriptorSetData() ;
    }

    DescriptorSet::DescriptorSet( const DescriptorSet& set )
    {
      this->set_data = new DescriptorSetData() ;
      
      *this->set_data = *set.set_data ;
    }

    DescriptorSet::~DescriptorSet()
    {
      delete this->set_data ;
    }

    void DescriptorSet::operator=( const DescriptorSet& set )
    {
      *this->set_data = *set.set_data ;
    }

    void DescriptorSet::set( const Uniform& uniform )
    {
      for( auto iter = uniform.begin(); iter != uniform.end(); ++iter )
      {
        for( auto set : data().map )
        {
          if( std::string( iter.name() ) == set.first )
          {
            if( iter.type() == Uniform::Type::Sampler )
            {
              data().setImage( iter.image(), set.second ) ;
            }
            else
            {
              data().setBuffer( iter.buffer(), set.second ) ;
            }
          }
        }
      }
    }
    
    void DescriptorSet::reset()
    {
      data().device.freeDescriptorSets( data().pool, data().descriptors.size(), data().descriptors.data() ) ;
    }
    
    void DescriptorPoolData::buildUniformData( const Shader& shader )
    {
      UniformInformation uni_info ;

      this->uni_map.clear() ;
      this->layout = shader.layout() ;
      for( auto sh = shader.begin(); sh != shader.end(); ++sh )
      {
        for( unsigned index = 0; index < sh.numUniforms(); index++ )
        {
          auto it = this->uni_map.find( sh.uniformName( index ) ) ;
          
          if( it == this->uni_map.end() )
          {
            uni_info.binding = sh.uniformBinding( index ) ;
            uni_info.type    = sh.uniformType   ( index ) ;
            uni_info.count   = sh.uniformSize   ( index ) ;

            this->uni_map.insert({ sh.uniformName( index ), uni_info } ) ;
          }
          else
          {
            it->second.count++ ;
          }
        }
      }
    }

    const ::vk::DescriptorSet DescriptorSet::descriptor( unsigned index ) const
    {
      static const ::vk::DescriptorSet dummy ;

      return index < data().descriptors.size() ? data().descriptors[ index ] : dummy ;
    }

    DescriptorSetData& DescriptorSet::data()
    {
      return *this->set_data ;
    }

    const DescriptorSetData& DescriptorSet::data() const
    {
      return *this->set_data ;
    }

    DescriptorPool::DescriptorPool()
    {
      this->pool_data = new DescriptorPoolData() ;
    }

    DescriptorPool::~DescriptorPool()
    {
      delete this->pool_data ;
    }

    const ::vk::DescriptorPool DescriptorPool::pool() const
    {
      return data().pool ;
    }
    
    void DescriptorPool::reset() const
    {
      data().device.resetDescriptorPool( data().pool ) ;
    }
    
    DescriptorSet DescriptorPool::makeDescriptorSet( unsigned count )
    {
      const std::vector<::vk::DescriptorSetLayout> layouts( count, data().layout ) ;
      ::vk::DescriptorSetAllocateInfo info ;
      DescriptorSet                   set  ;

      info.setDescriptorPool    ( data().pool    ) ;
      info.setDescriptorSetCount( count          ) ;
      info.setPSetLayouts       ( layouts.data() ) ;

      set.data().descriptors.resize( count ) ;

      set.data().pool        = data().pool                                  ;
      set.data().device      = data().device                                ;
      set.data().descriptors = data().device.allocateDescriptorSets( info ) ;
      set.data().p_device    = data().p_dev                                 ;
      set.data().gpu         = data().gpu                                   ;
      set.data().buildBufferMap( data().uni_map ) ;
      return set ;
    }

    void DescriptorPool::initialize( unsigned gpu, unsigned max_amt, const Shader& shader )
    {
      const ::kgl::vk::compute::Context context ;
      ::vk::DescriptorPoolCreateInfo    info ;
      std::vector<::vk::DescriptorPoolSize> size ;
      size = {} ;
      
      data().gpu    = gpu                           ;
      data().device = context.virtualDevice ( gpu ) ;
      data().p_dev  = context.physicalDevice( gpu ) ;
      
      data().buildUniformData( shader ) ;

      for( auto desc : data().uni_map )
      {
        ::vk::DescriptorPoolSize sz                    ;
        sz.setType( typeFromEnum( desc.second.type ) ) ;
        sz.setDescriptorCount( max_amt               ) ;
        size.push_back( sz )                           ;
      }

      info.setPoolSizeCount( size.size()                                            ) ;
      info.setPPoolSizes   ( size.data()                                            ) ;
      info.setMaxSets      ( max_amt                                                ) ;
      info.setFlags        ( ::vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet ) ;

      data().pool = data().device.createDescriptorPool( info, nullptr ) ;
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
