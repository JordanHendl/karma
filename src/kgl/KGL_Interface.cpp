#include "KGL_Interface.h"
#include "Setup.h"
#include "managers/AssetManager.h"
#include "managers/CommandManager.h"
#include "vk/node/Manager.h"
#include "vk/context/Context.h"
#include "Configuration.h"
#include <Parser.h>
#include <string>
#include <vector>

struct KGL_InterfaceData
{
  ::kgl::vk::render::Context     context       ; ///< The vulkan context for rendering.
  ::kgl::vk::Manager             mod_manager   ; ///< The Render Graph module manager.
  ::kgl::man::AssetManager       asset_manager ; ///< The asset manager.
  ::kgl::man::CommandManager     cmd_manager   ; ///< The GPU command manager.
  ::karma::config::Configuration config        ; ///< The Configuration to initialize the graphics library with.
  ::kgl::Setup                   setup         ;
  
  void fixString( std::string& str ) ;
};

void KGL_InterfaceData::fixString( std::string& str )
{
  const char c = str.back() ;

  if( c == '\\' || c == '/' )
  {
    str.pop_back() ;
  }
}

KGL_Interface::KGL_Interface()
{
  this->kgl_data = new KGL_InterfaceData() ;
}

KGL_Interface::~KGL_Interface()
{
  delete this->kgl_data ;
}

void KGL_Interface::shutdown()
{
  data().mod_manager.shutdown() ;
}

void KGL_Interface::initialize( const char* base_path )
{
  std::string path            ;
  std::string kgl_config_path ;
  std::string mod_path        ;
  std::string mod_config_path ;
  
  path = base_path ;
  
  data().fixString( path ) ;
  
  kgl_config_path = path + "/setup.json"             ;
  mod_path        = path + "/build/modules/"         ;
  mod_config_path = path + "/kgl_configuration.json" ;
  
  if( !::kgl::vk::isInitialized() )
  {
    ::kgl::vk::setBasePath( base_path ) ;
    ::kgl::vk::initialize() ;

    data().setup.subscribe( 0 ) ;
    
    data().config     .initialize( kgl_config_path.c_str(), 0                ) ;
    data().setup      .initialize(                                           ) ;
    data().mod_manager.initialize( mod_path.c_str(), mod_config_path.c_str() ) ;
    data().mod_manager.start() ;
  }
}

void KGL_Interface::setCurrentWindow( const char* name )
{
  
}

void KGL_Interface::loadPack( const char* path )
{
  data().config.initialize( path ) ;
}

void KGL_Interface::unloadPack()
{
  data().asset_manager.clear() ;
}

KGL_InterfaceData& KGL_Interface::data()
{
  return *this->kgl_data ;
}

const KGL_InterfaceData& KGL_Interface::data() const
{
  return *this->kgl_data ;
}
