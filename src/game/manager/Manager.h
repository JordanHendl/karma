#ifndef SAISEI_GAME_MANAGER_H
#define SAISEI_GAME_MANAGER_H

namespace karma
{
  namespace man
  {
    /**
     */ 
    class GameManager
    {
      public:
        GameManager() ;
        ~GameManager() ;
        void initialize( int num_arg, const char** str_buff ) ;
        int start() ;
      private:
        struct ManagerData *man_data ;
        ManagerData& data() ;
        const ManagerData& data() const ;
    };
  }
}

#endif