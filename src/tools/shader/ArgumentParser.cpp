/**********************************************************************
 * Copyright (C) 2020 Jordan Hendl - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the <INSERT_LICENSE>
 *
 * You should have received a copy of the <INSERT_LICENSE> license with
 * this file. If not, please write to: jordiehendl@gmail.com.
 * Author       : Jordan Hendl
 * File         : Bus.h
 * Version      : 1.0
 * Date created : 6/22/2020
 * Purpose      : Declaration for Data bus object.
**********************************************************************/
#include "ArgumentParser.h"
#include "UWUShader.h"
#include <string>
#include <vector>
#include <map>
namespace tools
{
  namespace shader
  {
    /**
     * @param name 
     * @return std::string 
     */
    static inline std::string getExtension( const std::string& name ) ;

    /**
     * @param extension 
     * @return ::tools::shader::ShaderStage 
     */
    static ::tools::shader::ShaderStage extensionToStage( std::string extension ) ;

    struct ArgParserData
    {
      std::string              include_directory   ;
      std::string              recursive_directory ;
      std::string              output_path         ;
      bool                     verbose             ;
      std::vector<std::string> shaders_paths       ;

      ArgParserData() ;
    };

    std::string getExtension( const std::string& name )
    {
      return ( name.rfind( '.' ) == std::string::npos ) ? "" : name.substr( name.rfind( '.' ) + 1 ) ;
    }

    static ::tools::shader::ShaderStage extensionToStage( std::string extension )
    {
           if( extension == "frag" ) return ::tools::shader::ShaderStage::FRAGMENT      ;
      else if( extension == "geom" ) return ::tools::shader::ShaderStage::GEOMETRY      ;
      else if( extension == "tesc" ) return ::tools::shader::ShaderStage::TESSALATION_C ;
      else if( extension == "tess" ) return ::tools::shader::ShaderStage::TESSELATION_E ;
      else if( extension == "comp" ) return ::tools::shader::ShaderStage::COMPUTE       ;
      else if( extension == "vert" ) return ::tools::shader::ShaderStage::VERTEX        ;

      return ::tools::shader::ShaderStage::VERTEX ;
    }

    ArgParserData::ArgParserData()
    {
      this->include_directory   = ""        ;
      this->recursive_directory = ""        ;
      this->output_path         = "out.uwu" ;
      this->shaders_paths       = {}        ;
      this->verbose             = false     ;
    }

    ArgumentParser::ArgumentParser()
    {
      this->arg_data = new ArgParserData() ;
    }

    ArgumentParser::~ArgumentParser()
    {
      delete this->arg_data ;
    }

    void ArgumentParser::parse( int num_inputs, const char** argv )
    {
      std::string buffer ;
      
      for( unsigned index = 1; index < num_inputs; index++ )
      {
        buffer = std::string( argv[ index ] ) ;

        if     ( buffer == "-i" && index + 1 < num_inputs ) { data().include_directory   = std::string( argv[ index + 1 ] ) ; index++ ; }
        else if( buffer == "-o"  && index + 1 < num_inputs ) { data().output_path         = std::string( argv[ index + 1 ] ) ; index++ ; }
        else if( buffer == "-r"       && index + 1 < num_inputs ) { data().recursive_directory = std::string( argv[ index + 1 ] ) ; index++ ; }
        else if( buffer == "-v"                                 ) { data().verbose             = true                             ;           }
        else                                                      { data().shaders_paths.push_back( std::string( argv[ index ] ) )          ; }
      }
    }
    
    bool ArgumentParser::verbose() const
    {
      return data().verbose ;
    }

    bool ArgumentParser::recursive() const
    {
      return data().recursive_directory != "" ;
    }
    
    const char* ArgumentParser::output() const
    {
      return ( data().output_path + ".uwu" ).c_str() ;
    }

    const char* ArgumentParser::recursionDirectory() const
    {
      return data().recursive_directory.c_str() ;
    }

    int ArgumentParser::getShaderType( unsigned index ) const
    {
      if( index < data().shaders_paths.size () ) 
      {
        return ::tools::shader::extensionToStage( ::tools::shader::getExtension( data().shaders_paths.at( index ) ) ) ;
      }

      return 0 ;
    }

    unsigned ArgumentParser::getNumberOfInputs() const
    {
      return data().shaders_paths.size() ;
    }

    const char* ArgumentParser::getFilePath( unsigned index ) const
    {
      return index < data().shaders_paths.size() ? data().shaders_paths[ index ].c_str() : "" ;
    }
    
    const char* ArgumentParser::usage() const
    {
      static const std::string program_usage = std::string( 
      "Usage:: glslToUWU <shader.type1> <shader2.type2> ... <options>\n"  
      "  Options: -r <directory>\n"                                       
      "              -> Recursively parses all files in the directory.\n" 
      "           -i <directory>\n"                                 
      "              -> Sets the directory on the filesystem to use as the include directory for GLSL shader compilation.\n" 
      "           -v\n"                                                   
      "              -> Verbose output.\n"
      "           -o <name>\n"                                                   
      "              -> The output name, if not using recursive. File extension is automatically appended to input\n"
                                                          ) ;
      return program_usage.c_str() ;
    }

    bool ArgumentParser::valid() const
    {
      return !data().shaders_paths.empty() ;
    }

    ArgParserData& ArgumentParser::data()
    {
      return *this->arg_data ;
    }

    const ArgParserData& ArgumentParser::data() const
    {
      return *this->arg_data ;
    }
  }
}