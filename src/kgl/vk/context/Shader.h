
#ifndef KGL_VK_SHADER_H
#define KGL_VK_SHADER_H

namespace tools
{
  namespace shader
  {
    class ShaderIterator        ;
    enum ShaderStage : unsigned ;
  }
}

namespace vk
{
  class VertexInputAttributeDescription ;
  class VertexInputBindingDescription   ;
  class ShaderModule                    ;
  class DescriptorSetLayout             ;
  class PipelineShaderStageCreateInfo   ;
  enum class VertexInputRate ;
}

namespace kgl
{
  namespace vk
  {
    class Shader
    {
      public:
        Shader() ;
        ~Shader() ;
        void load( unsigned gpu, const char* path ) ;
        unsigned numModules() const ;
        const ::vk::ShaderModule module( ::tools::shader::ShaderStage stage ) const ;
        const ::vk::DescriptorSetLayout layout() const ;
        const ::vk::VertexInputAttributeDescription* vertexAttributes() const ;
        const ::vk::VertexInputBindingDescription* vertexBindings() const ;
        unsigned numVertexAttributes() const ;
        unsigned numVertexBindings() const ;
        const ::vk::PipelineShaderStageCreateInfo* infos() const ;
        void setVertexInputType( const ::vk::VertexInputRate& rate ) ;
        unsigned numShaders() const ;
        const ::tools::shader::ShaderIterator& begin() const ;
        const ::tools::shader::ShaderIterator& end() const ;
      private:
        struct ShaderData* shader_data ;
        ShaderData& data() ;
        const ShaderData& data() const ;
    };
  }
}

#endif /* SHADER_H */

