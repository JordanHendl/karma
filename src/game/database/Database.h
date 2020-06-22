#ifndef SAISEI_DATABASE_H
#define SAISEI_DATABASE_H

namespace database
{
  class Database
  {
    public:
      Database() ;
      ~Database() ;
      void subscribe( unsigned id ) ;
      void initialize() ;
    private:
      struct DatabaseData& db_data ;
      DatabaseData& data() ;
      const DatabaseData& data() const ;
  };
}
#endif