#ifndef SAISEI_IO_MANAGER_H
#define SAISEI_IO_MANAGER_H

namespace ss
{
  namespace io
  {
    class IOManager
    {
      public:
        IOManager() ;
        ~IOManager() ;
        void subscribe( unsigned id ) ;
        void initialize() ;
      private:
        struct IOIOManagerData& io_data ;
        IOIOManagerData& data() ;
        const IOIOManagerData& data() const ;
    };
  }
}

#endif