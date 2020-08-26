#ifndef KGL_VK_PIPELINE_H
#define KGL_VK_PIPELINE_H

namespace vk
{
  class Buffer         ;
  class RenderPass     ;
  class PipelineLayout ;
}

namespace kgl
{
  namespace vk
  {
    class Uniform       ;
    class DescriptorSet ;
    class Shader        ;
    namespace render
    {
      class CommandBuffer ;

      class Pipeline
      {
        public:
          Pipeline() ;
          ~Pipeline() ;
          void initialize( const char* uwu_path, unsigned gpu, unsigned width, unsigned height, const ::vk::RenderPass pass ) ;
          Shader& shader() ;
          const ::vk::PipelineLayout layout() const ;
          void subscribe( const char* name, unsigned channel ) ;
          void bind( const ::kgl::vk::render::CommandBuffer& buffer, const ::kgl::vk::DescriptorSet& set ) const ;
          void setPushConstantByteSize( unsigned size ) ;
          void setPushConstantStageFlag( unsigned stage ) ;
          void reset() ;
          const char* name() const ;
        private:
          struct PipelineData *pipe_data ;
          PipelineData& data() ;
          const PipelineData& data() const ;
      };
    }

    namespace compute
    {
      class CommandBuffer ;

      class Pipeline
      {
        public:
          Pipeline() ;
          ~Pipeline() ;
          void initialize( const char* uwu_path, unsigned gpu ) ;
          void bind( const ::kgl::vk::compute::CommandBuffer& buffer, const ::kgl::vk::DescriptorSet& set ) const ;
          void subscribe( unsigned channel ) ;
          const char* name() const ;
        private:
          struct PipelineData *pipe_data ;
          PipelineData& data() ;
          const PipelineData& data() const ;
      };
    }    
  }
}

#endif /* PIPELINE_H */

