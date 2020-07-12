#include "Setup.h"
#include <vk/context/Context.h>
#include <string>
#include <vector>
#include <Bus.h>
#include <Signal.h>
#include <map>

namespace kgl
{
  struct WindowSetup
  {
    std::string name        ;
    unsigned    width       ;
    unsigned    height      ;
    unsigned    gpu         ;
    unsigned    bus_channel ;
    
    WindowSetup() ;
    void subscribe() ;
    void setWidth( unsigned val ) ;
    void setHeight( unsigned val ) ;
    void setGPU( unsigned val ) ;
  };
  
  struct SetupData
  {
    typedef std::map<std::string, WindowSetup> WindowSets ;
    
    WindowSets  window_sets      ;
    std::string database_dir     ;
    std::string module_dir       ;
    std::string graph_config_dir ;
    unsigned    bus_channel      ;
    
    SetupData() ;
    void makeWindow( const char* name ) ;
    void makeWindowIndexed( unsigned id, const char* name ) ;
    void setModuleDir( const char* name ) ;
    void setGraphConfigDir( const char* path ) ;
    void setDatabase( const char* name ) ;
  };
  
  WindowSetup::WindowSetup()
  {
    
  }
  
  SetupData::SetupData()
  {
    
  }

  void WindowSetup::subscribe()
  {
    const auto base_path = ::kgl::vk::basePath() ;
    ::data::module::Bus bus ;
    
    bus.setChannel( this->bus_channel ) ;
    
    bus( "kgl_windows::", this->name.c_str(), "::width"  ).attach( this, &WindowSetup::setWidth  ) ;
    bus( "kgl_windows::", this->name.c_str(), "::height" ).attach( this, &WindowSetup::setHeight ) ;
    bus( "kgl_windows::", this->name.c_str(), "::gpu"    ).attach( this, &WindowSetup::setGPU    ) ;
  }

  void WindowSetup::setWidth( unsigned val )
  {
    this->width = val ;
  }

  void WindowSetup::setHeight( unsigned val )
  {
    this->height = val ;
  }

  void WindowSetup::setGPU( unsigned val )
  {
    this->gpu = val ;
  }  

  void SetupData::makeWindow( const char* name )
  {
    if( this->window_sets.find( name ) == this->window_sets.end() )
    {
      this->window_sets.insert({ std::string( name ), WindowSetup() } ) ;
      this->window_sets[ name ].bus_channel = this->bus_channel ;
      this->window_sets[ name ].name        = name              ;
      this->window_sets[ name ].subscribe() ;
    }
  }

  void SetupData::makeWindowIndexed( unsigned id, const char* name )
  {
    this->makeWindow( name ) ;
  }

  void SetupData::setDatabase( const char* name )
  {
    this->database_dir = name ;
  }

  void SetupData::setModuleDir( const char* name )
  {
    this->module_dir = name ;
  }

  void SetupData::setGraphConfigDir( const char* path )
  {
    this->graph_config_dir = path ;
  }

  Setup::Setup()
  {
    this->setup_data = new SetupData() ;
  }
  
  void Setup::initialize()
  {
    ::kgl::vk::render::Context context ;
    for( auto setup : data().window_sets )
    {
      auto set = setup.second ;
      context.addWindow( set.name.c_str(), set.gpu, set.width, set.height ) ;
    }
  }

  void Setup::subscribe( unsigned channel )
  {
    ::data::module::Bus bus ;
    
    bus.setChannel( channel ) ;
    data().bus_channel = channel ;

    bus( "kgl_windows"      ).attach( this->setup_data, &SetupData::makeWindow        ) ;
    bus( "kgl_windows"      ).attach( this->setup_data, &SetupData::makeWindowIndexed ) ;
    bus( "database_path"    ).attach( this->setup_data, &SetupData::setDatabase       ) ;
    bus( "graph_config_path").attach( this->setup_data, &SetupData::setGraphConfigDir ) ;
    bus( "module_path"      ).attach( this->setup_data, &SetupData::setModuleDir      ) ;
  }

  Setup::~Setup()
  {
    delete this->setup_data ;
  }
  
  SetupData& Setup::data()
  {
    return *this->setup_data ;
  }
  
  const SetupData& Setup::data() const
  {
    return *this->setup_data ;
  }
}
