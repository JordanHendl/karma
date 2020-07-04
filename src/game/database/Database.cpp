#include "Database.h"
#include <Parser.h>
#include <string>
#include <Bus.h>
#include <Signal.h>

namespace karma
{
  namespace database
  {
    struct DatabaseData
    {
      std::string                 database_path ;
      data::module::Bus         bus           ;
      karma::config::json::Parser parser        ;
      
      void setDBPath( const char* path )
      {
        this->database_path = path ; 
      }

      void getValue( const char* key )
      {
        auto token = parser.find( key ) ;
        this->bus[ key ].emit( token.string() ) ;
      }
    };
  
    Database::Database()
    {
      this->db_data = new DatabaseData() ;
    }
  
    Database::~Database()
    {
      delete this->db_data ;
    }
  
    DatabaseData& Database::data()
    {
      return *this->db_data ;
    }
  
    const DatabaseData& Database::data() const
    {
      return *this->db_data ;
    }
  
    void Database::initialize()
    {
      data().parser.initialize( data().database_path.c_str() ) ;
    }
  
    void Database::subscribe( unsigned id )
    {
      data().bus[ "database_path" ].attach( this->db_data, &DatabaseData::setDBPath ) ;
    }
  }
}