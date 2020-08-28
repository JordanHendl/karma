#include "Database.h"
#include <log/Log.h>
#include <Parser.h>
#include <string>
#include <Bus.h>
#include <Signal.h>
#include <map>

namespace kal
{
  namespace database
  {
    struct SoundSetup
    {
      data::module::Bus        bus           ;
      std::string              name          ;
      std::string              path          ;

      SoundSetup() ;
      void initialize( const char* name ) ;
      void setName( const char* name ) ;
      void setPath( const char* path ) ;
    };

    struct MusicSetup
    {
      data::module::Bus        bus     ;
      std::string              name    ;
      std::string              path    ;

      MusicSetup() ;
      void initialize( const char* name ) ;
      void setName( const char* name ) ;
      void setPath( const char* path ) ;
     };

    struct DatabaseData
    {
      typedef std::map<std::string, SoundSetup > SoundSets ;
      typedef std::map<std::string, MusicSetup > MusicSets ;
    
      SoundSets                   sound_sets    ; 
      MusicSets                   music_sets    ; 
      std::string                 database_path ;
      data::module::Bus           bus           ;
      karma::config::json::Parser parser        ;
      
      void setDBPath( const char* path )
      {
        this->database_path = path ; 
      }
      
      void getValue( const char* key )
      {
        auto token = parser.find( key ) ;
        this->bus( key ).emit( token.string() ) ;
      }
      
      void musicName( const char* name ) ;
      void soundName( const char* name ) ;
    };
  

    MusicSetup::MusicSetup()
    {
      this->name          = "" ;
      this->path          = "" ;
    }

    void MusicSetup::initialize( const char* name )
    {
      this->setName( name ) ;
      karma::log::Log::output( "Adding Music ", this->name.c_str(), " to database." ) ;
      this->bus( "music::", name, "::path"          ).attach( this, &MusicSetup::setPath  ) ;
    }

    void MusicSetup::setName( const char* name )
    {
      this->name = name ;
    }

    void MusicSetup::setPath( const char* path )
    {
      this->path = path ;
    }
    
    SoundSetup::SoundSetup()
    {
      this->name = "" ;
      this->path = "" ;
    }

    void SoundSetup::initialize( const char* name )
    {
      this->setName( name ) ;

      karma::log::Log::output( "Adding sound effect ", this->name.c_str(), " to database." ) ;
      this->bus( "sound::", name, "::path" ).attach( this, &SoundSetup::setPath ) ;
    }

    void SoundSetup::setName( const char* name )
    {
      this->name = name ;
    }

    void SoundSetup::setPath( const char* path )
    {
      this->path = path ;
    }

    void DatabaseData::musicName( const char* name )
    {
      if( this->music_sets.find( name ) == this->music_sets.end() )
      {
        this->music_sets.insert( { std::string( name ), MusicSetup() } ) ;
        this->music_sets.at( name ).initialize( name ) ;
      } 
    }
    
    void DatabaseData::soundName( const char* name )
    {
      if( this->sound_sets.find( name ) == this->sound_sets.end() )
      {
        this->sound_sets.insert( { std::string( name ), SoundSetup() } ) ;
        this->sound_sets.at( name ).initialize( name ) ;
      } 
    }
   
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
    
    const char* Database::sound( const char* name )
    {
      auto iter = data().sound_sets.find( name ) ;
      
      if( iter != data().sound_sets.end() ) return iter->second.path.c_str() ;
      
      return "null" ;
    }

    const char* Database::music( const char* name )
    {
      auto iter = data().music_sets.find( name ) ;
      
      if( iter != data().music_sets.end() ) return iter->second.path.c_str() ;
      
      return "null" ;
    }

    void Database::initialize()
    {
      karma::log::Log::output( "Initializing KGL database using config: ", data().database_path.c_str() ) ;
      data().parser.initialize( data().database_path.c_str() ) ;
    }
  
    void Database::subscribe( unsigned id )
    {
      data().bus( "database_path" ).attach( this->db_data, &DatabaseData::setDBPath ) ;
      data().bus( "music"         ).attach( this->db_data, &DatabaseData::musicName ) ;
      data().bus( "sound"         ).attach( this->db_data, &DatabaseData::soundName ) ;
    }
  }
}