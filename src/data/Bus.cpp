/**********************************************************************
 * Copyright (C) 2020 Jordan Hendl - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the <INSERT_LICENSE>
 *
 * You should have received a copy of the <INSERT_LICENSE> license with
 * this file. If not, please write to: jordiehendl@gmail.com.
 * Author       : Jordan Hendl
 * File         : Bus.cpp
 * Version      : 1.0
 * Date created : 6/22/2020
 * Purpose      : Definitions for Data bus object.
**********************************************************************/
#include "Bus.h"
#include "Signal.h"
#include <map>
#include <string>

namespace data
{
  namespace module
  {
    typedef std::map<std::string, Signal> SignalMap ;
    typedef std::map<unsigned, SignalMap> BusPool   ;
    
    static BusPool pool ;
    
    Bus::Bus( unsigned id )
    {
      setId( id ) ;
    }

    Signal& Bus::operator[]( const char* key )
    {
      return pool[ this->identifier ][ key ] ;
    }

    const Signal& Bus::operator[]( const char* key ) const
    {
      return pool[ this->identifier ][ key ] ;
    }

    Signal& Bus::get( const char* key )
    {
      return pool[ this->identifier ][ key ] ;
    }

    const Signal& Bus::get( const char* key ) const
    {
      return pool[ this->identifier ][ key ] ;
    }

    unsigned Bus::id()
    {
      return this->identifier ;
    }

    void Bus::setId( unsigned id )
    {
      this->identifier = id ;
    }

  }
}

