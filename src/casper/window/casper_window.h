#ifndef CASPER_WINDOW_H
#define CASPER_WINDOW_H

namespace casper
{
  class Window 
  {
    public:
      Window();
      ~Window();
      void initialize( const char* name, unsigned gpu, unsigned width, unsigned height ) ;
      bool isInitialized() const ;
      void pollEvents() ;
      void subscribe( unsigned id ) ;
      void setName( const char* name ) ;
      void setTitle( const char* title ) ;
      void setSize( unsigned width, unsigned height ) ;
      void setFullscreen( bool val ) ;
      void setBorderless( bool val ) ;
      void reset() ;
    private:
      struct WindowData* win_data ;
      WindowData& data() ;
      const WindowData& data() const ;
      friend class Vulkan ;
  };
}
#endif /* CASPER_WINDOW_H */

