#include "Shader.h"
#include "Context.h"
#include <shader/UWUShader.h>
#include <vulkan/vulkan.hpp>
#include <map>
#include <vector>
namespace kgl
{
  namespace vk
  {
    /**
     * @param desc_type
     * @param type
     */
    static inline void operator<<( ::vk::DescriptorType& desc_type, const ::tools::shader::UniformType& type ) ;
    
    /**
     * @param stage
     */
    static inline ::vk::ShaderStageFlagBits vulkanBitFromStage( const ::tools::shader::ShaderStage& stage ) ;
    
    /**
     * @param type
     * @return 
     */
    static inline unsigned numIterationsFromType( const char* type ) ;
    
    /**
     * @param type
     * @return 
     */
    static inline ::vk::Format formatFromAttributeType( const char* type ) ;
    
    /**
     * @param it
     * @param index
     * @return 
     */
    static inline unsigned byteSizeFromFormat( ::tools::shader::ShaderIterator& it, unsigned index ) ;

    struct ShaderData
    {
      typedef std::map<::tools::shader::ShaderStage, ::vk::ShaderModule> ShaderModules         ;
      typedef std::vector<::vk::VertexInputAttributeDescription>         AttributeDescriptions ;
      typedef std::vector<::vk::VertexInputBindingDescription>           BindingDescriptions   ;
      typedef std::vector<::vk::PipelineShaderStageCreateInfo>           ShaderStageInfos      ;

      ShaderModules                            modules     ; ///< TODO
      AttributeDescriptions                    attr_desc   ; ///< TODO
      BindingDescriptions                      bind_desc   ; ///< TODO
      ShaderStageInfos                         stage_infos ; ///< TODO
      ::vk::DescriptorSetLayout                layout      ; ///< Descriptor Layout for this shader.
      ::tools::shader::ShaderIterator          begin       ; ///< TODO
      ::tools::shader::ShaderIterator          end         ; ///< TODO
      ::tools::shader::UWUShader               compiler    ; ///< TODO
      ::vk::PipelineVertexInputStateCreateInfo vertex_info ; ///< TODO
      ::vk::Device                             gpu         ; ///< TODO
      
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
      
      /**
       */
      void generatePipelineShaderInfo() ;
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

    ::vk::Format formatFromAttributeType( const char* type )
    {
      const std::string t = std::string( type ) ;
      
           if( t == "mat4"  ) return ::vk::Format::eR32G32B32A32Sfloat ;
      else if( t == "vec4"  ) return ::vk::Format::eR32G32B32A32Sfloat ;
      else if( t == "ivec4" ) return ::vk::Format::eR32G32B32A32Sint   ;
      else if( t == "uvec4" ) return ::vk::Format::eR32G32B32A32Uint   ;
      else if( t == "mat3"  ) return ::vk::Format::eR32G32B32A32Sfloat ;
      else if( t == "vec3"  ) return ::vk::Format::eR32G32B32A32Sfloat ;
      else if( t == "ivec3" ) return ::vk::Format::eR32G32B32A32Sint   ;
      else if( t == "uvec3" ) return ::vk::Format::eR32G32B32A32Uint   ;
      else if( t == "mat2"  ) return ::vk::Format::eR32G32Sfloat       ;
      else if( t == "vec2"  ) return ::vk::Format::eR32G32Sfloat       ;
      else if( t == "ivec2" ) return ::vk::Format::eR32G32Sint         ;
      else if( t == "uvec2" ) return ::vk::Format::eR32G32Uint         ;
      
      return ::vk::Format::eR32Sfloat ;
    }

    ::vk::ShaderStageFlagBits vulkanBitFromStage( const ::tools::shader::ShaderStage& stage )
    {
      switch( stage )
      {
        case ::tools::shader::ShaderStage::FRAGMENT      : return ::vk::ShaderStageFlagBits::eFragment               ;
        case ::tools::shader::ShaderStage::GEOMETRY      : return ::vk::ShaderStageFlagBits::eGeometry               ;
        case ::tools::shader::ShaderStage::TESSALATION_C : return ::vk::ShaderStageFlagBits::eTessellationControl    ;
        case ::tools::shader::ShaderStage::TESSELATION_E : return ::vk::ShaderStageFlagBits::eTessellationEvaluation ;
        case ::tools::shader::ShaderStage::COMPUTE       : return ::vk::ShaderStageFlagBits::eCompute                ;
        case ::tools::shader::ShaderStage::VERTEX        : return ::vk::ShaderStageFlagBits::eVertex                 ;
        default                                          : return ::vk::ShaderStageFlagBits::eFragment               ;
      }
    }

    void operator<<( ::vk::DescriptorType& desc_type, const ::tools::shader::UniformType& type )
    {
      switch( type )
      {
        case ::tools::shader::UniformType::UBO     : desc_type = ::vk::DescriptorType::eUniformBuffer        ; break ;
        case ::tools::shader::UniformType::SAMPLER : desc_type = ::vk::DescriptorType::eCombinedImageSampler ; break ;
        case ::tools::shader::UniformType::SSBO    : desc_type = ::vk::DescriptorType::eStorageBuffer        ; break ;
        case ::tools::shader::UniformType::None    : desc_type = ::vk::DescriptorType::eUniformBuffer        ; break ;
      }
    }

    void ShaderData::generateAttributeInfo()
    {
      ::vk::VertexInputAttributeDescription attr ;
      ::vk::VertexInputBindingDescription   bind ;
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
                attr.setLocation( sh.attributeLocation( index ) + num_iter            ) ;
                attr.setBinding( 0                                                    ) ; /// Problem? What is this?
                attr.setFormat ( formatFromAttributeType( sh.attributeType( index ) ) ) ;
                attr.setOffset ( offset                                               ) ;
                
                this->attr_desc.push_back( attr ) ;
                offset += byteSizeFromFormat( sh, index ) ;
              }
            }
          }
        }
      }

      bind.setBinding  ( 0                              ) ;
      bind.setInputRate( ::vk::VertexInputRate::eVertex ) ;
      bind.setStride   ( offset                         ) ;
      
      this->bind_desc.push_back( bind ) ;
    }

    void ShaderData::generateVulkanLayer()
    {
      ::vk::DescriptorSetLayoutCreateInfo                     info        ; ///< TODO
      std::map<std::string, ::vk::DescriptorSetLayoutBinding> binding_map ; ///< TODO
      std::vector<::vk::DescriptorSetLayoutBinding>           bindings    ; ///< TODO
      ::vk::DescriptorSetLayoutBinding                        binding     ; ///< TODO
      unsigned                                                iterator    ; ///< TODO

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
            auto flags = binding_map[ iter.uniformName( index ) ].stageFlags ;
            flags |= vulkanBitFromStage( iter.stage() ) ;
            binding_map[ iter.uniformName( index) ].setStageFlags( flags ) ;
          }
          else
          {
            binding.setBinding         ( iter.uniformBinding( index )                             ) ;
            binding.setDescriptorCount ( iter.uniformSize   ( index )                             ) ;          
            binding.setStageFlags      ( binding.stageFlags |= vulkanBitFromStage( iter.stage() ) ) ;
            binding.descriptorType << iter.uniformType( index ) ;

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
      
      info.setBindingCount( bindings.size() );
      info.setPBindings   ( bindings.data() );
      
      this->layout = this->gpu.createDescriptorSetLayout( info, nullptr) ;
    }

    void ShaderData::generateShaderModules()
    {
      ::vk::ShaderModuleCreateInfo info ;
      ::vk::ShaderModule           mod  ;

      this->modules.clear() ;

      for( auto shader = this->compiler.begin(); shader != this->compiler.end(); ++shader )
      {
        info.setCodeSize( shader.spirvSize() * sizeof( unsigned ) ) ;
        info.setPCode   ( shader.spirv()                          ) ;

        mod = this->gpu.createShaderModule( info, nullptr ) ;
        this->modules.insert( { shader.stage(), mod } ) ;
      }
    }
    
    void ShaderData::generatePipelineShaderInfo()
    {
      ::vk::PipelineShaderStageCreateInfo info ;
      unsigned iter ;
      
      iter = 0 ;
      this->stage_infos.resize( this->modules.size() ) ;
      for( auto it = this->compiler.begin(); it != this->compiler.end(); ++it )
      {
        info.setStage ( vulkanBitFromStage( it.stage() ) ) ;
        info.setModule( this->modules.at( it.stage()   ) ) ;
        info.setPName ( "main"                           ) ;
        
        this->stage_infos[ iter++ ] = info ;
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

    void Shader::load( unsigned gpu, const char* path )
    {
      const ::kgl::vk::render::Context context ;
      data().gpu = context.virtualDevice( gpu ) ;
      
      data().compiler.load( path ) ;
      
      data().begin = data().compiler.begin() ;
      data().end   = data().compiler.end()   ;

      data().generateVulkanLayer()        ; 
      data().generateShaderModules()      ;
      data().generateAttributeInfo()      ;
      data().generatePipelineShaderInfo() ;
    }

    unsigned Shader::numModules() const
    {
      return data().modules.size() ;
    }

    const ::vk::ShaderModule Shader::module( ::tools::shader::ShaderStage stage ) const
    {
      static ::vk::ShaderModule dummy ;
      auto iter = data().modules.find( stage ) ;
      
      return iter != data().modules.end() ? iter->second : dummy ;
    }

    const ::vk::DescriptorSetLayout Shader::layout() const
    {
      return data().layout ;
    }

    const ::vk::VertexInputAttributeDescription* Shader::vertexAttributes() const
    {
      return data().attr_desc.data() ;
    }

    const ::vk::VertexInputBindingDescription* Shader::vertexBindings() const
    {
      return data().bind_desc.data() ;
    }

    unsigned Shader::numVertexAttributes() const
    {
      return data().attr_desc.size() ;
    }

    unsigned Shader::numVertexBindings() const
    {
      return data().bind_desc.size() ;
    }

    const ::vk::PipelineShaderStageCreateInfo* Shader::infos() const
    {
      return data().stage_infos.data() ;
    }
    
    unsigned Shader::numShaders() const
    {
      return data().modules.size() ;
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