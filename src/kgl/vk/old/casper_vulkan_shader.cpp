#include "casper_vulkan_shader.h"
#include "casper_vulkan_error_handler.h"
#include "Device.h"
#include <vulkan/vulkan.h>
#include <shader/UWUShader.h>
#include <vector>
#include <string> 
#include <limits.h>
#include <iostream>
#include <map>

namespace casper
{
  namespace vulkan
  {
    /**
     * @param desc_type
     * @param type
     */
    static inline void operator<<( VkDescriptorType& desc_type, const ::tools::shader::UniformType& type ) ;
    
    /**
     * @param stage
     */
    static inline VkShaderStageFlagBits vulkanBitFromStage( const ::tools::shader::ShaderStage& stage ) ;
    
    /**
     * @param type
     * @return 
     */
    static inline unsigned numIterationsFromType( const char* type ) ;
    
    /**
     * @param type
     * @return 
     */
    static inline VkFormat formatFromAttributeType( const char* type ) ;
    
    /**
     * @param it
     * @param index
     * @return 
     */
    static inline unsigned byteSizeFromFormat( ::tools::shader::ShaderIterator& it, unsigned index ) ;

    /**
     */
    struct ShaderData
    {
      typedef std::map<::tools::shader::ShaderStage, VkShaderModule> ShaderModules         ;
      typedef std::vector<VkVertexInputAttributeDescription>         AttributeDescriptions ;
      typedef std::vector<VkVertexInputBindingDescription>           BindingDescriptions   ;
      
      ShaderModules                        modules      ; ///< TODO
      ::tools::shader::ShaderIterator      begin        ; ///< TODO
      ::tools::shader::ShaderIterator      end          ; ///< TODO
      VkDescriptorSetLayout                layout       ; ///< Descriptor Layout for this shader.
      ::tools::shader::UWUShader           compiler     ; ///< TODO
      VkDevice                             gpu          ; ///< TODO
      VkPipelineVertexInputStateCreateInfo vertex_info  ; ///< TODO
      AttributeDescriptions                attr_desc    ;
      BindingDescriptions                  bind_desc    ;
      
      /**
       */
      ShaderData() ;
      
      /**
       */
      void generateVulkanLayer() ;
      
      /**
       */
      void generateShaderModules() ;
      
      /**
       */
      void generateAttributeInfo() ;
    };
    
    unsigned numIterationsFromType( const char* type )
    {
      const std::string t = std::string( type ) ;
      
           if( t == "mat4" ) return 4 ;
      else if( t == "mat3" ) return 3 ;
      else if( t == "mat2" ) return 2 ;
      else                   return 1 ;
    }
          
    unsigned byteSizeFromFormat( ::tools::shader::ShaderIterator& it, unsigned index )
    {
      const std::string s = std::string( it.attributeType( index ) ) ;
      
           if( s == "mat4" ) return 4 * sizeof( float ) ;
      else if( s == "mat3" ) return 3 * sizeof( float ) ;
      else if( s == "mat2" ) return 3 * sizeof( float ) ;
      
      else return it.attributeByteSize( index ) ;
    }

    VkFormat formatFromAttributeType( const char* type )
    {
      const std::string t = std::string( type ) ;
      
           if( t == "mat4"  ) return VK_FORMAT_R32G32B32A32_SFLOAT ;
      else if( t == "vec4"  ) return VK_FORMAT_R32G32B32A32_SFLOAT ;
      else if( t == "ivec4" ) return VK_FORMAT_R32G32B32A32_SINT   ;
      else if( t == "uvec4" ) return VK_FORMAT_R32G32B32A32_UINT   ;
      else if( t == "mat3"  ) return VK_FORMAT_R32G32B32_SFLOAT    ;
      else if( t == "vec3"  ) return VK_FORMAT_R32G32B32_SFLOAT    ;
      else if( t == "ivec3" ) return VK_FORMAT_R32G32B32_SINT      ;
      else if( t == "uvec3" ) return VK_FORMAT_R32G32B32_UINT      ;
      else if( t == "mat2"  ) return VK_FORMAT_R32G32_SFLOAT       ;
      else if( t == "vec2"  ) return VK_FORMAT_R32G32_SFLOAT       ;
      else if( t == "ivec2" ) return VK_FORMAT_R32G32_SINT         ;
      else if( t == "uvec2" ) return VK_FORMAT_R32G32_UINT         ;
      
      return VK_FORMAT_R32_SFLOAT ;
    }

    VkShaderStageFlagBits vulkanBitFromStage( const ::tools::shader::ShaderStage& stage )
    {
      switch( stage )
      {
        case ::tools::shader::ShaderStage::FRAGMENT      : return VK_SHADER_STAGE_FRAGMENT_BIT                ;
        case ::tools::shader::ShaderStage::GEOMETRY      : return VK_SHADER_STAGE_GEOMETRY_BIT                ;
        case ::tools::shader::ShaderStage::TESSALATION_C : return VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT    ;
        case ::tools::shader::ShaderStage::TESSELATION_E : return VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT ;
        case ::tools::shader::ShaderStage::COMPUTE       : return VK_SHADER_STAGE_COMPUTE_BIT                 ;
        case ::tools::shader::ShaderStage::VERTEX        : return VK_SHADER_STAGE_VERTEX_BIT                  ;
        default                                          : return VK_SHADER_STAGE_VERTEX_BIT                  ;
      }
    }

    void operator<<( VkDescriptorType& desc_type, const ::tools::shader::UniformType& type )
    {
      switch( type )
      {
        case ::tools::shader::UniformType::UBO     : desc_type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER         ; break ;
        case ::tools::shader::UniformType::SAMPLER : desc_type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER ; break ;
        case ::tools::shader::UniformType::SSBO    : desc_type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER         ; break ;
        case ::tools::shader::UniformType::None    : desc_type = VK_DESCRIPTOR_TYPE_END_RANGE              ; break ;
      }
    }
    
    void ShaderData::generateAttributeInfo()
    {
      VkVertexInputAttributeDescription attr ;
      VkVertexInputBindingDescription   bind ;
      unsigned offset ;
      
      offset = 0 ; 

      for( auto sh = this->compiler.begin(); sh != this->compiler.end(); ++sh )
      {
        if( sh.stage() == ::tools::shader::ShaderStage::VERTEX )
        {
          for( unsigned index = 0; index < sh.numAttributes(); index++ )
          {
            if( sh.attributeIsInput( index ) )
            {
              /** This is done in the case that a matrix ( or value greater than a vec4 ) is an input.
               */
              for( unsigned num_iter = 0 ; num_iter < numIterationsFromType( sh.attributeType( index ) ); num_iter++ )
              {
                attr.location = sh.attributeLocation( index ) + num_iter ;
                attr.binding  = 0 ; /// Problem? What is this?
                attr.format   = formatFromAttributeType( sh.attributeType( index ) ) ;
                attr.offset   = offset ;
                
                this->attr_desc.push_back( attr ) ;
                offset += byteSizeFromFormat( sh, index ) ;
              }
            }
          }
        }
      }

      bind.binding   = 0                           ;
      bind.inputRate = VK_VERTEX_INPUT_RATE_VERTEX ;
      bind.stride    = offset                      ;
      
      this->bind_desc.push_back( bind ) ;
    }

    void ShaderData::generateVulkanLayer()
    {
      VkDescriptorSetLayoutCreateInfo                     info        ; ///< TODO
      std::map<std::string, VkDescriptorSetLayoutBinding> binding_map ; ///< TODO
      std::vector<VkDescriptorSetLayoutBinding>           bindings    ; ///< TODO
      VkDescriptorSetLayoutBinding                        binding     ; ///< TODO
      unsigned                                            iterator    ; ///< TODO

      iterator = 0 ;
      
      /* Build mapping of uniforms to name. This is so we can easily get shader stage bitmask, as
       * the same uniform may be present in multiple shader stages.
       */ 
      for( auto iter = this->compiler.begin(); iter != this->compiler.end(); ++iter )
      {
        for( unsigned index = 0; index < iter.numUniforms(); index++ )
        {
          if( binding_map.find( iter.uniformName( index ) ) != binding_map.end() ) 
          {
            binding_map[ iter.uniformName( index) ].stageFlags |= vulkanBitFromStage( iter.stage() ) ;
          }
          else
          {
            binding                 = {}                                    ;
            binding.binding         = iter.uniformBinding( index )          ;
            binding.descriptorCount = iter.uniformSize   ( index )          ;          
            binding.stageFlags      |= vulkanBitFromStage( iter.stage() )   ;
            binding.descriptorType  << iter.uniformType( index )            ;

            binding_map[ iter.uniformName( index ) ] = binding ;
          }
        }
      }
      
      // Now, we need data in flat format, so fill mapping into an array.
      bindings.resize( binding_map.size() ) ;
      for( auto bind : binding_map )
      {
        bindings[ iterator ] = bind.second ;
        iterator++ ;
      }
      
      info              = {}                                                  ;
      info.sType        = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO ;
      info.bindingCount = bindings.size()                                     ;
      info.pBindings    = bindings.data()                                     ;
      info.pNext        = NULL                                                ;
      
      HANDLE_ERROR( vkCreateDescriptorSetLayout( this->gpu, &info, nullptr, &this->layout ) ) ;
    }

    void ShaderData::generateShaderModules()
    {
      VkShaderModuleCreateInfo        info    ;
      VkShaderModule                  mod     ;

      info       = {}                                          ;
      info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO ;
      info.pNext = nullptr                                     ;      
      this->modules.clear() ;

      for( auto shader = this->compiler.begin(); shader != this->compiler.end(); ++shader )
      {
        info.codeSize = shader.spirvSize() * sizeof( unsigned ) ;
        info.pCode    = shader.spirv()                          ;
  
        HANDLE_ERROR( vkCreateShaderModule( this->gpu, &info, nullptr, &mod ) ) ;
        this->modules.insert( { shader.stage(), mod } ) ;
      }
    }
    
    ShaderData::ShaderData()
    {
    }

    Shader::Shader()
    {
      this->shader_data = new ShaderData() ;
    }

    Shader::~Shader()
    {
      delete this->shader_data ;
    }
    
    void Shader::load( const char* path )
    {
      data().compiler.load( path ) ;
    }

    const VkDescriptorSetLayout* Shader::layout() const
    {
      return &data().layout ;
    }

    const VkShaderModule& Shader::module( ::tools::shader::ShaderStage index ) const
    {
      static VkShaderModule dummy ;
      return data().modules.find( index ) != data().modules.end() ? data().modules.at( index ) : dummy ;
    }

    void Shader::read( const Device& device, const char* path )
    {
      data().gpu   = device.device()         ;
      data().compiler.load( path )           ;
      data().begin = data().compiler.begin() ;
      data().end   = data().compiler.end  () ;

      data().generateVulkanLayer  () ;
      data().generateShaderModules() ;
      data().generateAttributeInfo() ;
    }
    const VkVertexInputAttributeDescription* Shader::attributeVertexDesc() const
    {
      return data().attr_desc.data() ;
    }

    const VkVertexInputBindingDescription* Shader::bindingVertexDesc() const
    {
      return data().bind_desc.data() ;
    }

    unsigned Shader::numVertexAttr() const
    {
      return data().bind_desc.size() ;
    }

    unsigned Shader::numVertexDesc() const
    {
      return data().attr_desc.size() ;
    }

    const ::tools::shader::ShaderIterator& Shader::begin() const
    {
      return data().begin ;
    }

    const ::tools::shader::ShaderIterator& Shader::end() const
    {
      return data().end ;
    }

    ShaderData& Shader::data()
    {
      return *this->shader_data ;
    }

    const ShaderData& Shader::data() const
    {
      return *this->shader_data ;
    }
  }
}