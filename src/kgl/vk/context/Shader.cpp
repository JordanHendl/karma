#include "Shader.h"
#include <shader/UWUShader.h>
#include <vulkan/vulkan.hpp>

namespace kgl
{
  namespace vk
  {
    struct ShaderData
    {
      
    };

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
    
    }

    unsigned Shader::numModules() const
    {
    
    }

    const ::vk::ShaderModule Shader::module( ::tools::shader::ShaderStage stage ) const
    {
    
    }

    const ::vk::DescriptorSetLayout Shader::layout() const
    {
    
    }

    const ::vk::VertexInputAttributeDescription Shader::vertexAttributes() const
    {
    
    }

    const ::vk::VertexInputBindingDescription Shader::vertexBindings() const
    {
    
    }

    unsigned Shader::numVertexAttributes() const
    {
    
    }

    unsigned Shader::numVertexBindings() const
    {
    
    }

    const ::tools::shader::ShaderIterator& Shader::begin() const
    {
    
    }

    const ::tools::shader::ShaderIterator& Shader::end() const
    {
    
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