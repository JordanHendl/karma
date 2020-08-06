#ifndef CASPER_VULKAN_SURFACE_H
#define CASPER_VULKAN_SURFACE_H

typedef struct SDL_Window SDL_Window ;

namespace vk
{
  class SurfaceKHR ;
}

namespace kgl
{
  namespace vk
  {
    class Surface
    {
      public:
        Surface() ;
        Surface( const Surface& surface ) ;
        ~Surface() ;
        void operator=( const Surface& surface ) ;
        void initialize( const SDL_Window& window ) ;
        const ::vk::SurfaceKHR surface() const ;
        const SDL_Window* window() const ;
        const char* name() const ;
        unsigned width() const ;
        unsigned height() const ;
      private:
        struct SurfaceData* surf_data ;
        SurfaceData& data() ;
        const SurfaceData& data() const ;
    };
  }
}
#endif