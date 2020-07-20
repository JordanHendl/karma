#ifndef CASPER_WINDOW_H
#define CASPER_WINDOW_H

#include "Surface.h"


namespace vk
{
  class SurfaceKHR    ;
  class RenderPass    ;
  class CommandBuffer ;
  class Semaphore     ;
}

namespace kgl
{
  namespace vk
  { 
    namespace render
    {
      class CommandBuffer ;
    }
    class Device          ;
    class SwapChain       ;
    class Synchronization ;

    class Window 
    {
      public:
        Window();
        Window( const Window& window ) ;
        ~Window();
        void operator=( const Window& window ) ;
        void initialize( const char* name, unsigned gpu, unsigned width, unsigned height ) ;
        const Device& device() const ;
        const SwapChain& chain() const ;
        void present( const Synchronization& sync ) ;
        void clear() ;
        unsigned currentSwap() const ;
        const ::vk::SurfaceKHR surface() const ;
        bool isInitialized() const ;
        void pollEvents() ;
      private:
        struct WindowData* win_data ;
        WindowData& data() ;
        const WindowData& data() const ;
    };
  }
}
#endif /* CASPER_WINDOW_H */

