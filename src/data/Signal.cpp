#include "Signal.h"
#include <memory>
#include <map>
#include <vector>
#include <mutex>
namespace data
{
  namespace module
  {

    static std::mutex mutex ;

    struct SignalData
    {
      struct Dependancy
      {
        public:
        std::map<Signal::Callback*, bool> callbacks ;
        
        Signal::DependancyCallback* callback  ;
        unsigned          count     ;
        unsigned          satisfied ;

        Dependancy()
        {
          this->count = 0 ;
        }
        
        void clear()
        {
          this->callbacks.clear() ;
        };

        bool met()
        {
          return this->callbacks.size() >= this->count ;
        };
        
        void execute()
        {
          this->callback->execute() ;
          
          mutex.lock() ;
          this->clear() ;
          mutex.unlock() ;
        };

        bool callbackExecuted( Signal::Callback* callback )
        {
          mutex.lock() ;
          return this->callbacks.find( callback ) != this->callbacks.end() ;
          mutex.unlock() ;
        }
        
        void called( Signal::Callback* callback )
        {
          mutex.lock() ;
          auto iter = this->callbacks.find( callback ) ;
          
          if( iter == this->callbacks.end() ) this->callbacks.insert( { callback, true } ) ;
          mutex.unlock() ;
        }
      };

      struct CallbackEntry
      {
        Signal::Callback* callback   ;
        Dependancy*       dependancy ;
        
        CallbackEntry()
        {
          this->callback   = nullptr ;
          this->dependancy = nullptr ;
        }
        
        bool hasDependancy()
        {
          return this->dependancy != nullptr ;
        }
        
        bool dependancyMet()
        {
          return this->hasDependancy() ? this->dependancy->met() : false ;
        }
        
        void execute( const void* data )
        {
          this->callback->execute( data ) ;
          
          if( this->hasDependancy() )
          {
            this->dependancy->called( callback ) ;
          }
        }
        
        void executeDependancy()
        {
          if( this->hasDependancy() ) this->dependancy->execute() ;
        }
      };
      
      typedef std::multimap<long, CallbackEntry           > CallbackMap        ;
      typedef std::multimap<long, Signal::IndexedCallback*> IndexedCallbackMap ;
      typedef std::map<std::string, Dependancy>             DependancyMap      ;

      static inline DependancyMap dependancies ;
      
      CallbackMap        callbacks         ;
      IndexedCallbackMap indexed_callbacks ;
      std::string        key               ;
    };
    
    
    
    
    SignalType::SignalType()
    {
      this->ident = NULL ;
    }
    
    SignalType::SignalType( Identifier id )
    {
      this->ident = id ;
    }
    
    bool inline SignalType::operator <( SignalType type ) const
    {
      return this->ident < type.ident ;
    }
    
    Signal::Signal( const char* key )
    {
      this->signal_data = new SignalData() ;
      data().key = key ;
    }
    
    void Signal::addDependancyBase( unsigned type, Callback* callback, const char* dependancy )
    {
      for( auto it = data().callbacks.lower_bound( type ); it != data().callbacks.upper_bound( type ); ++it ) 
      {
        auto dep = data().dependancies.find( dependancy ) ;
        
        if( dep != data().dependancies.end() ) { it->second.dependancy = &dep->second ; dep->second.count++ ; } ;
      }
    }

    void Signal::addDependancyCallbackBase( DependancyCallback* callback, const char* dependancy )
    {
      auto iter = data().dependancies.find( dependancy ) ;
      
      if( iter == data().dependancies.end() )
      {
        data().dependancies.insert( { dependancy, SignalData::Dependancy() } ) ;
        data().dependancies.at( dependancy ).callback = callback ;
      }
    }
        
    Signal::~Signal()
    {
      for( auto it = data().callbacks.begin(); it != data().callbacks.end(); ++it )
      {
        delete( it->second.callback ) ;
      }
      
      for( auto it = data().indexed_callbacks.begin(); it != data().indexed_callbacks.end(); ++it )
      {
        delete( it->second ) ;
      }
    }
    
    SignalData& Signal::data()
    {
      return *this->signal_data ;
    }
    
    const SignalData& Signal::data() const
    {
      return *this->signal_data ;
    }
    
    void Signal::attachBase( unsigned type, Callback* callback)
    {
      SignalData::CallbackEntry entry ;
      
      entry.callback = callback ;
      data().callbacks.insert( { type, entry } ) ;
    }
    
    void Signal::attachBase( unsigned type, IndexedCallback* callback)
    {
      SignalData::CallbackEntry entry ;
      
//      entry.callback = callback ;

      data().indexed_callbacks.insert( { type, callback } ) ;
    }
    
    void Signal::emitBase( unsigned type, const void* ptr)
    {
      for( auto it = data().callbacks.lower_bound( type ); it != data().callbacks.upper_bound( type ); ++it ) 
      {
        it->second.execute( ptr ) ;
        
        if( it->second.dependancyMet() )
        {
          it->second.executeDependancy() ;
        }
      }
    }
    
    void Signal::emitBase( unsigned type, unsigned idx, const void* ptr)
    {
      for( auto it : data().indexed_callbacks ) 
      {
        it.second->execute( idx, ptr ) ;
      }
    }
    
  }
}
