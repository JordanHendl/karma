#ifndef KGL_IO_INPUT
#define KGL_IO_INPUT

namespace kgl
{
  namespace io
  {
    class Input
    {
      public:
        Input() ;
        ~Input() ;
        void subscribe( const char* window_name ) ;
    };
  }
}
#endif