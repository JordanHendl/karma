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

#include "../uwuwriter/UWUWriter.h"
#include "../shader/UWUShader.h"
#include "ArgumentParser.h"
#include <string>
#include <iostream>
#include <filesystem>
#include <fstream>

static std::string loadStream( std::ifstream& stream ) ;
static ::tools::ShaderStage extensionToStage( std::string extension ) ;
static std::string getExtension( const std::string& name ) ;

std::string loadStream( std::ifstream& stream )
{
  std::string buff ;
  buff = std::string( ( std::istreambuf_iterator<char>( stream ) ), std::istreambuf_iterator<char>() ) ;

  return buff ;
}

std::string getExtension( const std::string& name )
{
  return ( name.rfind( '.' ) == std::string::npos ) ? "" : name.substr( name.rfind( '.' ) + 1 ) ;
}

::tools::ShaderStage extensionToStage( std::string extension )
{
       if( extension == "frag" ) return ::tools::ShaderStage::FRAGMENT      ;
  else if( extension == "geom" ) return ::tools::ShaderStage::GEOMETRY      ;
  else if( extension == "tesc" ) return ::tools::ShaderStage::TESSALATION_C ;
  else if( extension == "tess" ) return ::tools::ShaderStage::TESSELATION_E ;
  else if( extension == "comp" ) return ::tools::ShaderStage::COMPUTE       ;
  else if( extension == "vert" ) return ::tools::ShaderStage::VERTEX        ;
  
  return ::tools::ShaderStage::VERTEX ;
}

int main( int argc, const char** argv )
{
  std::ifstream                   stream           ;
  ::tools::shader::ArgumentParser parser           ;
  ::tools::UWUWriter              shader           ;
  ::tools::shader::UWUShader      shader_validator ;
  std::string                     recursive_path   ;
  std::string                     directory_name   ;

  parser.parse( argc, argv ) ;
  if( parser.valid() )
  {
    if( parser.recursive() )
    {
      recursive_path = std::string( parser.recursionDirectory() ) ;
      for( auto& file : std::filesystem::directory_iterator( recursive_path ) )
      {
        if( file.is_directory() ) 
        {
          if( !directory_name.empty() ) shader.save( directory_name.c_str() ) ;
          directory_name = file.path() ;
        }
        else
        {
          shader.compile( extensionToStage( getExtension( file.path().string() ) ), file.path().string().c_str() ) ;
        }
      }      
    }

    for( unsigned i = 0; i < parser.getNumberOfInputs(); i++ )
    {
      stream.open( parser.getFilePath( i ) ) ;
      if( stream )
      {
        shader.compile( static_cast<::tools::ShaderStage>( parser.getShaderType( i ) ), loadStream( stream ).c_str() ) ;
        stream.close() ;
      }
    }
    
    shader.save( parser.output() ) ;
    
    if( parser.verbose() )
    {
      shader_validator.load( parser.output() ) ;
      
      for( auto sh = shader_validator.begin(); sh != shader_validator.end(); ++sh )
      {
        std::cout << "Shader Stage:   " << sh.stage()         << "\n" ;
        std::cout << "Num Uniforms:   " << sh.numUniforms()   << "\n" ;
        std::cout << "Num Attributes: " << sh.numAttributes() << "\n" ;
        
        for( unsigned i = 0; i < sh.numAttributes(); i++ )
        {
          std::cout << "  Attribute Name       : " << sh.attributeName    ( i ) << "\n" ;
          std::cout << "  Attribute Type       : " << sh.attributeType    ( i ) << "\n" ;
          std::cout << "  Attribute Size       : " << sh.attributeByteSize    ( i ) << "\n" ;
          std::cout << "  Attribute Location   : " << sh.attributeLocation( i ) << "\n" ;
          std::cout << "  Attribute is INPUT   : " << sh.attributeIsInput ( i ) << "\n" ;
          std::cout << "\n" ;
        }
        std::cout << "     -+-+ -+-+ -+-+ -+-+ -+-+ -+-+ -+-+\n\n" ;
        for( unsigned i = 0; i < sh.numUniforms(); i++ )
        {
          std::cout << "  Uniform Name   : " << sh.uniformName   ( i ) << "\n" ;
          std::cout << "  Uniform Binding: " << sh.uniformBinding( i ) << "\n" ;
          std::cout << "  Uniform Type   : " << sh.uniformType   ( i ) << "\n" ;
          std::cout << "  Uniform Size   : " << sh.uniformSize   ( i ) << "\n" ;
          std::cout << "\n" ;
        }
        std::cout << "\n" ;
        std::cout << "---------------------------------------------\n" ;
      }
    }
  }
  else
  {
    std::cout << parser.usage() << std::endl ;
    return -1 ;
  }  
  return 0 ;
}