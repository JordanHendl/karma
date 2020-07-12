#ifndef CASPER_VULKAN_SWAPCHAIN_H
#define CASPER_VULKAN_SWAPCHAIN_H

namespace vk
{
  class Framebuffer  ;
  class SwapchainKHR ;
  class Device       ;
  class SurfaceKHR   ;
  class RenderPass   ;
}

namespace kgl
{
  namespace vk
  {
    class Device  ;
    class Surface ;

    class SwapChain
    {
      public:
        SwapChain() ;
        SwapChain( const SwapChain& chain ) ;
        ~SwapChain() ;
        const ::vk::Framebuffer buffer( unsigned id ) const ;
        unsigned numBuffers() const ;
        void initialize( const Surface& surface, const Device& device ) ;
        const ::vk::SwapchainKHR chain() const ;
        void createFrameBuffers( const ::vk::RenderPass render_pass, const Device& device ) ;
        int format() const ;
        unsigned width() const ;
        unsigned height() const ;
        void reset() ;
      private:

        void findProperties( const Device& device, const Surface& ) ;
        void findImageViews( const Device& surface, const Surface& ) ;
        struct SwapChainData* chain_data ;
        SwapChainData& data() ;
        const SwapChainData& data() const ;
    };
  }
}
#endif