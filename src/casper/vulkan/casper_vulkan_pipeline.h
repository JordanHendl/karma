#ifndef CASPER_VULKAN_PIPELINE_H
#define CASPER_VULKAN_PIPELINE_H

namespace casper
{
  namespace vulkan
  {
    class VertexType    ;
    class CommandBuffer ;
    class Window        ;
    class DescriptorSet ;
    class Pipeline
    {
      public:
        Pipeline() ;
        ~Pipeline() ;

        void initialize( const char* path, const Window& window ) ;
        void subscribe( const char* name, unsigned id ) ;
        void bind( const CommandBuffer& buffer ) const ;
        const char* name() const ;
        const DescriptorSet& set() ;
      private:
        struct PipelineData* pipe_data ;
        PipelineData& data() ;
        const PipelineData& data() const ;
    };
  }
}

#endif