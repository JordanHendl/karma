#ifndef CASPER_VK_SURFACE_MANAGER_H
#define CASPER_VK_SURFACE_MANAGER_H

namespace casper
{
  namespace vulkan
  {
    class Surface ;
    
    /** An object for managing all vulkan surfaces. Whenever a window is created, if using the vulkan API,
     *  a surface is automatically created. Whenever a window is destroyed, it is also destroyed.
     */ 
    class SurfaceManager
    {
      public:
        SurfaceManager() ;
        ~SurfaceManager() ;
        Surface& surface( const char* name ) ;
    };
  }
}

#endif