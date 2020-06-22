/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   casper_signal.cpp
 * Author: jhendl
 * 
 * Created on February 24, 2020, 1:52 AM
 */

#include <memory>
#include <map>

#include "Signal.h"

namespace data
{
  namespace module
  {
    struct SignalData
    {
      typedef std::multimap<SignalType, Signal::Callback*       > CallbackMap        ;
      typedef std::multimap<SignalType, Signal::IndexedCallback*> IndexedCallbackMap ;
      
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
    
    void Signal::attachBase(SignalType type, Callback* callback)
    {
      data().callbacks.insert( { type, callback } ) ;
    }
    
    void Signal::attachBase(SignalType type, IndexedCallback* callback)
    {
      data().indexed_callbacks.insert( { type, callback } ) ;
    }
    
    void Signal::emitBase(SignalType type, const void* ptr)
    {
      for( auto it : data().callbacks ) 
      {
        it.second->execute( ptr ) ;
      }
    }
    
    void Signal::emitBase(SignalType type, unsigned idx, const void* ptr)
    {
      for( auto it : data().indexed_callbacks ) 
      {
        it.second->execute( idx, ptr ) ;
      }
    }
    
  }
}
