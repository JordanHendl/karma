#include "Signal.h"
#include <memory>
#include <map>


namespace data
{
  namespace module
  {
    struct SignalData
    {
      typedef std::multimap<long, Signal::Callback*       > CallbackMap        ;
      typedef std::multimap<long, Signal::IndexedCallback*> IndexedCallbackMap ;
      
      CallbackMap        callbacks         ;
      IndexedCallbackMap indexed_callbacks ;
    };
    
    SignalType::SignalType()
    {
      this->ident = NULL ;
    }
    
    SignalType::SignalType( Identifier id )
    {
      this->ident = id ;
    }
    
    bool SignalType::operator <( SignalType type ) const
    {
      return this->ident < type.ident ;
    }
    
    Signal::Signal()
    {
      this->signal_data = new SignalData() ;
    }
    
    Signal::~Signal()
    {
      for( auto it = data().callbacks.begin(); it != data().callbacks.end(); ++it )
      {
        delete( it->second ) ;
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
      data().callbacks.insert( { type, callback } ) ;
    }
    
    void Signal::attachBase( unsigned type, IndexedCallback* callback)
    {
      data().indexed_callbacks.insert( { type, callback } ) ;
    }
    
    void Signal::emitBase( unsigned type, const void* ptr)
    {
      for( auto it = data().callbacks.lower_bound( type ); it != data().callbacks.upper_bound( type ); ++it ) 
      {
        it->second->execute( ptr ) ;
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
