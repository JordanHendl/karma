#ifndef SAISEI_STATE_MANAGER_H
#define SAISEI_STATE_MANAGER_H

namespace ss
{
  namespace man
  {
    /**
     */ 
    class StateManager
    {
      public:
        StateManager() ;
        ~StateManager() ;
        void initialize() ;
        void kick() ;
        void subscribe( unsigned id ) ;
        int start() ;
      private:
        struct StateManagerData *man_data ;
        StateManagerData& data() ;
        const StateManagerData& data() const ;
    };
  }
}

#endif