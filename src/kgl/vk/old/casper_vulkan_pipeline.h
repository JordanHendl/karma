#ifndef CASPER_VULKAN_PIPELINE_H
#define CASPER_VULKAN_PIPELINE_H

namespace casper
{
  namespace vulkan
  {
    class CommandBuffer ;
    class Window        ;
    class DescriptorSet ;
    
    class Pipeline
    {
      public:
        
        enum PipelineType
        {
          GRAPHICS,
          COMPUTE
        };

        Pipeline() ;
        ~Pipeline() ;

        void initialize( const char* path, const Window& window, PipelineType type = PipelineType::GRAPHICS ) ;
        void subscribe( const char* name, unsigned id ) ;
        void bind( const CommandBuffer& buffer ) const ;
        void bindCompute( const CommandBuffer& buffer ) const ;
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