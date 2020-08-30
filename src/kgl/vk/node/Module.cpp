#include "Module.h"
#include <Bus.h>
#include <Signal.h>
#include <string>
#include <limits.h>
#include <iostream>
#include <thread>
#include <chrono>

#ifdef _WIN32
  #include <windows.h>
  
  static inline void setThreadPriority()
  {
    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_BELOW_NORMAL ) )
  }

#elif __linux__ 
  #include <pthread.h>
#include <sys/resource.h>

  static inline void setThreadPriority()
  {
    setpriority( PRIO_PROCESS, 0, 30 ) ;
    pthread_setschedprio( pthread_self(), 1 ) ;
  }
#endif
  
namespace kgl
{
  namespace vk
  {
    /** Structure to contain the Module object's internal data.
     */
    struct ModuleData
    {
      typedef bool Flag ;
      
      std::string             name       ; ///< TODO
      std::string             type       ; ///< TODO
      unsigned                version    ; ///< TODO
      Flag                    running    ; ///< TODO
      Flag                    should_run ; ///< TODO
      ::data::module::Bus     bus        ; ///< TODO
      unsigned                num_deps   ;
      unsigned                wait_sem   ;
      unsigned                id         ;
      /**
       */
      ModuleData() ;
    };
    
    ModuleData::ModuleData()
    {
      this->name       = ""    ;
      this->running    = false ;
      this->should_run = false ;
      this->num_deps   = 0     ;
      this->wait_sem   = 0     ;
      this->id         = 0     ;
    }

    Module::Module()
    {
      this->module_data = new ModuleData() ;
    }
    
    Module::~Module()
    {
      delete this->module_data ;
    }
    
    void Module::subscribe( const char* pipeline, unsigned id )
    {
      data().bus.setChannel( id ) ;
    }
            
    void Module::start()
    {
      data().running    = true ;
      data().should_run = true ;
      
//      setThreadPriority() ;
      
      while( data().should_run )
      {
        while ( data().wait_sem < data().num_deps ) { std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) ) ; } ;

        data().wait_sem = 0 ;

        this->execute() ;
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
    
    unsigned Module::id() const
    {
      return data().id ;
    }
    
    void Module::setId( unsigned id )
    {
      data().id = id ;
    }

    const char* Module::type() const
    {
      return data().type.c_str() ;
    }
    
    unsigned Module::version() const
    {
      return data().version ;
    }

    void Module::setNumDependancies( unsigned count )
    {
      data().num_deps = count ;
    }
    
    void Module::semIncrement()
    {
      data().wait_sem++ ;
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