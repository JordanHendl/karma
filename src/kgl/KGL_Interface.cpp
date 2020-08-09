#include "KGL_Interface.h"
#include "Setup.h"
#include "io/Input.h"
#include "managers/AssetManager.h"
#include "database/Database.h"
#include "managers/CommandManager.h"
#include "vk/node/Manager.h"
#include "vk/context/Context.h"
#include "Configuration.h"
#include <Parser.h>
#include <Bus.h>
#include <Signal.h>
#include <string>
#include <vector>

struct KGL_InterfaceData
{
  ::data::module::Bus            bus           ; ///< The bus to use for data transfer.
  ::kgl::vk::render::Context     context       ; ///< The vulkan context for rendering.
  ::kgl::vk::Manager             mod_manager   ; ///< The Render Graph module manager.
  ::kgl::io::Input               input         ; ///< The object in charge of emitting event data.
  ::kgl::man::AssetManager       asset_manager ; ///< The asset manager.
  ::kgl::man::CommandManager     cmd_manager   ; ///< The GPU command manager.
  ::karma::config::Configuration config        ; ///< The Configuration to initialize the graphics library with.
  ::kgl::database::Database      database      ; ///< The database that contains all asset metadata.
  ::kgl::Setup                   setup         ; ///< The object to handle setup of kgl.
  std::string                    window        ; ///< The name of the current active window.
  std::string                    module_path   ; ///< The path to the modules on the filesystem.

  void fixString( std::string& str ) ;
  void addImage( const char* img_name ) ;
  void addModel( const char* model_name ) ;
  void setModulePath( const char* path ) ;
};


void KGL_InterfaceData::addImage( const char* img_name )
{
  this->database.loadImage( img_name ) ;
}

void KGL_InterfaceData::setModulePath( const char* path )
{
  this->module_path = path ;
}

void KGL_InterfaceData::addModel( const char* model_name )
{
  
}

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
  std::string database_path   ;
  
  path = base_path ;
  
  data().fixString( path ) ;
  
  
  if( !::kgl::vk::isInitialized() )
  {
    ::kgl::vk::setBasePath( base_path ) ;
    ::kgl::vk::initialize() ;
    
    data().database.subscribe( base_path, 0 ) ;
    data().setup   .subscribe( 0            ) ;
    
    data().bus( "required_images"   ).attach( this->kgl_data, &KGL_InterfaceData::addImage      ) ;
    data().bus( "required_models"   ).attach( this->kgl_data, &KGL_InterfaceData::addImage      ) ;
    data().bus( "modules_directory" ).attach( this->kgl_data, &KGL_InterfaceData::setModulePath ) ;

    kgl_config_path = path + "/setup.json" ;

    data().config     .initialize( kgl_config_path.c_str(), 0 ) ;
    database_path   = path + "/database.json"                   ;
    mod_path        = path + data().module_path                 ;
    mod_config_path = path + "/kgl_configuration.json"          ;

    data().config     .initialize( database_path.c_str()  , 0                ) ;

    data().setup      .initialize(                                           ) ;
    data().mod_manager.initialize( mod_path.c_str(), mod_config_path.c_str() ) ;
    data().mod_manager.start() ;
  }
}

void KGL_Interface::setCurrentWindow( const char* name )
{
  this->data().window = name ; 
}

void KGL_Interface::pollEvents()
{
  data().input.poll() ;
}

void KGL_Interface::present()
{
  
}

void KGL_Interface::start()
{
  data().context.start( data().window.c_str() ) ;
}

void KGL_Interface::loadPack( const char* path )
{
  this->unloadPack() ;
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
