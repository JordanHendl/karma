#ifndef SAISEI_INPUT_MANAGER_H
#define SAISEI_INPUT_MANAGER_H

namespace karma
{
  namespace io
  {
    class InputManager
    {
      public:
        InputManager() ;
        ~InputManager() ;
        void kick() ;
        void subscribe( unsigned id ) ;
        void initialize() ;
      private:
        struct InputManagerData& input_data ;
        InputManagerData& data() ;
        const InputManagerData& data() const ;
    };
  }
}
#endif