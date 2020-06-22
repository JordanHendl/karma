#ifndef CASPER_LIBRARY_H
#define CASPER_LIBRARY_H

namespace casper
{
  namespace library
  {
    enum RENDERING_API
    {
      CSP_API_NONE   = 0,
      CSP_API_VULKAN = 1,
      CSP_API_OPENGL = 2
    } ;
    
    
    bool renderingInitialized() ;
    
    void setRenderingAPI( casper::library::RENDERING_API api ) ;
    
    RENDERING_API renderingApi() ;
    
    void initializeAPI() ;
  }
}

#endif
