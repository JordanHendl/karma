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
        void initialize( const char* name, const char* display_name, unsigned gpu, unsigned width, unsigned height, bool resizable, bool borderless, unsigned num_sems ) ;
        const Device& device() const ;
        SwapChain& chain() ;
        void present( const Synchronization& sync ) ;
        void setSize( unsigned width, unsigned height ) ;
        void setTitle( const char* title ) ;
        void setFullscreen( bool val ) ;
        void setBorderless( bool val ) ;
        void reset() ;
        void start() ;
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

