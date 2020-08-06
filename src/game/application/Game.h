#ifndef KARMA_GAME_H
#define KARMA_GAME_H

namespace karma
{
  class Game
  {
    public:
      Game() ;
      ~Game() ;
      void initialize( const char* setup_path ) ;
      bool run() ;
    private:
      struct GameData* game_data ;
      GameData& data() ;
      const GameData& data() const ;
  };
}
#endif /* GAME_H */

