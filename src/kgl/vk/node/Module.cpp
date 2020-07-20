#include "Module.h"
#include <Bus.h>
#include <Signal.h>
#include <condition_variable>
#include <mutex>
#include <string>
#include <limits.h>

namespace kgl
{
  namespace vk
  {
    const unsigned DATA_BUS_SUBSCRIPTION_CHANNEL = UINT_MAX ;

    /** Structure to contain the Module object's internal data.
     */
    struct ModuleData
    {
      typedef bool Flag ;
      
      std::condition_variable variable   ; ///< TODO
      std::mutex              lock       ; ///< TODO
      std::string             name       ; ///< TODO
      std::string             type       ; ///< TODO
      unsigned                version    ; ///< TODO
      Flag                    running    ; ///< TODO
      Flag                    should_run ; ///< TODO
      ::data::module::Bus     bus        ; ///< TODO
              
      /**
       */
      ModuleData() ;
    };
    
    ModuleData::ModuleData()
    {
      this->name       = ""    ;
      this->running    = false ;
      this->should_run = false ;
    }

    Module::Module()
    {
      this->module_data = new ModuleData() ;
    }
    
    Module::~Module()
    {
      delete this->module_data ;
    }

    void Module::kick()
    {
      data().variable.notify_all() ;
    }
    
    void Module::subscribe( const char* pipeline, unsigned id )
    {
      data().bus.setChannel( id ) ;
    }
            
    void Module::start()
    {
      std::unique_lock<std::mutex> lock ;
      
      data().running    = true ;
      data().should_run = true ;

      while( data().should_run ) 
      {
        lock = std::unique_lock<std::mutex>( data().lock ) ;
        data().variable.wait( lock ) ;
        
        this->execute() ;
        
        data().lock.unlock() ;
      }
      
      data().running = false ;
    }

    bool Module::stop()
    {
      data().should_run = false ;
      
      return data().running ; // TODO fix this, shutdown is borken.
    }

    void Module::setName( const char* name )
    {
      data().name = name ;
    }
    
    void Module::setVersion( unsigned version )
    {
      data().version = version ;
    }
    
    void Module::setTypeName( const char* name )
    {
      data().type = name ;
    }
    
    const char* Module::type() const
    {
      return data().type.c_str() ;
    }
    
    unsigned Module::version() const
    {
      return data().version ;
    }

    const char* Module::name() const
    {
      return data().name.c_str() ;
    }

    ModuleData& Module::data()
    {
      return *this->module_data ;
    }

    const ModuleData& Module::data() const
    {
      return *this->module_data ;
    }
  }
}