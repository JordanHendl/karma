#include "AssetManager.h"
#include "../vk/context/Context.h"
#include "../vk/context/Image.h"
#include "../io/ImageLoader.h"
#include <log/Log.h>
#include <data/Bus.h>
#include <data/Signal.h>
#include <vulkan/vulkan.hpp>
#include <map>
#include <string>

namespace kgl
{
  namespace man
  {
    typedef std::map<std::string, ::kgl::vk::Image*> ImageMap ;
    typedef std::map<std::string, Atlas           *> AtlasMap ;
//    typedef std::map<std::string, ::kgl::vk::Model*> ModelMap ;

    struct AssetMaps
    {
      ImageMap images      ;
      AtlasMap spritesheet ;
//      ModelMap models ;
    };
    
    struct AtlasData
    {
      std::string name       ;
      ::kgl::vk::Image image ;
      unsigned sprite_width  ;
      unsigned sprite_height ;
      unsigned sprite_count  ;
    };

    static AssetMaps data ;
    
    Atlas::Atlas()
    {
      this->atlas_data = new AtlasData() ;
    }
    
    Atlas::Atlas( const Atlas& atlas )
    {
      this->atlas_data = new AtlasData() ;
      *this->atlas_data = *atlas.atlas_data ;
    }

    Atlas::~Atlas()
    {
      delete this->atlas_data ;
    }

    ::kgl::vk::Image& Atlas::image()
    {
      return data().image ;
    }

    unsigned Atlas::spriteWidth()
    {
      return data().sprite_width ;
    }

    unsigned Atlas::maxSprites()
    {
      return data().sprite_count ;
    }

    unsigned Atlas::spriteHeight()
    {
      return data().sprite_height ;
    }

    Atlas& Atlas::operator=( const Atlas& atlas )
    {
      *this->atlas_data = *atlas.atlas_data ;
      
      return *this ;
    }

    AtlasData& Atlas::data()
    {
      return *this->atlas_data ;
    }

    const AtlasData& Atlas::data() const
    {
      return *this->atlas_data ;
    }

    void AssetManager::addImage( const char* path, const char* name, unsigned gpu )
    {
      ::kgl::io::ImageLoader loader ;
      

      if( data.images.find( name ) == data.images.end() )
      {
        karma::log::Log::output( "Loading image ", name, " at path ", path, " on GPU ", gpu ) ;
        loader.load( path ) ;
        data.images.insert( { name,  new ::kgl::vk::Image() } ) ;

        data.images.at( name )->initialize( gpu, loader.width(), loader.height() ) ;
        data.images.at( name )->copy( loader.pixels(), loader.channels() ) ;
      }
    }

    void AssetManager::addAtlas( const char* path, const char* name, unsigned sprite_width, unsigned sprite_height, unsigned gpu )
    {
      ::kgl::io::ImageLoader loader ;

      if( data.spritesheet.find( name ) == data.spritesheet.end() )
      {
        karma::log::Log::output( "Loading spritesheet ", name, " at path ", path, " on GPU ", gpu ) ;
        loader.load( path ) ;
        data.spritesheet.insert( { name,  new Atlas() } ) ;

        data.spritesheet.at( name )->data().image.initialize( gpu, loader.width(), loader.height() ) ;
        data.spritesheet.at( name )->data().image.copy( loader.pixels(), loader.channels() ) ;
      }
    }

    const ::kgl::vk::Image& AssetManager::image( const char* name ) const
    {
      const static ::kgl::vk::Image dummy ;
      const auto iter = data.images.find( name ) ;
      
      if( iter != data.images.end() ) return *iter->second ;
      
      return dummy ;
    }

    void AssetManager::addModel( const char* path, const char* name )
    {
    
    }

    void AssetManager::addParticle( const char* image, const char* name )
    {
    
    }

    bool AssetManager::contains( const char* name ) const
    {
      return data.images.find( name ) != data.images.end() ;
    }

    void AssetManager::remove( const char* name )
    {
      auto iter =  data.images.find( name ) ;
      
      if( iter != data.images.end() ) data.images.erase( iter ) ;
    }

    void AssetManager::clear()
    {
      for( auto img : data.images )
      {
        delete img.second ;
      }
      
      data.images.clear() ;
    }
  }
}
