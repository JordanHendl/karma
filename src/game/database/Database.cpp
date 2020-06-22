#include "Database.h"
#include <config/Parser.h>
#include <string>
#include <casper_bus.h>
#include <casper_signal.h>
namespace database
{
  struct DatabaseData
  {
    std::string         database_path ;
    casper::module::Bus bus           ;
    void setDBPath( const char* path )
    {
      this->database_path = path ; 
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

  void Database::initialize()
  {

  }

  void Database::subscribe( unsigned id )
  {

  }
}