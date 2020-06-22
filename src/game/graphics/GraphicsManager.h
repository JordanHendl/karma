#ifndef SAISEI_GRAPHICS_MANAGER_H
#define SAISEI_GRAPHICS_MANAGER_H

namespace karma
{
  namespace graphics
  {
    class Manager
    {
      public:
        Manager() ;
        ~Manager() ;
        void kick() ;
        void subscribe( unsigned id ) ;
        void initialize() ;
      private:
        struct ManagerData& input_data ;
        ManagerData& data() ;
        const ManagerData& data() const ;
    };
  }
}
#endif