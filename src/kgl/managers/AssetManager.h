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
  namespace man
  {
    class AssetManager
    {
      public:
        AssetManager() ;
        ~AssetManager() ;
        void addImage( const char* path, const char* name ) ;
        const ::vk::Buffer imageVertices( const char * name ) const ;
        const ::vk::Buffer imageSampler( const char * name ) const ;
        const ::vk::Buffer modelIndices( const char* name ) const ;
        const ::vk::Buffer modelVertices( const char* name ) const ;
        void addStaticModel( const char* path, const char* name ) ;
        void addModel( const char* path, const char* name ) ;
        void addParticle( const char* image, const char* name ) ;
        bool contains( const char* name ) const ;
        void remove( const char* name ) ;
        void clear() ;
    };
  }
}

#endif /* ASSETMANAGER_H */

