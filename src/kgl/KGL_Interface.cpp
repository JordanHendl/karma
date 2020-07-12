#include "KGL_Interface.h"
#include "Setup.h"
#include "managers/AssetManager.h"
#include "managers/CommandManager.h"
#include "vk/context/Context.h"
#include "vk/node/Manager.h"
#include "Configuration.h"
#include <Parser.h>
#include <string>
#include <vector>

struct KGL_InterfaceData
{
  ::kgl::vk::render::Context     context       ; ///< The vulkan context for rendering.
  ::kgl::man::AssetManager       asset_manager ; ///< The asset manager.
  ::kgl::man::CommandManager     cmd_manager   ; ///< The GPU command manager.
  ::kgl::vk::Manager             mod_manager   ; ///< The Render Graph module manager.
  ::karma::config::Configuration config        ; ///< The Configuration to initialize the graphics library with.
  ::kgl::Setup                   setup         ;
};

struct ModelSubmitionData
{
  typedef std::vector<std::string> NameList ;
  struct vec3
  {
    float x, y, z ;
  };
  std::string model  ;
  std::string cmd    ;
  std::string anim   ;
  vec3        pos    ;
  vec3        rot    ;
  vec3        size   ;
  NameList    graphs ;
};

struct ImageSubmitionData
{
  
};

ModelSubmition::ModelSubmition()
{
  this->sub_data = new ModelSubmitionData() ;
}

ModelSubmition::~ModelSubmition()
{
  delete this->sub_data ;
}

void ModelSubmition::setModelName( const char* name )
{
  
}

void ModelSubmition::setSubmitionName( const char* name )
{
  
}

void ModelSubmition::setPosition( float x, float y, float z )
{
  
}

void ModelSubmition::setSize( float x, float y, float z )
{
  
}

void ModelSubmition::setRotationEuler( float x, float y, float z )
{
  
}

void ModelSubmition::setAnimation( const char* animation_name )
{
  
}

ModelSubmitionData& ModelSubmition::data()
{
  return *this->sub_data ;
}

const ModelSubmitionData& ModelSubmition::data() const
{
  return *this->sub_data ;
}

ImageSubmition::ImageSubmition()
{
  this->sub_data = new ImageSubmitionData() ;
}

ImageSubmition::~ImageSubmition()
{
  delete this->sub_data ;
}

void ImageSubmition::setImageName( const char* name )
{
  
}

void ImageSubmition::addGraph( const char* graph )
{
  
}

void ImageSubmition::setSubmitionName( const char* name )
{
  
}

void ImageSubmition::setPosition( float x, float y, float z )
{
  
}

void ImageSubmition::setSize( float x, float y, float z )
{
  
}

void ImageSubmition::setRotationEuler( float x, float y, float z )
{
  
}

ImageSubmitionData& ImageSubmition::data()
{
  return *this->sub_data ;
}

const ImageSubmitionData& ImageSubmition::data() const
{
  return *this->sub_data ;
}


KGL_Interface::KGL_Interface()
{
  this->kgl_data = new KGL_InterfaceData() ;
}

KGL_Interface::~KGL_Interface()
{
  delete this->kgl_data ;
}

void KGL_Interface::initialize( const char* base_path )
{
  std::string path ;
  
  path = base_path ;
  path += "/setup.json" ; 
  
  if( !::kgl::vk::isInitialized() )
  {
    ::kgl::vk::setBasePath( base_path ) ;
    ::kgl::vk::initialize() ;

    data().setup.subscribe( 0 ) ;
    
    data().config.initialize( path.c_str(), 0 ) ;
    data().setup.initialize() ;
  }
}

void KGL_Interface::setCurrentWindow( const char* name )
{
  
}

void KGL_Interface::loadPack( const char* path )
{
  data().config.initialize( path ) ;
}

void KGL_Interface::unlockPack()
{
  
}

void KGL_Interface::drawModel( const ModelSubmition& submit )
{
  
}

void KGL_Interface::drawImage( const ImageSubmition& submit )
{
  
}

void KGL_Interface::drawParticle( const char* name, float cx, float cy, unsigned amount )
{
  
}

KGL_InterfaceData& KGL_Interface::data()
{
  return *this->kgl_data ;
}

const KGL_InterfaceData& KGL_Interface::data() const
{
  return *this->kgl_data ;
}
