#ifndef CASPER_WINDOW_H
#define CASPER_WINDOW_H

typedef struct VkRenderPass_T*   VkRenderPass ;
namespace casper
{
  namespace vulkan
  { 
    class Device        ;
    class SwapChain     ;
    class CommandBuffer ;
    class Window 
    {
      public:
        Window();
        ~Window();
        void initialize( const char* name, unsigned gpu, unsigned width, unsigned height ) ;
        const Device& device() const ;
        const SwapChain& chain() const ;
        const VkRenderPass& pass() const ;
        bool isInitialized() const ;
        void submit( const CommandBuffer& buffer ) ;
        void present() ;
        void pollEvents() ;
      private:
        struct WindowData* win_data ;
        WindowData& data() ;
        const WindowData& data() const ;
    };
  }
}
#endif /* CASPER_WINDOW_H */

