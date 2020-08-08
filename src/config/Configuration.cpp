/**********************************************************************
 * Copyright (C) 2020 Jordan Hendl - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the <INSERT_LICENSE>
 *
 * You should have received a copy of the <INSERT_LICENSE> license with
 * this file. If not, please write to: jordiehendl@gmail.com.
 * Author       : Jordan Hendl
 * File         : Configuration.cpp
 * Version      : 1.0
 * Date created : 6/21/2020
 * Purpose      : Definitions for Configuration object to configure an entire program pipeline.
**********************************************************************/
#include "Configuration.h"
#include "Parser.h"
#include <Bus.h>
#include <Signal.h>
#include <fstream>
#include <istream>
#include <iostream>

namespace karma
{
  namespace config
  {
    /** The data structure to contain all of the Configuration object's data.
     */
    struct ConfigurationData
    {
      ::data::module::Bus         bus    ; ///< The bus to sent data over.
      karma::config::json::Parser parser ; ///< The parser to use to parse the configuration.
      json::Token                 begin  ; ///< The beginning of this object's internal parsed data.
      json::Token                 end    ; ///< The end of this object's internal parsed data.
    };

    Configuration::Configuration()
    {
      this->config_data = new ConfigurationData() ;
    }

    Configuration::~Configuration()
    {
      delete this->config_data ;
    }

    const json::Token& Configuration::begin() const
    {
      return data().begin ;
    }

    const json::Token& Configuration::end() const
    {
      return data().end ;
    }

    void Configuration::initialize( const char* path, unsigned channel )
    {
      std::string   file   ;
      std::ifstream stream ;
      
      data().bus.setChannel( channel ) ;

      stream.open( path ) ;
      data().parser.clear() ;
      if( stream )
      {
        // Copy stream's contents into string.
        stream.seekg  ( 0, std::ios::end  ) ;
        file  .reserve( stream.tellg()    ) ;
        stream.seekg  ( 0, std::ios::beg  ) ;

        file.assign ( ( std::istreambuf_iterator<char>( stream ) ), std::istreambuf_iterator<char>() ) ;

        // Feed data to parser.
        data().parser.initialize( file.c_str() ) ;
        data().begin = data().parser.begin() ;
        data().end   = data().parser.end  () ;

        stream.close() ;

        // Send parsed data over event bus.
        for( auto token = data().begin; token != data().end; ++token )
        {
          if( token.isArray() )
          {
            for( unsigned index = 0; index < token.size(); index++ )
            {
              // TODO- Look for a better way than just emitting over all types.
              data().bus( token.key() ).emit( index, token.string  ( index ) ) ;
              data().bus( token.key() ).emit( index, token.number  ( index ) ) ;
              data().bus( token.key() ).emit( index, token.boolean ( index ) ) ;
              data().bus( token.key() ).emit( index, token.decimal ( index ) ) ;

              data().bus( token.key() ).emit( token.string  ( index ) ) ;
              data().bus( token.key() ).emit( token.number  ( index ) ) ;
              data().bus( token.key() ).emit( token.boolean ( index ) ) ;
              data().bus( token.key() ).emit( token.decimal ( index ) ) ;
            }
          }
          else
          {
            data().bus( token.key() ).emit<const char*>( token.string  () ) ;
            data().bus( token.key() ).emit<unsigned   >( token.number  () ) ;
            data().bus( token.key() ).emit<bool       >( token.boolean () ) ;
            data().bus( token.key() ).emit<float      >( token.decimal () ) ;
          }
        }
      }
    }

    ConfigurationData& Configuration::data()
    {
      return *this->config_data ;
    }

    const ConfigurationData& Configuration::data() const
    {
      return *this->config_data ;
    }
  }
}