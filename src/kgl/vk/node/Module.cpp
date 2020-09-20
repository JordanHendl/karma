#include "Module.h"
#include <Bus.h>
#include <Signal.h>
#include <string>
#include <limits.h>
#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>
#include <iostream>
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
#include <vector>

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
      struct Dependancy
      {
        std::atomic<unsigned>   wait_sem ;
        unsigned                num_deps ;
        std::condition_variable cv    ;
        std::mutex              mutex ;
        
        Dependancy()
        {
          this->wait_sem = 0 ;
          this->num_deps = 0 ;
        };
        
        Dependancy( const Dependancy& dep )
        {
          this->wait_sem.exchange( dep.wait_sem ) ;
          this->num_deps = dep.num_deps ;
        };
      };
      
      std::string             name         ; ///< The name of this module.
      std::string             type         ; ///< The type of module this object is.
      unsigned                version      ; ///< The version of module.
      Flag                    running      ; ///< Whether or not this module is running.
      Flag                    should_run   ; ///< Whether or not this module should be running.
      ::data::module::Bus     bus          ; ///< The bus to communicate data over.
//      unsigned                num_deps     ; ///< The number of dependancies this object has.
//      std::atomic_uint        wait_sem     ; ///< The semaphore to wait on for dependancies to be met.
      unsigned                id           ; ///< The id associated with this module.
      std::mutex              mutex        ; ///< The mutex for synchronization.
      std::condition_variable cv           ; ///< The condition variable to wait on for dependancies to be met.
      std::vector<Dependancy> dependancies ;
      std::atomic<bool>       flag         ;
      std::mutex xxx ;
      /**
       */
      ModuleData() ;
      
      bool dependanciesMet() ;
    };
    
    ModuleData::ModuleData()
    {
      this->name       = ""    ;
      this->running    = false ;
      this->should_run = false ;
//      this->num_deps   = 0     ;
//      this->wait_sem   = 0     ;
      this->id         = 0     ;
      this->flag       = false ;
    }
    
    bool ModuleData::dependanciesMet()
    {
      unsigned num_deps_met ;
      
      num_deps_met = 0 ;
      
      for( const auto& dep : this->dependancies )
      {
        if( dep.wait_sem >= dep.num_deps ) num_deps_met++ ;
      }
      return num_deps_met == this->dependancies.size() ;
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
      
      setThreadPriority() ;
      
      while( data().should_run )
      {
        std::unique_lock<std::mutex> lock ;
        lock = std::unique_lock( data().mutex ) ;
        data().flag = false ;
        data().cv.wait( lock, [=]
          { 
            for( const auto& dep : data().dependancies ) if( dep.wait_sem < dep.num_deps ) return false ;
            return true ;
          } ) ;
        data().flag = true ;
        
        this->execute() ;
        for( unsigned i = 0; i < data().dependancies.size(); i++ )
        {
          {
            std::lock_guard<std::mutex> dep_lock( data().dependancies[ i ].mutex ) ;
            data().dependancies[ i ].wait_sem = 0    ;
            data().dependancies[ i ].cv.notify_one() ;
          }
        }
      }
      
      data().running = false ;
    }

    bool Module::stop()
    {
      data().should_run = false ;
      
      return data().running ; // TODO fix this, shutdown is borken.
    }
    
    void Module::wait( unsigned dep_id )
    {
      
      std::unique_lock<std::mutex> lock ;
      lock = std::unique_lock( data().dependancies[ dep_id ].mutex ) ;
      data().dependancies[ dep_id ].cv.wait( lock, [=]{ return data().dependancies[ dep_id ].wait_sem == 0 ; } ) ;
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

    void Module::setNumDependancies( unsigned count, unsigned dep_id )
    {
      if( data().dependancies.size() <= dep_id ) data().dependancies.resize( dep_id + 1 ) ;
      data().dependancies[ dep_id ].num_deps = count ;
    }
    
    void Module::semIncrement( unsigned dep_id )
    {
      bool notify ;
      
      notify = true ;
      
      data().dependancies[ dep_id ].wait_sem++ ;

      for( const auto& dep : data().dependancies ) if( dep.wait_sem < dep.num_deps )
      {
        notify = false ;
      }
      
      if( notify )
      {
        std::lock_guard<std::mutex> lock( data().mutex ) ;
        data().cv.notify_one() ;
      }
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
