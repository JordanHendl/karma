#include "Graph.h"
#include "Module.h"
#include "Loader.h"
#include <Bus.h>
#include <Signal.h>
#include <string>
#include <map>
#include <thread>

namespace kgl
{
  namespace vk
  { 
    struct NodeLoader
    {
      ::data::module::Bus         bus        ;
      std::map<std::string, bool> loaded_map ;
      std::string                 pipeline   ;
      std::string                 type       ;
      std::string                 name       ;
      unsigned                    version    ;
      bool                        set_deps   ;
      
      NodeLoader()
      {
        this->set_deps = false ;
      }

      void setName( const char* name )
      {
        std::string dependency ;
        
        dependency = this->pipeline + "::add" ;
        
        this->name = name ;
        if( this->loaded_map.find( name ) == this->loaded_map.end() )
        {
          this->bus( "Graphs::", this->pipeline.c_str(), "::Modules::", name, "::version" ).attach( this, &NodeLoader::setVersion                            ) ;
          this->bus( "Graphs::", this->pipeline.c_str(), "::Modules::", name, "::type"    ).attach( this, &NodeLoader::setType                               ) ;
          this->bus( "Graphs::", this->pipeline.c_str(), "::Modules::", name, "::type"    ).addDependancy( this, &NodeLoader::setType   , dependency.c_str() ) ;
          
          this->loaded_map.insert( { name, true } ) ;
        }
      }

      void setVersion( unsigned version )
      {
        this->version = version ;
      }
      
      void setType( const char* type )
      {
        this->type = type ;
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
      void add() ;
      void setName( const char* name ) ;
    };

    void GraphData::setName( const char* name )
    {
      std::string dependency ;
       
      dependency = std::string( name ) + "::add" ;

      this->node_loader.pipeline = name ;
      this->node_loader.bus.setChannel( this->bus_id ) ;

      this->bus( name, "::remove" ).attach( this, &GraphData::remove ) ;
      this->bus( name, "::stop"   ).attach( this, &GraphData::stop   ) ;
      
      this->bus( "Graphs::", name, "::Modules" ).attach( &node_loader, &NodeLoader::setName ) ;
      
      this->bus("").onCompletion( dependency.c_str(), this, &GraphData::add ) ;
    }

    void GraphData::add()
    {
      if( this->graph.find( this->node_loader.name ) == this->graph.end() )
      {
        auto descriptor = loader->descriptor( this->node_loader.type.c_str() ) ;  
        auto module     = descriptor.create( this->node_loader.version )      ;
        
        if( module )
        {
          module->setName   ( this->node_loader.name.c_str()                   ) ;
          module->setVersion( this->node_loader.version                        ) ;
          module->subscribe ( this->node_loader.pipeline.c_str(), this->bus_id ) ;
          this->graph.insert( { this->node_loader.name, module }               ) ;
        }
        
        this->node_loader.version = 0 ;
      }
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
      
    }

    void Graph::kick()
    {
      for( auto module : data().graph )
      {
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