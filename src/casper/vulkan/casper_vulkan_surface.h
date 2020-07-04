#ifndef CASPER_VULKAN_SURFACE_H
#define CASPER_VULKAN_SURFACE_H

typedef struct VkPresentInfoKHR  VkPresentInfoKHR ;
typedef struct VkSurfaceKHR_T*   VkSurfaceKHR     ;
typedef struct SDL_Window        SDL_Window       ;

namespace casper
{
  namespace vulkan
  {
    class Device ;
    class Surface
    {
      public:
        Surface() ;
        ~Surface() ;
        void initialize( const SDL_Window& window ) ;
        void present( const VkPresentInfoKHR& present_info ) ;
        VkSurfaceKHR surface() const ;
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