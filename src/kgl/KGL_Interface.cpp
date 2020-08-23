#include "KGL_Interface.h"
#include <log/Log.h>
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
#include <filesystem>
#include <stdlib.h>

struct KGL_InterfaceData
{
  ::data::module::Bus            bus                ; ///< The bus to use for data transfer.
  ::kgl::vk::render::Context     context            ; ///< The vulkan context for rendering.
  ::kgl::vk::Manager             mod_manager        ; ///< The Render Graph module manager.
  ::kgl::io::Input               input              ; ///< The object in charge of emitting event data.
  ::kgl::man::AssetManager       asset_manager      ; ///< The asset manager.
  ::kgl::man::CommandManager     cmd_manager        ; ///< The GPU command manager.
  ::karma::config::Configuration config             ; ///< The Configuration to initialize the graphics library with.
  ::kgl::database::Database      database           ; ///< The database that contains all asset metadata.
  ::kgl::Setup                   setup              ; ///< The object to handle setup of kgl.
  std::string                    window             ; ///< The name of the current active window.
  std::string                    module_path        ; ///< The path to the modules on the filesystem.
  std::string                    module_config_path ; ///< The path to the modules on the filesystem.
  std::string                    database_path      ; ///< The path to the modules on the filesystem.

  void fixString( std::string& str ) ;
  void addSheet( const char* img_name ) ;
  void addImage( const char* img_name ) ;
  void addModel( const char* model_name ) ;
  void setDatabasePath( const char* path ) ;
  void setModuleConfigPath( const char* path ) ;
  void setModulePath( const char* path ) ; 
  void setDebugOutput( const char* output ) ;
  void directoryCheck() ;
};


void KGL_InterfaceData::addSheet( const char* img_name )
{
  this->database.loadSheet( img_name ) ;
}

void KGL_InterfaceData::addImage( const char* img_name )
{
  this->database.loadImage( img_name ) ;
}

void KGL_InterfaceData::addModel( const char* model_name )
{
  
}

void KGL_InterfaceData::setModulePath( const char* path )
{
  this->module_path = path ;
}

void KGL_InterfaceData::setModuleConfigPath( const char* path )
{
  this->module_config_path = path ;
}

void KGL_InterfaceData::setDatabasePath( const char* path )
{
  this->database_path = path ;
}

void KGL_InterfaceData::setDebugOutput( const char* output )
{
  karma::log::Log::initialize( output ) ;
}

void KGL_InterfaceData::fixString( std::string& str )
{
  const char c = str.back() ;

  if( c == '\\' || c == '/' )
  {
    str.pop_back() ;
  }
}

void KGL_InterfaceData::directoryCheck()
{
  namespace fs = std::filesystem ;
  
  fs::path        path   ;
  fs::file_status status ;
  bool            valid  ;
  
  valid = true ;
  
  path = "data" ;
  if( !fs::exists( path ) ) valid = false ;
  
  path = "data/images" ;
  if( !fs::exists( path ) ) valid = false ;
  
  path = "uwu" ;
  if( !fs::exists( path ) ) valid = false ;
  
  path = "krender" ;
  if( !fs::exists( path ) ) valid = false ;
  
  if( !valid )
  {
    ::karma::log::Log::output( karma::log::Log::Level::Fatal, "Cannot find required directories. Please execute program in the correct directory." ) ;
    ::karma::log::Log::write() ;
    exit( EXIT_FAILURE ) ;
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
  karma::log::Log::write()      ;
//  data().mod_manager.shutdown() ;
}

void KGL_Interface::initialize()
{
  using namespace karma::log ;
  
  std::string path            ;
  std::string kgl_config_path ;
  
  
  path = "./" ;
  
  data().fixString( path ) ;
  data().directoryCheck() ;
  if( !::kgl::vk::isInitialized() )
  {
    ::kgl::vk::setBasePath( "./" ) ;
    ::kgl::vk::initialize() ;
    
    data().database.subscribe( "./", 0 ) ;
    data().setup   .subscribe( 0            ) ;
    
    data().bus( "required_images"       ).attach( this->kgl_data, &KGL_InterfaceData::addImage            ) ;
    data().bus( "required_models"       ).attach( this->kgl_data, &KGL_InterfaceData::addImage            ) ;
    data().bus( "required_spritesheets" ).attach( this->kgl_data, &KGL_InterfaceData::addSheet            ) ;
    data().bus( "database_path"         ).attach( this->kgl_data, &KGL_InterfaceData::setDatabasePath     ) ;
    data().bus( "modules_path"          ).attach( this->kgl_data, &KGL_InterfaceData::setModulePath       ) ;
    data().bus( "graph_config_path"     ).attach( this->kgl_data, &KGL_InterfaceData::setModuleConfigPath ) ;
    data().bus( "log_output_path"       ).attach( this->kgl_data, &KGL_InterfaceData::setDebugOutput      ) ;

    kgl_config_path = path + "/setup.json" ;
    
    
    data().config     .initialize( kgl_config_path.c_str(), 0 ) ;

    data().config     .initialize( data().database_path.c_str()  , 0                             ) ;
    data().setup      .initialize(                                                               ) ;
    data().mod_manager.initialize( data().module_path.c_str(), data().module_config_path.c_str() ) ;
    data().mod_manager.start() ;
    
    karma::log::Log::write() ;
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
