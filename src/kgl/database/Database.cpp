#include "Database.h"
#include "../managers/AssetManager.h"
#include <log/Log.h>
#include <Parser.h>
#include <string>
#include <Bus.h>
#include <Signal.h>
#include <map>

static std::string base_path ; 

namespace kgl
{
  namespace database
  {
    struct ImageSetup
    {
      ::kgl::man::AssetManager manager ;
      data::module::Bus        bus     ;
      std::string              name    ;
      std::string              path    ;
      unsigned                 gpu     ;

      ImageSetup() ;
      void add() ;
      void initialize( const char* name ) ;
      void setName( const char* name ) ;
      void setPath( const char* path ) ;
      void setGPU( unsigned gpu ) ;
    };

    struct DatabaseData
    {
      typedef std::map<std::string, ImageSetup > ImageSets ;
    
      ImageSets                   image_sets    ; 
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
      
      void imageName( const char* name ) ;
    };
  

    ImageSetup::ImageSetup()
    {
      this->name = "" ;
      this->path = "" ;
      this->gpu  = 0  ;
    }

    void ImageSetup::add()
    {
      if( !this->manager.contains( this->name.c_str() ) )
      {
        karma::log::Log::output( "Adding image to database: ", this->name.c_str(), " using path ", this->path.c_str() ) ;
        this->manager.addImage( this->path.c_str(), this->name.c_str(), this->gpu ) ;
      }
    }

    void ImageSetup::initialize( const char* name )
    {
      this->setName( name ) ;

      this->bus( "textures::", name, "::path" ).attach( this, &ImageSetup::setPath ) ;
      this->bus( "textures::", name, "::gpu"  ).attach( this, &ImageSetup::setGPU  ) ;
    }

    void ImageSetup::setName( const char* name )
    {
      this->name = name ;
    }

    void ImageSetup::setPath( const char* path )
    {
      this->path = ::base_path + path ;
    }

    void ImageSetup::setGPU( unsigned gpu )
    {
      this->gpu = gpu ;
    }
    
    void DatabaseData::imageName( const char* name )
    {
      if( this->image_sets.find( name ) == this->image_sets.end() )
      {
        this->image_sets.insert( { std::string( name ), ImageSetup()  } ) ;
        this->image_sets.at( name ).initialize( name ) ;
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
    
    void Database::loadImage( const char* name )
    {
      auto iter = data().image_sets.find( name ) ;
      
      if( iter != data().image_sets.end() ) iter->second.add() ;
    }

    void Database::initialize()
    {
      karma::log::Log::output( "Initializing KGL database using config: ", data().database_path.c_str() ) ;
      data().parser.initialize( data().database_path.c_str() ) ;
    }
  
    void Database::subscribe( const char* base_path, unsigned id )
    {
      ::base_path = base_path ;
      data().bus( "database_path" ).attach( this->db_data, &DatabaseData::setDBPath ) ;
      data().bus( "textures"      ).attach( this->db_data, &DatabaseData::imageName ) ;
    }
  }
}