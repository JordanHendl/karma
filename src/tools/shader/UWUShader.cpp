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
#include <glslang/Public/ShaderLang.h>
#include <glslang/SPIRV/GlslangToSpv.h>
#include <glslang/StandAlone/DirStackFileIncluder.h>
#include <string>
#include <sstream>
#include <fstream>
#include <ostream>
#include <istream>
#include <vector>
#include <cerrno>
#include <string> 
#include <iostream>
#include <memory>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <variant>
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

    const static constexpr TBuiltInResource DefaultTBuiltInResource = 
    {
      /* .MaxLights = */ 32,
      /* .MaxClipPlanes = */ 6,
      /* .MaxTextureUnits = */ 32,
      /* .MaxTextureCoords = */ 32,
      /* .MaxVertexAttribs = */ 64,
      /* .MaxVertexUniformComponents = */ 4096,
      /* .MaxVaryingFloats = */ 64,
      /* .MaxVertexTextureImageUnits = */ 32,
      /* .MaxCombinedTextureImageUnits = */ 80,
      /* .MaxTextureImageUnits = */ 32,
      /* .MaxFragmentUniformComponents = */ 4096,
      /* .MaxDrawBuffers = */ 32,
      /* .MaxVertexUniformVectors = */ 128,
      /* .MaxVaryingVectors = */ 8,
      /* .MaxFragmentUniformVectors = */ 16,
      /* .MaxVertexOutputVectors = */ 16,
      /* .MaxFragmentInputVectors = */ 15,
      /* .MinProgramTexelOffset = */ -8,
      /* .MaxProgramTexelOffset = */ 7,
      /* .MaxClipDistances = */ 8,
      /* .MaxComputeWorkGroupCountX = */ 65535,
      /* .MaxComputeWorkGroupCountY = */ 65535,
      /* .MaxComputeWorkGroupCountZ = */ 65535,
      /* .MaxComputeWorkGroupSizeX = */ 1024,
      /* .MaxComputeWorkGroupSizeY = */ 1024,
      /* .MaxComputeWorkGroupSizeZ = */ 64,
      /* .MaxComputeUniformComponents = */ 1024,
      /* .MaxComputeTextureImageUnits = */ 16,
      /* .MaxComputeImageUniforms = */ 8,
      /* .MaxComputeAtomicCounters = */ 8,
      /* .MaxComputeAtomicCounterBuffers = */ 1,
      /* .MaxVaryingComponents = */ 60,
      /* .MaxVertexOutputComponents = */ 64,
      /* .MaxGeometryInputComponents = */ 64,
      /* .MaxGeometryOutputComponents = */ 128,
      /* .MaxFragmentInputComponents = */ 128,
      /* .MaxImageUnits = */ 8,
      /* .MaxCombinedImageUnitsAndFragmentOutputs = */ 8,
      /* .MaxCombinedShaderOutputResources = */ 8,
      /* .MaxImageSamples = */ 0,
      /* .MaxVertexImageUniforms = */ 0,
      /* .MaxTessControlImageUniforms = */ 0,
      /* .MaxTessEvaluationImageUniforms = */ 0,
      /* .MaxGeometryImageUniforms = */ 0,
      /* .MaxFragmentImageUniforms = */ 8,
      /* .MaxCombinedImageUniforms = */ 8,
      /* .MaxGeometryTextureImageUnits = */ 16,
      /* .MaxGeometryOutputVertices = */ 256,
      /* .MaxGeometryTotalOutputComponents = */ 1024,
      /* .MaxGeometryUniformComponents = */ 1024,
      /* .MaxGeometryVaryingComponents = */ 64,
      /* .MaxTessControlInputComponents = */ 128,
      /* .MaxTessControlOutputComponents = */ 128,
      /* .MaxTessControlTextureImageUnits = */ 16,
      /* .MaxTessControlUniformComponents = */ 1024,
      /* .MaxTessControlTotalOutputComponents = */ 4096,
      /* .MaxTessEvaluationInputComponents = */ 128,
      /* .MaxTessEvaluationOutputComponents = */ 128,
      /* .MaxTessEvaluationTextureImageUnits = */ 16,
      /* .MaxTessEvaluationUniformComponents = */ 1024,
      /* .MaxTessPatchComponents = */ 120,
      /* .MaxPatchVertices = */ 32,
      /* .MaxTessGenLevel = */ 64,
      /* .MaxViewports = */ 16,
      /* .MaxVertexAtomicCounters = */ 0,
      /* .MaxTessControlAtomicCounters = */ 0,
      /* .MaxTessEvaluationAtomicCounters = */ 0,
      /* .MaxGeometryAtomicCounters = */ 0,
      /* .MaxFragmentAtomicCounters = */ 8,
      /* .MaxCombinedAtomicCounters = */ 8,
      /* .MaxAtomicCounterBindings = */ 1,
      /* .MaxVertexAtomicCounterBuffers = */ 0,
      /* .MaxTessControlAtomicCounterBuffers = */ 0,
      /* .MaxTessEvaluationAtomicCounterBuffers = */ 0,
      /* .MaxGeometryAtomicCounterBuffers = */ 0,
      /* .MaxFragmentAtomicCounterBuffers = */ 1,
      /* .MaxCombinedAtomicCounterBuffers = */ 1,
      /* .MaxAtomicCounterBufferSize = */ 16384,
      /* .MaxTransformFeedbackBuffers = */ 4,
      /* .MaxTransformFeedbackInterleavedComponents = */ 64,
      /* .MaxCullDistances = */ 8,
      /* .MaxCombinedClipAndCullDistances = */ 8,
      /* .MaxSamples = */ 4,
      /* .maxMeshOutputVerticesNV = */ 256,
      /* .maxMeshOutputPrimitivesNV = */ 512,
      /* .maxMeshWorkGroupSizeX_NV = */ 32,
      /* .maxMeshWorkGroupSizeY_NV = */ 1,
      /* .maxMeshWorkGroupSizeZ_NV = */ 1,
      /* .maxTaskWorkGroupSizeX_NV = */ 32,
      /* .maxTaskWorkGroupSizeY_NV = */ 1,
      /* .maxTaskWorkGroupSizeZ_NV = */ 1,
      /* .maxMeshViewCountNV = */ 4,  /* .limits = */ 
      /* .nonInductiveForLoops = */ 1,
      /* .whileLoops = */ 1,
      /* .doWhileLoops = */ 1,
      /* .generalUniformIndexing = */ 1,
      /* .generalAttributeMatrixVectorIndexing = */ 1,
      /* .generalVaryingIndexing = */ 1,
      /* .generalSamplerIndexing = */ 1,
      /* .generalVariableIndexing = */ 1,
      /* .generalConstantMatrixVectorIndexing = */ 1,
    } ;
  
    /**
     * @param eshlang 
     * @param stage 
     */
    void operator<<( EShLanguage& eshlang, const ::tools::shader::ShaderStage& stage ) ;
  
    /**
     * @param str 
     * @param stage 
     */
    void operator<<( std::string& str, const ::tools::shader::ShaderStage& stage ) ;

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
      else std::cout << "Unknown type : " << type_name << std::endl ; exit( -1 ) ;
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
      ShaderMap::iterator it ;
    };
  
    /**
     */
    struct UWUShaderData
    {
        std::string include_directory ;
        ShaderMap   map               ;
        
      /**
       * @param data 
       * @param type 
       */
      void loadShader( const char* data, ::tools::shader::ShaderStage type ) ;
      
      /**
       * @param data
       * @param stage
       */
      void parseAttributes( const char* data, ::tools::shader::ShaderStage stage ) ;

      /**
       * @param map 
       * @param program 
       * @param stage_flag 
       */
      void generateDescriptorSetBindings( Shader& map, glslang::TProgram& program ) ;

      /**
       * @param stream 
       * @param str 
       */
      void writeString( std::ofstream& stream, std::string str ) const ;

      /**
       * @param stream 
       * @param num 
       */
      void writeUnsigned( std::ofstream& stream, unsigned num ) const ;

      /**
       * @param stream 
       * @param val 
       */
      void writeBoolean( std::ofstream& stream, bool val ) const ;

      /**
       * @param stream 
       * @param magic 
       */
      void writeMagic( std::ofstream& stream, unsigned long long magic ) const ;

      /**
       * @param stream 
       * @param sz 
       * @param spirv 
       */
      void writeSpirv( std::ofstream& stream, unsigned sz, const unsigned* spirv ) const ;

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
  
    void operator<<( EShLanguage& eshlang, const ::tools::shader::ShaderStage& stage )
    {
      switch( stage )
      {
        case ShaderStage::VERTEX        : eshlang = EShLangVertex         ; break ; 
        case ShaderStage::COMPUTE       : eshlang = EShLangCompute        ; break ; 
        case ShaderStage::FRAGMENT      : eshlang = EShLangFragment       ; break ; 
        case ShaderStage::GEOMETRY      : eshlang = EShLangGeometry       ; break ; 
        case ShaderStage::TESSALATION_C : eshlang = EShLangTessControl    ; break ; 
        case ShaderStage::TESSELATION_E : eshlang = EShLangTessEvaluation ; break ; 
        default                         : eshlang = EShLangCount          ; break ;
      }
    }
  
    void operator<<( std::string& str, const ::tools::shader::ShaderStage& stage )
    {
      switch( stage )
      {
        case ShaderStage::VERTEX        : str = "Vertex"               ; break ; 
        case ShaderStage::COMPUTE       : str = "Compute"              ; break ; 
        case ShaderStage::FRAGMENT      : str = "Fragment"             ; break ; 
        case ShaderStage::GEOMETRY      : str = "Geometry"             ; break ; 
        case ShaderStage::TESSALATION_C : str = "Tessalation_Control"  ; break ; 
        case ShaderStage::TESSELATION_E : str = "Tesselation_Evaluate" ; break ; 
        default                         : str = ""                     ; break ;
      }
    }
    
    void UWUShaderData::parseAttributes( const char* data, ::tools::shader::ShaderStage stage )
    {
      auto shader = this->map.find( stage ) ;
      
      std::vector<std::string> history    ;
      std::stringstream        file_data  ;
      std::string              token      ;
      Attribute                attr       ;
      unsigned                 position   ;
      unsigned                 location   ;
      bool                     valid      ;
      
      position = 0 ;
      history.resize( 3 ) ;
      if( shader != this->map.end() )
      {
        file_data << data ;
      
        while( !file_data.eof() )
        {
          file_data >> token ; // Get Type.
          
          if( token == "in"  )
          {
            for( unsigned i = 0; i < history.size(); i++ )
            {
              auto found = history[ i ].find( "location" ) ;
              
              if( history[ i ] == "=" )
              {
                location = i + 1 == history.size() ? std::atoi( history[ 0 ].c_str() ) : std::atoi( history[ i + 1 ].c_str() ) ;
              }
              if( ( found ) != std::string::npos ) valid = true ;
            }

            /// Get type information.
            file_data >> token                                     ;
            attr.type     = token                                  ; 
            attr.size     = ::tools::shader::sizeFromType( token ) ;
            attr.input    = true                                   ;
            attr.location = location                               ;
            
            file_data >> token ;
            attr.name  = token ;
            
            shader->second.attributes.push_back( attr ) ;
          }
          else if( token == "out" )
          {            
            for( unsigned i = 0; i < history.size(); i++ )
            {
              auto found = history[ i ].find( "location" ) ;
              
              if( history[ i ] == "=" ) 
              {
                location = i + 1 == history.size() ? std::atoi( history[ 0 ].c_str() ) : std::atoi( history[ i + 1 ].c_str() ) ;
              }
              if( ( found ) != std::string::npos ) valid = true ;
            }

            /// Get type information.
            file_data >> token                                     ;
            attr.type     = token                                  ; 
            attr.size     = ::tools::shader::sizeFromType( token ) ;
            attr.input    = false                                  ;
            attr.location = location                               ;

            file_data >> token ;
            attr.name  = token ;
            
            shader->second.attributes.push_back( attr ) ;
          }
          valid               = false ;
          history[ position ] = token ;
          position = ( position + 1 ) % history.size() ;
        }
      }
    }

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

    void UWUShaderData::writeString( std::ofstream& stream, std::string val ) const
    {
      unsigned sz ;

      sz = val.size() ;
      stream.write( (char*)&sz, sizeof( unsigned ) ) ;
      stream.write( (char*)&val[0], sz             ) ;
    }
    
    void UWUShaderData::writeMagic( std::ofstream& stream, unsigned long long val ) const
    {
      stream.write( (char*)&val, sizeof( unsigned long long ) ) ;
    }

    void UWUShaderData::writeUnsigned( std::ofstream& stream, unsigned val ) const
    {
      stream.write( (char*)&val, sizeof( unsigned ) ) ;
    }

    void UWUShaderData::writeBoolean( std::ofstream& stream, bool val ) const
    {
      stream.write( (char*)&val, sizeof( bool ) ) ;
    }

    void UWUShaderData::writeSpirv( std::ofstream& stream, unsigned sz, const unsigned* spirv ) const
    {
      stream.write( (char*)spirv, sz * sizeof( unsigned ) ) ;
    }

    void UWUShaderData::generateDescriptorSetBindings( Shader& shader, glslang::TProgram& program )
    {
      int         sz      ; 
      std::string name    ; 
      Uniform     uniform ;
  
      for( unsigned i = 0; i < program.getNumUniformVariables(); i++ )
      {
        name = std::string( program.getUniformTType( i )->getCompleteString() ) ;
        if( name.find( "sampler2D") != std::string::npos )
        {
          uniform.name    = program.getUniformName( i )    ;
          uniform.binding = program.getUniformBinding( i ) ;
          uniform.size    = 1                              ;
          uniform.type    = UniformType::SAMPLER           ;
  
          shader.uniforms.push_back( uniform ) ;
        }
      }
  
      for( unsigned i = 0; i < program.getNumUniformBlocks(); i++ )
      {
        sz   = program.getUniformArraySize( i ) ;
        name = program.getUniformBlockName( i ) ;
  
        uniform.name    = name                                ;
        uniform.binding = program.getUniformBlockBinding( i ) ;
        uniform.size    = program.getUniformArraySize( i )    ;
        uniform.type    = UniformType::UBO                    ;
  
        shader.uniforms.push_back( uniform ) ;
      }
    }
  
    void UWUShaderData::loadShader( const char* data, ::tools::shader::ShaderStage type )
    {
      static bool glslang_initialized = false ;
  
      const int default_version = 100 ;
      const int input_version   = 100 ;
  
      Shader                            shader         ;
      glslang::EshTargetClientVersion   vulkan_version ;
      glslang::TProgram                 program        ;
      glslang::EShTargetLanguageVersion glsl_version   ;
      glslang::SpvOptions               options        ;
      spv::SpvBuildLogger               logger         ;
      std::string                       glsl_code      ;
      std::string                       stage_name     ;
      std::string                       pre_processed  ;
      EShLanguage                       lang_type      ;
      TBuiltInResource                  resources      ;
      EShMessages                       messages       ;
      DirStackFileIncluder              includer       ;
  
      if( !glslang_initialized )
      {
        glslang::InitializeProcess() ;
        glslang_initialized = true ;
      }
       
      lang_type  << type ;
      stage_name << type ;
      
      resources      = DefaultTBuiltInResource                                        ;
      vulkan_version = glslang::EShTargetVulkan_1_2                                   ;
      glsl_version   = glslang::EShTargetSpv_1_0                                      ;
      messages       = static_cast<EShMessages>( EShMsgSpvRules | EShMsgVulkanRules ) ;

      // Why is there not default constructor for a TShader???? I cannot declare, then initialize. Must do both here. Ugh.
      glslang::TShader glslang_shader( lang_type ) ;
      glslang_shader.setEnvInput ( glslang::EShSourceGlsl, lang_type, glslang::EShClientVulkan, input_version ) ;
      glslang_shader.setEnvClient( glslang::EShClientVulkan, vulkan_version                                   ) ;
      glslang_shader.setEnvTarget( glslang::EshTargetSpv, glsl_version                                        ) ;
      glslang_shader.setStrings  ( &data, 1                                                                   ) ;
      includer.pushExternalLocalDirectory( this->include_directory.c_str() ) ;
  
      if( !glslang_shader.preprocess( &resources, default_version, ENoProfile, false, false, messages, &pre_processed, includer ) )
      {
        std::cout << "GLSL Preprocessing Failed for: " << stage_name << std::endl ;
        std::cout << glslang_shader.getInfoLog()                     << std::endl ;
        std::cout << glslang_shader.getInfoDebugLog()                << std::endl ;
      }

      // For some reason i have to do this as well. I cannot just use the c_str() directly.
      const char* preprocc = pre_processed.c_str() ;
      glslang_shader.setStrings( &preprocc, 1 ) ;
  
      if( !glslang_shader.parse( &resources, default_version, false, messages ) )
      {
        std::cout << "GLSL Parsing Failed for: " << stage_name << std::endl ;
        std::cout << glslang_shader.getInfoLog()               << std::endl ;
        std::cout << glslang_shader.getInfoDebugLog()          << std::endl ;
      }
  
      program.addShader( &glslang_shader ) ;
      if( !program.link( messages        ) )
      {
        std::cout << "GLSL Program Linking Failed for: " << stage_name << std::endl ;
        std::cout << glslang_shader.getInfoLog()                       << std::endl ;
        std::cout << glslang_shader.getInfoDebugLog()                  << std::endl ;
      }
      
      glslang::GlslangToSpv( *program.getIntermediate( lang_type ), shader.spirv, &logger, &options ) ;
  
      program.buildReflection() ;
      shader.stage = type ;
      this->generateDescriptorSetBindings( shader, program ) ;
      this->map.insert( { type, shader } ) ;
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
      unsigned*                  spirv   ;
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

    void UWUShader::save( const char* path )
    {
      std::ofstream stream ;

      stream.open( path ) ;

      if( stream, std::ios::binary )
      {
        data().writeMagic   ( stream, ::tools::shader::MAGIC ) ;
        data().writeUnsigned( stream, size()                 ) ;

        for( auto it = begin(); it != end(); ++it )
        {
          data().writeUnsigned( stream, it.spirvSize()             ) ; // Size of SPIRV code.
          data().writeSpirv   ( stream, it.spirvSize(), it.spirv() ) ; // SPIRV Code.
          
          data().writeUnsigned( stream, it.stage()         ) ; // Shader stage.
          data().writeUnsigned( stream, it.numUniforms()   ) ; // Number of Uniforms.
          data().writeUnsigned( stream, it.numAttributes() ) ; // Number of Attributes.
          for( unsigned index = 0; index < it.numUniforms(); index++ )
          {
            data().writeString  ( stream, it.uniformName   ( index ) ) ; // Uniform Name.
            data().writeUnsigned( stream, it.uniformType   ( index ) ) ; // Uniform Type.
            data().writeUnsigned( stream, it.uniformBinding( index ) ) ; // Uniform Binding.
            data().writeUnsigned( stream, it.uniformSize   ( index ) ) ; // Uniform Binding.
          }
          
          for( unsigned index = 0; index < it.numAttributes(); index ++ )
          {
            data().writeString  ( stream, it.attributeName    ( index ) ) ;
            data().writeString  ( stream, it.attributeType    ( index ) ) ;
            data().writeUnsigned( stream, it.attributeByteSize    ( index ) ) ;
            data().writeUnsigned( stream, it.attributeLocation( index ) ) ;
            data().writeBoolean ( stream, it.attributeIsInput ( index ) ) ;
          }
        }
      }
    }

    void UWUShader::setIncludeDirectory( const char* include_directory )
    {
      data().include_directory = include_directory ;
    }

    ShaderIterator UWUShader::begin()
    {
      ShaderIterator it ;
      it.data().it = data().map.begin() ;

      return it ;
    }

    ShaderIterator UWUShader::end()
    {
      ShaderIterator it ;
      it.data().it = data().map.end() ;

      return it ;
    }

    unsigned UWUShader::size() const
    {
      return data().map.size() ;
    }

    void UWUShader::compile( ShaderStage stage, const char* shader_data )
    {
      this->data().loadShader     ( shader_data, stage ) ;
      this->data().parseAttributes( shader_data, stage ) ;
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