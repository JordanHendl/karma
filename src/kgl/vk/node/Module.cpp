#include "Module.h"
#include <Bus.h>
#include <Signal.h>
#include <string>
#include <limits.h>
#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>

#ifdef _WIN32
  #include <windows.h>

  static inline void setThreadPriority()
  {
    SetThreadPriority( GetCurrentThread(), THREAD_PRIORITY_BELOW_NORMAL ) ;
  }

#elif __linux__ 
  #include <pthread.h>
#include <sys/resource.h>
#include <condition_variable>

  static inline void setThreadPriority()
  {
    // TODO because I don't think this is doable without writing my own thread library which is stupid. Why tf can't std thread handle this?
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
      
      std::string             name       ; ///< The name of this module.
      std::string             type       ; ///< The type of module this object is.
      unsigned                version    ; ///< The version of module.
      Flag                    running    ; ///< Whether or not this module is running.
      Flag                    should_run ; ///< Whether or not this module should be running.
      ::data::module::Bus     bus        ; ///< The bus to communicate data over.
      unsigned                num_deps   ; ///< The number of dependancies this object has.
      std::atomic_uint        wait_sem   ; ///< The semaphore to wait on for dependancies to be met.
      unsigned                id         ; ///< The id associated with this module.
      std::mutex              mutex      ; ///< The mutex for synchronization.
      std::condition_variable cv         ; ///< The condition variable to wait on for dependancies to be met.

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
      std::unique_lock<std::mutex> lock ;
      
      data().running    = true ;
      data().should_run = true ;
      
      setThreadPriority() ;
      
      while( data().should_run )
      {
        lock = std::unique_lock( data().mutex ) ;
        data().cv.wait( lock, [=]{ return data().wait_sem >= data().num_deps ; } ) ;
        data().mutex.unlock() ; 
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
      data().wait_sem.fetch_add( 1 ) ;
      
      if( data().wait_sem >= data().num_deps ) data().cv.notify_one() ;
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
