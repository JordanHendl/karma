#ifndef CASPER_VULKAN_PIPELINE_H
#define CASPER_VULKAN_PIPELINE_H

typedef struct VkRenderPass_T *VkRenderPass ;

namespace casper
{
  namespace vulkan
  {
    class VertexType    ;
    class Device        ;
    class SwapChain     ;
    class CommandBuffer ;

    class Pipeline
    {
      public:
        Pipeline() ;
        ~Pipeline() ;

        void initialize( const char* path, SwapChain& chain, const Device& device, const VertexType& vertex ) ;
        const VkRenderPass& renderPass() const ;
        void bind( const CommandBuffer& buffer, unsigned buffer_id ) const ;

      private:
        void buildRasterizer() ;
        void buildMultisampler() ;
        void buildDepthStencilBuffers() ;
        void buildColorBlend() ;
        struct PipelineData* pipe_data ;
        PipelineData& data() ;
        const PipelineData& data() const ;
    };
  }
}

#endif