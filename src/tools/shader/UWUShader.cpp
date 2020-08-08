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

#include "UWUShader.h"
#include <string>
#include <sstream>
#include <fstream>
#include <ostream>
#include <istream>
#include <iostream>
#include <vector>
#include <cerrno>
#include <memory>
#include <algorithm>
#include <ctype.h>
#include <map>
#include <limits.h>
#include <stdlib.h>

namespace tools
{
  namespace shader
  {
    struct Shader ;
    typedef std::map<shader::ShaderStage, Shader> ShaderMap ;

    const unsigned long long MAGIC = 0x555755200d0a ;

    static inline unsigned sizeFromType( std::string type_name ) ;

    unsigned sizeFromType( std::string type_name )
    {
           if( type_name == "mat4"     ) return sizeof( float    ) * 16 ;
      else if( type_name == "mat3"     ) return sizeof( float    ) * 9  ;
      else if( type_name == "vec4"     ) return sizeof( float    ) * 4  ;
      else if( type_name == "ivec4"    ) return sizeof( unsigned ) * 4  ;
      else if( type_name == "vec3"     ) return sizeof( float    ) * 3  ;
      else if( type_name == "ivec3"    ) return sizeof( unsigned ) * 3  ;
      else if( type_name == "vec2"     ) return sizeof( float    ) * 2  ;
      else if( type_name == "ivec2"    ) return sizeof( unsigned ) * 2  ;
      else if( type_name == "unsigned" ) return sizeof( float    ) * 3  ;
      else if( type_name == "float"    ) return sizeof( float    ) * 3  ;
      else if( type_name == "bool"     ) return sizeof( float    ) * 3  ;
      else { std::cout << "Unknown type : " << type_name << std::endl ; exit( -1 ) ; } ;
    }

    /**
     */
    struct Uniform
    {
      unsigned    binding ;
      unsigned    size    ;
      UniformType type    ;
      std::string name    ;
    };
    
    struct Attribute
    {
      bool        input    ;
      std::string name     ;
      std::string type     ;
      unsigned    size     ;
      unsigned    location ;
    };

    /** TODO
     */
    struct Shader
    {
      typedef std::vector<unsigned>   SpirVData     ;
      typedef std::vector<Uniform>    UniformList   ;
      typedef std::vector<Attribute>  AttributeList ;
  
      UniformList   uniforms   ;
      AttributeList attributes ;
      SpirVData     spirv      ;
      ShaderStage   stage      ;
      std::string   name       ;
    };
  
    /**
     */
    struct ShaderIteratorData
    {
      ShaderMap::const_iterator it ;
    };
  
    /**
     */
    struct UWUShaderData
    {
        std::string include_directory ;
        ShaderMap   map               ;

      /**
       * @param stream 
       * @return 
       */
      std::string readString( std::ifstream& stream ) const ;

      /**
       * @param stream 
       * @return 
       */
      unsigned readUnsigned( std::ifstream& stream ) const ;

      /**
       * @param stream 
       * @return 
       */
      bool readBoolean( std::ifstream& stream ) const ;

      /**
       * @param stream 
       * @return 
       */
      unsigned long long readMagic( std::ifstream& stream ) const ;

      /**
       * @param stream 
       * @return 
       */
      unsigned* readSpirv( std::ifstream& stream, unsigned sz ) const ;
    };

    std::string UWUShaderData::readString( std::ifstream& stream ) const
    {
      unsigned    sz  ;
      std::string out ;
     
      stream.read( (char*)&sz, sizeof( unsigned ) ) ;
      out.resize( sz ) ;
      stream.read( (char*)out.data(), sz ) ;

      return out ;
    }

    unsigned UWUShaderData::readUnsigned( std::ifstream& stream ) const
    {
      unsigned val ;

      stream.read( (char*)&val, sizeof( unsigned ) ) ;
      return val ;
    }

    bool UWUShaderData::readBoolean( std::ifstream& stream ) const
    {
      bool val ;

      stream.read( (char*)&val, sizeof( bool ) ) ;
      return val ;
    }

    unsigned long long UWUShaderData::readMagic( std::ifstream& stream ) const
    {
      unsigned long long val ;

      stream.read( (char*)&val, sizeof( unsigned long long ) ) ;
      return val ;
    }

    unsigned* UWUShaderData::readSpirv( std::ifstream& stream, unsigned sz ) const
    {
      unsigned* data = new unsigned[ sz ] ;
      stream.read( (char*)data, sz * sizeof( unsigned ) ) ;
      
      return data ;
    }
  
    ShaderIterator::ShaderIterator()
    {
      this->shader_iterator_data = new ShaderIteratorData() ;
    }
  
    ShaderIterator::ShaderIterator( const ShaderIterator& input )
    {
      this->shader_iterator_data = new ShaderIteratorData() ;
  
      *this->shader_iterator_data = *input.shader_iterator_data ;
    }
  
    ShaderIterator::~ShaderIterator()
    {
      delete this->shader_iterator_data ;
    }

    unsigned ShaderIterator::spirvSize() const
    {
      return data().it->second.spirv.size() ;
    }

    unsigned ShaderIterator::numUniforms() const
    {
      return data().it->second.uniforms.size() ;
    }
  
    const unsigned* ShaderIterator::spirv() const
    {
      return data().it->second.spirv.data() ;
    }
  
    ShaderStage ShaderIterator::stage() const
    {
      return data().it->first ;
    }
    
    unsigned ShaderIterator::numAttributes() const
    {
      return data().it->second.attributes.size() ;
    }
    
    const char* ShaderIterator::attributeType( unsigned index )
    {
      return index < data().it->second.attributes.size() ? data().it->second.attributes[ index ].type.c_str() : "" ;
    }

    const char* ShaderIterator::attributeName( unsigned index )
    {
      return index < data().it->second.attributes.size() ? data().it->second.attributes[ index ].name.c_str() : "" ;
    }

    unsigned ShaderIterator::attributeByteSize( unsigned index )
    {
      return index < data().it->second.attributes.size() ? data().it->second.attributes[ index ].size : 0 ;
    }

    unsigned ShaderIterator::attributeLocation( unsigned index )
    {
      return index < data().it->second.attributes.size() ? data().it->second.attributes[ index ].location : 0 ;
    }

    bool ShaderIterator::attributeIsInput( unsigned index )
    {
      return index < data().it->second.attributes.size() ? data().it->second.attributes[ index ].input : 0 ;
    }
    
    UniformType ShaderIterator::uniformType( unsigned id ) const
    {
      return id < data().it->second.uniforms.size() ? data().it->second.uniforms[ id ].type : UniformType::None ;
    }
  
    unsigned ShaderIterator::uniformSize( unsigned id ) const
    {
      return id < data().it->second.uniforms.size() ? data().it->second.uniforms[ id ].size : 0 ;
    }

    unsigned ShaderIterator::uniformBinding( unsigned id ) const
    {
      return id < data().it->second.uniforms.size() ? data().it->second.uniforms[ id ].binding : UINT_MAX ;
    }
  
    const char* ShaderIterator::uniformName( unsigned id ) const
    {
      return id < data().it->second.uniforms.size() ? data().it->second.uniforms[ id ].name.c_str() : "OutOfBoundsAccess" ;
    }
  
    void ShaderIterator::operator++()
    {
      ++data().it ;
    }
  
    void ShaderIterator::operator=( const ShaderIterator& input )
    {
      data().it = input.data().it ;
    }
  
    bool ShaderIterator::operator!=( const ShaderIterator& input )
    {
      return data().it != input.data().it ;
    }
  
    ShaderIteratorData& ShaderIterator::data()
    {
      return *this->shader_iterator_data ;
    }
  
    const ShaderIteratorData& ShaderIterator::data() const
    {
      return *this->shader_iterator_data ;
    }
  
    UWUShader::UWUShader()
    {
      this->compiler_data = new UWUShaderData() ;
    }
  
    UWUShader::~UWUShader()
    {
      delete this->compiler_data ;
    }

    void UWUShader::load( const char* path )
    {
      std::ifstream              stream  ;
      std::string                str     ;
      unsigned                   sz      ;
      unsigned long long         magic   ;
      ::tools::shader::Shader    shader  ;
      ::tools::shader::Uniform   uniform ;
      ::tools::shader::Attribute attr    ;

      data().map.clear() ;
      stream.open( path, std::ios::binary ) ;
      
      if( stream )
      {
        magic = data().readMagic( stream ) ;        
        if( magic != ::tools::shader::MAGIC ) /*TODO: LOG ERROR HERE */ return ;

        sz = data().readUnsigned( stream ) ;
        for( unsigned it = 0; it < sz; it++ )
        {
          const unsigned  spirv_size     = data().readUnsigned( stream             ) ;
          const unsigned* spirv          = data().readSpirv   ( stream, spirv_size ) ;
          const unsigned  stage          = data().readUnsigned( stream             ) ;
          const unsigned  num_uniforms   = data().readUnsigned( stream             ) ;
          const unsigned  num_attributes = data().readUnsigned( stream             ) ;

          shader.spirv   .clear() ;
          shader.uniforms.clear() ;

          shader.spirv     .assign( spirv, spirv + spirv_size ) ;
          shader.uniforms  .resize( num_uniforms              ) ;
          shader.attributes.resize( num_attributes            ) ;
          shader.stage = static_cast<::tools::shader::ShaderStage>( stage ) ;
          for( unsigned index = 0; index < num_uniforms; index++ )
          {
             const std::string name         = data().readString  ( stream ) ;
             const unsigned uniform_type    = data().readUnsigned( stream ) ;
             const unsigned uniform_binding = data().readUnsigned( stream ) ;
             const unsigned uniform_size    = data().readUnsigned( stream ) ;

             uniform.name    = name                                                      ;
             uniform.type    = static_cast<::tools::shader::UniformType>( uniform_type ) ;
             uniform.binding = uniform_binding                                           ;
             uniform.size    = uniform_size                                              ;

             shader.uniforms[ index ] = uniform ;
          }
          for( unsigned index = 0; index < num_attributes; index++ )
          {
            const std::string name  = data().readString  ( stream ) ;
            const std::string type  = data().readString  ( stream ) ;
            const unsigned    size  = data().readUnsigned( stream ) ;
            const unsigned location = data().readUnsigned( stream ) ;
            const bool     input    = data().readBoolean ( stream ) ;
            
            attr.name     = name     ;
            attr.location = location ;
            attr.size     = size     ;
            attr.type     = type     ;
            attr.input    = input    ;
            
            shader.attributes[ index ] = attr ;
          }

          data().map.insert( { shader.stage, shader } ) ;
        }
      }
    }
    
    ShaderIterator UWUShader::begin() const
    {
      ShaderIterator it ;
      it.data().it = data().map.begin() ;

      return it ;
    }

    ShaderIterator UWUShader::end() const 
    {
      ShaderIterator it ;
      it.data().it = data().map.end() ;

      return it ;
    }

    unsigned UWUShader::size() const
    {
      return data().map.size() ;
    }
  
    UWUShaderData& UWUShader::data()
    {
      return *this->compiler_data ;
    }
  
    const UWUShaderData& UWUShader::data() const
    {
      return *this->compiler_data ;
    }
  }
}