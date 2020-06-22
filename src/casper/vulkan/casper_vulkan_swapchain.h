#ifndef CASPER_VULKAN_SWAPCHAIN_H
#define CASPER_VULKAN_SWAPCHAIN_H

typedef struct VkSwapchainKHR_T   *VkSwapchainKHR   ;
typedef struct VkFramebuffer_T *VkFramebuffer ;
typedef struct VkRenderPass_T  *VkRenderPass  ;

namespace casper
{
  namespace vulkan
  {
    struct Device ;
    struct Surface ;

    class SwapChain
    {
      public:
        SwapChain() ;
        const VkFramebuffer* buffers() const ;
        unsigned numBuffers() const ;
        void initialize( const Surface& surface, const Device& device ) ;
        const VkSwapchainKHR* chain() const ;
        void createFrameBuffers( const VkRenderPass& render_pass, const Device& device ) ;
        int format() const ;
        unsigned width() const ;
        unsigned height() const ;
        ~SwapChain() ;
        void reset() ;
      private:

        unsigned findPresent( const Surface& surface, const Device& device ) ;
        void findProperties( const casper::vulkan::Device& device, const Surface& surface ) ;
        void findImageViews( const Surface& surface, const Device& device ) ;
        struct SwapChainData* chain_data ;
        SwapChainData& data() ;
        const SwapChainData& data() const ;
    };
  }
}
#endif