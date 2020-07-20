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
#include <string>
#include <sstream>
#include <iostream>
#include <map>

namespace data
{
  namespace module
  {
    typedef std::map<std::string, Signal*> SignalMap ;
    typedef std::map<unsigned, SignalMap > BusPool   ;
    
    static BusPool pool ;
    
    void operator<<( String& first, const char* second )
    {
      std::stringstream stream ;
      std::string       string ;

      stream << first.str()  ;
      stream << second       ;
      
      string = stream.str() ;
      first.setStr( string.c_str() ) ;
    }

    void operator<<( String& first, unsigned second )
    {
      std::stringstream stream ;
      std::string       string ;

      stream << first.str()  ;
      stream << second       ;
      
      string = stream.str() ;
      first.setStr( string.c_str() ) ;
    }
    
    void operator<<( String& first, float second )
    {
      std::stringstream stream ;
      std::string       string ;
      
      stream << first.str() ;
      stream << second      ;
      
      string = stream.str() ;
      first.setStr( string.c_str() ) ;
    }
    
    void operator<<( String& first, const String& second )
    {
      std::stringstream stream ;
      std::string       string ;
      
      stream << first.str()  ;
      stream << second.str() ;
      
      string = stream.str() ;
      first.setStr( string.c_str() ) ;
    }

    struct StringData
    {
      std::string str ;
    };

    String::String()
    {
      this->string_data = new StringData() ;
    }

    String::String( const String& string )
    {
      this->string_data = new StringData() ;
      *this->string_data = *string.string_data ;
    }
    
    void String::operator=( const String& string )
    {
      *this->string_data = *string.string_data ;
    }
        
    String::~String()
    {
      delete this->string_data ;
    }
    
    void String::setStr( const char* str )
    {
      data().str = str ;
    }

    const char* String::str() const
    {
      return data().str.c_str() ;
    }

    StringData& String::data()
    {
      return *this->string_data ;
    }

    const StringData& String::data() const
    {
      return *this->string_data ;
    }

    Bus::Bus( unsigned id )
    {
      setChannel( id ) ;
    }
    
    Signal& Bus::getBase( const char* key  )
    {
      if( pool[ this->identifier ].find( key ) == pool[ this->identifier ].end() )
      {
        pool[ this->identifier ].insert( { key, new Signal( key ) } ) ;
      }

      return *pool[ this->identifier ][ key ] ;
    }

    const Signal& Bus::getBase( const char* key ) const
    {
      if( pool[ this->identifier ].find( key ) == pool[ this->identifier ].end() )
      {
        pool[ this->identifier ].insert( { key, new Signal( key ) } ) ;
      }

      return *pool[ this->identifier ][ key ] ;
    }

    unsigned Bus::id()
    {
      return this->identifier ;
    }

    void Bus::setChannel( unsigned id )
    {
      this->identifier = id ;
    }

  }
}

