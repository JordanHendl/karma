#include "Graph.h"
#include "Module.h"
#include "Loader.h"
#include <log/Log.h>
#include <Bus.h>
#include <Signal.h>
#include <string>
#include <map>
#include <thread>
#include <iostream>

namespace kgl
{
  namespace vk
  { 
    struct NodeLoader
    {
      struct NodeInfo
      {
        typedef std::map<std::string, Module*> ModuleGraph ;
        std::string  name     ;
        std::string  type     ;
        std::string  pipeline ;
        unsigned     version  ;
        unsigned     id       ;
        ModuleGraph* graph    ;
        Loader*      loader   ;
        
        void setVersion( unsigned version )
        {
          this->version = version ;
        }

        void setType( const char* type )
        {
            this->type = type ;
        }
        
        void create()
        {
          using namespace karma::log ;

          if( this->graph->find( this->name ) == this->graph->end() )
          { 
            Log::output( " Initializing module. \n\n",

            "  - Name    : ", this->name.c_str()    , "\n",
            "  - Module  : ", this->type.c_str()    , "\n",
            "  - Version : ", this->version         , "\n",
            "  - Pipeline: ", this->pipeline.c_str(), "\n" ) ;

            auto descriptor = loader->descriptor( this->type.c_str() ) ;  
            auto module     = descriptor.create ( this->version      ) ;
            
            if( module )
            {
              module->setName    ( this->name.c_str()               ) ;
              module->setVersion ( this->version                    ) ;
              module->subscribe  ( this->pipeline.c_str(), this->id ) ;
              this->graph->insert( { this->name, module }           ) ;
            }
          }
        }
      };

      typedef std::map<std::string, Module*> ModuleGraph ;
      
      ModuleGraph*                    graph      ;
      Loader*                         loader     ;
      ::data::module::Bus             bus        ;
      std::map<std::string, NodeInfo> loaded_map ;
      std::string                     pipeline   ;
      
      NodeLoader()
      {
      }

      void setName( const char* name )
      {

        std::string dependency ;

        dependency = this->pipeline + name ;
        
        if( this->loaded_map.find( name ) == this->loaded_map.end() )
        {
          this->bus("").onCompletion( dependency.c_str(), &this->loaded_map[ name ], &NodeInfo::create ) ;
          
          this->bus( "Graphs::", this->pipeline.c_str(), "::Modules::", name, "::version" ).attach( &this->loaded_map[ name ], &NodeInfo::setVersion                         ) ;
          this->bus( "Graphs::", this->pipeline.c_str(), "::Modules::", name, "::type"    ).attach( &this->loaded_map[ name ], &NodeInfo::setType                            ) ;
          this->bus( "Graphs::", this->pipeline.c_str(), "::Modules::", name, "::type"    ).addDependancy( &this->loaded_map[ name ], &NodeInfo::setType, dependency.c_str() ) ;
          this->loaded_map[ name ].graph    = this->graph    ;
          this->loaded_map[ name ].loader   = this->loader   ;
          this->loaded_map[ name ].name     = name           ;
          this->loaded_map[ name ].pipeline = this->pipeline ;

        }
      }
    };

    struct GraphData
    {
      typedef std::map<std::string, Module*> ModuleGraph ;
        
      ::data::module::Bus bus         ;
      ModuleGraph         graph       ;
      Loader*             loader      ;
      NodeLoader          node_loader ;
      unsigned            bus_id      ;

      void stop  ( const char* name ) ;
      void remove( const char* name ) ;
      void setName( const char* name ) ;
    };

    void GraphData::setName( const char* name )
    {
      std::string dependency ;
       
      dependency = std::string( name ) + "::add" ;

      this->node_loader.pipeline = name ;

      this->bus( name, "::remove" ).attach( this, &GraphData::remove ) ;
      this->bus( name, "::stop"   ).attach( this, &GraphData::stop   ) ;
      
      this->bus( "Graphs::", name, "::Modules" ).attach( &node_loader, &NodeLoader::setName ) ;
      
      
      this->bus( name, "::stop" ).addDependancy( &this->node_loader, &NodeLoader::setName, dependency.c_str() ) ;
    }

    void GraphData::stop  ( const char* name )
    {
      auto iter = this->graph.find( name ) ;
      
      if( iter != this->graph.end() )
      {
        while( !iter->second->stop() ) {} ;
      }
    }

    void GraphData::remove( const char* name )
    {
      auto iter = this->graph.find( name ) ;

      if( iter != this->graph.end() )
      {
        this->stop( name ) ;
        this->graph.erase( iter ) ;
      }
    }

    Graph::Graph()
    {
      this->graph_data = new GraphData() ;
    }

    Graph::~Graph()
    { 
      delete this->graph_data ;
    }

     void Graph::initialize( Loader* loader )
    {
      data().loader = loader ; //todo remove this

      data().node_loader.loader   =  data().loader ;
      data().node_loader.graph    = &data().graph ;
      data().node_loader.bus.setChannel( data().bus_id ) ;
    }

    void Graph::add( const char* name, Module* module )
    {
      if( data().graph.find( name ) == data().graph.end() )
      {
        data().graph.insert( {name, module } ) ;
      }
    }
    
    void Graph::setName( const char* name )
    {
      data().setName( name ) ;
    }

    void Graph::subscribe( const char* name, unsigned id )
    {
      ::data::module::Bus bus ;
      
      bus.setChannel( id ) ;
      data().node_loader.graph = &data().graph ;
      
    }

    void Graph::kick()
    {
      for( auto module : data().graph )
      {
        module.second->initialize() ;
        std::thread( &Module::start, module.second ).detach() ;
      }
    }

    void Graph::stop()
    {
      for( auto module : data().graph )
      {
        while( !module.second->stop() ) {} ;
      }
    }

    void Graph::reset()
    {
      std::string type ;
      for( auto node : data().graph )
      {
        type = node.second->type() ;

        auto descriptor = data().loader->descriptor( type.c_str() ) ;
        descriptor.destroy( node.second ) ;
      }

      data().graph.clear() ;
    }

    GraphData& Graph::data()
    {
      return *this->graph_data ;
    }

    const GraphData& Graph::data() const
    {
      return *this->graph_data ;
    }
  }
}