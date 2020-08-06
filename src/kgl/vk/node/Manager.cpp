#include "Manager.h"
#include "Loader.h"
#include "Graph.h"
#include <Bus.h>
#include <Signal.h>
#include <Configuration.h>
#include <Parser.h>
#include <map>
#include <vector>
#include <string>

namespace kgl
{
  namespace vk
  {
    struct ManagerData
    {
      typedef unsigned                      Version    ;
      typedef std::map<Version, Module*>    ModuleMap  ;
      typedef std::map<std::string, Graph*> NodeGraphs ;
      
      ::karma::config::Configuration config ;
      
      std::string config_path ;
      Loader      loader      ;
      NodeGraphs  graphs      ;
      
      /**
       * @param name
       */
      void addGraph( const char* name ) ;
    };
    
    void ManagerData::addGraph( const char* name )
    {
      if( this->graphs.find( name ) == this->graphs.end() )
      {
        Graph* graph ;
        
        graph = new Graph() ;
        graph->subscribe ( name, this->graphs.size() ) ;
        graph->setName   ( name                      ) ;
        graph->initialize( &this->loader             ) ;
        this->graphs.insert( { name, graph } ) ;
      }
    }

    Manager::Manager()
    {
      this->man_data = new ManagerData() ;
    }

    Manager::~Manager()
    {
      delete this->man_data ;
    }

    void Manager::initialize( const char* mod_path, const char* configuration_path )
    {
      ::data::module::Bus bus ;
      
      bus.setChannel( 0 ) ;

      data().config_path = configuration_path ;
      
      data().loader.initialize( mod_path ) ;
      bus( "Graphs" ).attach( this->man_data, &ManagerData::addGraph ) ;
      data().config.initialize( data().config_path.c_str(), 0 ) ;
    }

    void Manager::start()
    {
      unsigned index ;
      
      index = 0 ;
      for( auto &graph : data().graphs ) 
      {
        // Push config once to build nodes.
        data().config.initialize( data().config_path.c_str(), index ) ;
        
        // Push a second time to configure nodes.
        data().config.initialize( data().config_path.c_str(), index ) ;

        graph.second->kick() ;

        index++ ;
      }
    }
    
    void Manager::stop()
    {
      for( auto graph : data().graphs )
      {
        graph.second->stop() ;
      }
    }
    
    void Manager::shutdown()
    {
      for( auto graph : data().graphs )
      {
        graph.second->reset() ;
      }
      
      
    }

    ManagerData& Manager::data()
    {
      return *this->man_data ;
    }

    const ManagerData& Manager::data() const
    {
      return *this->man_data ;
    }
  }
}