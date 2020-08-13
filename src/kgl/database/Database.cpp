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
    struct SheetSetup
    {
      ::kgl::man::AssetManager manager       ;
      data::module::Bus        bus           ;
      std::string              name          ;
      std::string              path          ;
      unsigned                 gpu           ;
      unsigned                 sprite_width  ;
      unsigned                 sprite_height ;
      SheetSetup() ;
      void add() ;
      void initialize( const char* name ) ;
      void setName( const char* name ) ;
      void setPath( const char* path ) ;
      void setSpriteWidth( unsigned width ) ;
      void setSpriteHeight( unsigned height ) ;
      void setGPU( unsigned gpu ) ;
    };

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
      typedef std::map<std::string, SheetSetup > SheetSets ;
    
      ImageSets                   image_sets    ; 
      SheetSets                   sheet_sets    ; 
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
      
      void sheetName( const char* name ) ;
      void imageName( const char* name ) ;
    };
  

    SheetSetup::SheetSetup()
    {
      this->name          = "" ;
      this->path          = "" ;
      this->gpu           = 0  ;
      this->sprite_width  = 0  ;
      this->sprite_height = 0  ;
    }

    void SheetSetup::add()
    {
      if( !this->manager.contains( this->name.c_str() ) )
      {
        this->manager.addAtlas( this->path.c_str(), this->name.c_str(), this->sprite_width, this->sprite_height, this->gpu ) ;
      }
    }

    void SheetSetup::initialize( const char* name )
    {
      this->setName( name ) ;
      karma::log::Log::output( "Adding spritesheet ", this->name.c_str(), " to database." ) ;
      this->bus( "spritesheets::", name, "::path"          ).attach( this, &SheetSetup::setPath         ) ;
      this->bus( "spritesheets::", name, "::gpu"           ).attach( this, &SheetSetup::setGPU          ) ;
      this->bus( "spritesheets::", name, "::sprite_width"  ).attach( this, &SheetSetup::setSpriteWidth  ) ;
      this->bus( "spritesheets::", name, "::sprite_height" ).attach( this, &SheetSetup::setSpriteHeight ) ;
    }

    void SheetSetup::setName( const char* name )
    {
      this->name = name ;
    }

    void SheetSetup::setPath( const char* path )
    {
      this->path = ::base_path + path ;
    }
    
    void SheetSetup::setSpriteWidth( unsigned width )
    {
      this->sprite_width = width ;
    }
    
    void SheetSetup::setSpriteHeight( unsigned height )
    {
      this->sprite_height = height ;
    }

    void SheetSetup::setGPU( unsigned gpu )
    {
      this->gpu = gpu ;
    }

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
        this->manager.addImage( this->path.c_str(), this->name.c_str(), this->gpu ) ;
      }
    }

    void ImageSetup::initialize( const char* name )
    {
      this->setName( name ) ;

      karma::log::Log::output( "Adding image ", this->name.c_str(), " to database." ) ;
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
    
    void DatabaseData::sheetName( const char* name )
    {
      if( this->sheet_sets.find( name ) == this->sheet_sets.end() )
      {
        this->sheet_sets.insert( { std::string( name ), SheetSetup() } ) ;
        this->sheet_sets.at( name ).initialize( name ) ;
      } 
    }
    
    void DatabaseData::imageName( const char* name )
    {
      if( this->image_sets.find( name ) == this->image_sets.end() )
      {
        this->image_sets.insert( { std::string( name ), ImageSetup() } ) ;
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

    void Database::loadSheet( const char* name )
    {
      auto iter = data().sheet_sets.find( name ) ;
      
      if( iter != data().sheet_sets.end() ) iter->second.add() ;
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
      data().bus( "spritesheets"  ).attach( this->db_data, &DatabaseData::sheetName ) ;
      data().bus( "textures"      ).attach( this->db_data, &DatabaseData::imageName ) ;
    }
  }
}