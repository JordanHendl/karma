#include "casper_object_loader.h"
#include <map>
#include <stdio.h>

#ifdef __WIN32__
  #include <windows.h>
  #define dlopen LoadLibrary 
  #define LibHandle HINSTANCE 
  #define dlsym GetProcAddress
#elif(UNIX)
  #include <dlfcn.h>
  #define LibHandle void* 
#endif

namespace casper
{
  namespace io
  {

    struct ObjectLoaderData
    {
      typedef std::map<std::string, ObjectLoader::DL_FUNC> FunctionMap ;

      LibHandle   handle ;
      FunctionMap map    ;

      ObjectLoaderData()
      {
        this->handle = NULL ;
      }
    };

    ObjectLoader::ObjectLoader()
    {
      this->loader_data = new ObjectLoaderData() ;
    }

    ObjectLoader::~ObjectLoader()
    {
      delete this->loader_data ;
    }

    ObjectLoader::DL_FUNC ObjectLoader::symbol( const char* symbol_name )
    {
      ObjectLoader::DL_FUNC func ;

      if( data().map.find( symbol_name ) == data().map.end() )
      {
        func = reinterpret_cast<ObjectLoader::DL_FUNC>( dlsym( data().handle, symbol_name ) ) ;
      }

      return data().map.at( symbol_name ) ;
    }

    void ObjectLoader::load( const char* lib_path )
    {
      data().handle = dlopen( lib_path ) ;

      if( data().handle == NULL )
      {
        printf(" Error loading shared object %s.", lib_path ) ;
      }
    }

    ObjectLoaderData& ObjectLoader::data()
    {
      return *this->loader_data ;
    }

    const ObjectLoaderData& ObjectLoader::data() const
    {
      return *this->loader_data ;
    }
  }
}