#ifndef KGL_VK_PIPELINE_H
#define KGL_VK_PIPELINE_H

namespace vk
{
  class Buffer     ;
  class RenderPass ;
}

namespace kgl
{
  namespace vk
  {
    class Uniform ;
    namespace render
    {
      class CommandBuffer ;

      class Pipeline
      {
        public:
          Pipeline() ;
          ~Pipeline() ;
          void initialize( const char* uwu_path, unsigned gpu, unsigned num_descriptors, const ::vk::RenderPass pass ) ;
          void subscribe( const char* name, unsigned channel ) ;
          void bind( const ::kgl::vk::render::CommandBuffer& buffer ) const ;
          void set( const Uniform& uniform ) ;
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
          void initialize( const char* uwu_path, unsigned gpu, unsigned num_descriptors ) ;
          void bind( const ::kgl::vk::compute::CommandBuffer& buffer ) const ;
          void subscribe( unsigned channel ) ;
          void set( const Uniform& uniform ) ;
          void set( const char* key, ::vk::Buffer ssbo ) ;
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

