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
//    typedef std::map<std::string, ::kgl::vk::Model*> ModelMap ;

    struct AssetMaps
    {
      ImageMap images ;
//      ModelMap models ;
    };
    
    static AssetMaps data ;

    AssetManager::AssetManager()
    {
    
    }

    AssetManager::~AssetManager()
    {
    
    }

    void AssetManager::addImage( const char* path, const char* name, unsigned gpu )
    {
      ::kgl::io::ImageLoader loader ;
      

      if( data.images.find( name ) == data.images.end() )
      {
        karma::log::Log::output( "Loading file ", name, " at path ", path, " on GPU ", gpu ) ;
        loader.load( path ) ;
        data.images.insert( { name,  new ::kgl::vk::Image() } ) ;

        data.images.at( name )->initialize( gpu, loader.width(), loader.height() ) ;
        data.images.at( name )->copy( loader.pixels(), loader.channels() ) ;
      }
    }

    const ::kgl::vk::Image& AssetManager::image( const char* name )
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
