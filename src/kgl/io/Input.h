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
        void initialize() ;
        void poll() ; 
      private:
        struct InputData *input_data ;
        InputData& data() ;
        const InputData& data() const ;
    };
  }
}
#endif