/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   AssetManager.h
 * Author: jhendl
 *
 * Created on July 10, 2020, 3:32 PM
 */

#ifndef KGL_ASSET_MANAGER_H
#define KGL_ASSET_MANAGER_H


namespace vk
{
  class Buffer ;
}

namespace kgl
{
  
  namespace vk
  {
    class Image ;
    class Model ;
  }

  namespace man
  {
    class AssetManager ;
    class Atlas
    {
      public:
        Atlas() ;
        Atlas( const Atlas& atlas ) ;
        ~Atlas() ;
        const ::kgl::vk::Image& image() const ;
        unsigned spriteWidth() const ;
        unsigned maxSprites() const ;
        unsigned spriteHeight() const ;
        Atlas& operator=( const Atlas& atlas ) ;
        
      private:
        friend class AssetManager ;
        struct AtlasData* atlas_data ;
        AtlasData& data() ;
        const AtlasData& data() const ;
    };

    class AssetManager
    {
      public:
        AssetManager() = default ;
        ~AssetManager() = default ;
        void addImage( const char* path, const char* name, unsigned gpu ) ;
        void addAtlas( const char* path, const char* name, unsigned sprite_width, unsigned sprite_height, unsigned gpu ) ;
        void addParticle( const char* image, const char* name ) ;
        void addModel( const char* path, const char* name ) ;
        const ::kgl::vk::Image& image( const char* name ) const ;
        const Atlas& atlas( const char* name ) const ;
        bool contains( const char* name ) const ;
        void remove( const char* name ) ;
        void clear() ;
    };
  }
}

#endif /* ASSETMANAGER_H */

