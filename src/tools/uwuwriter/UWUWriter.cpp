#include "UWUWriter.h"
#include <glslang/Public/ShaderLang.h>
#include <glslang/SPIRV/GlslangToSpv.h>
#include <glslang/StandAlone/DirStackFileIncluder.h>
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
  struct Shader ;
  typedef std::map<ShaderStage, Shader> ShaderMap ;

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
  void operator<<( EShLanguage& eshlang, const ShaderStage& stage ) ;

  /**
   * @param str 
   * @param stage 
   */
  void operator<<( std::string& str, const ShaderStage& stage ) ;

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
    else if( type_name == "uint"     ) return sizeof( unsigned )      ;
    else if( type_name == "float"    ) return sizeof( float    )      ;
    else if( type_name == "int"      ) return sizeof( int      )      ;
    else if( type_name == "bool"     ) return sizeof( bool     )      ;
    
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
  struct UWUWriterData
  {
    std::string include_directory ;
    ShaderMap   map               ;

    /**
     * @param data
     * @param type 
     */
    void loadShader( const char* data, ShaderStage type ) ;

    /**
     * @param data
     * @param stage
     */
    void parseAttributes( const char* data, ShaderStage stage ) ;

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
  };

  void operator<<( EShLanguage& eshlang, const ShaderStage& stage )
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

  void operator<<( std::string& str, const ShaderStage& stage )
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

  void UWUWriterData::parseAttributes( const char* data, ShaderStage stage )
  {
    auto shader = this->map.find( stage ) ;

    std::vector<std::string> history    ;
    std::stringstream        file_data  ;
    std::stringstream        line_data  ;
    std::string              token      ;
    std::string              line       ;
    Attribute                attr       ;
    unsigned                 position   ;
    unsigned                 location   ;
    
    location = 0 ;
    position = 0 ;
    history.resize( 3 ) ;
    if( shader != this->map.end() )
    {
      // Push all input into stream.
      line_data << data ;

      // While theres data to be parsed.
      while( std::getline( line_data, line ) )
      {

        // Push a single line of code into the stream.

        file_data.str("") ;
        file_data.clear() ;
        file_data << line ;

        // Look for inputs.
        while( !file_data.eof() )
        {
          token = "" ;
          file_data >> token ; // Get Type.

          // If we're a comment then just break.
          if( token.find( "//") != std::string::npos ) break ;

          if( token == "in"  )
          {
            for( unsigned i = 0; i < history.size(); i++ )
            {
              if( history[ i ] == "=" )
              {
                location = i + 1 == history.size() ? std::atoi( history[ 0 ].c_str() ) : std::atoi( history[ i + 1 ].c_str() ) ;
              }
            }

            /// Get type information.
            file_data >> token                    ;
            attr.type     = token                 ; 
            attr.size     = sizeFromType( token ) ;
            attr.input    = true                  ;
            attr.location = location              ;

            file_data >> token ;
            attr.name  = token ;

            shader->second.attributes.push_back( attr ) ;
          }
          else if( token == "out" )
          {            
            for( unsigned i = 0; i < history.size(); i++ )
            {
              if( history[ i ] == "=" ) 
              {
                location = i + 1 == history.size() ? std::atoi( history[ 0 ].c_str() ) : std::atoi( history[ i + 1 ].c_str() ) ;
              }
            }
            line = "" ;
            /// Get type information.
            file_data >> token                    ;
            attr.type     = token                 ; 
            attr.size     = sizeFromType( token ) ;
            attr.input    = false                 ;
            attr.location = location              ;

            file_data >> token ;
            attr.name  = token ;

            shader->second.attributes.push_back( attr ) ;
          }
          history[ position ] = token ;
          position = ( position + 1 ) % history.size() ;
        }
      }
    }
  }

  void UWUWriterData::writeString( std::ofstream& stream, std::string val ) const
  {
    unsigned sz ;

    sz = val.size() ;
    stream.write( (char*)&sz, sizeof( unsigned ) ) ;
    stream.write( (char*)&val[0], sz             ) ;
  }

  void UWUWriterData::writeMagic( std::ofstream& stream, unsigned long long val ) const
  {
    stream.write( (char*)&val, sizeof( unsigned long long ) ) ;
  }

  void UWUWriterData::writeUnsigned( std::ofstream& stream, unsigned val ) const
  {
    stream.write( (char*)&val, sizeof( unsigned ) ) ;
  }

  void UWUWriterData::writeBoolean( std::ofstream& stream, bool val ) const
  {
    stream.write( (char*)&val, sizeof( bool ) ) ;
  }

  void UWUWriterData::writeSpirv( std::ofstream& stream, unsigned sz, const unsigned* spirv ) const
  {
    stream.write( (char*)spirv, sz * sizeof( unsigned ) ) ;
  }

  void UWUWriterData::generateDescriptorSetBindings( Shader& shader, glslang::TProgram& program )
  {
    std::string name    ; 
    Uniform     uniform ;

    for( unsigned i = 0; i < static_cast<unsigned>( program.getNumUniformVariables() ); i++ )
    {
      name = std::string( program.getUniformTType( i )->getCompleteString() ) ;
      if( name.find( "sampler2D" ) != std::string::npos )
      {
        uniform.name    = program.getUniformName( i )    ;
        uniform.binding = program.getUniformBinding( i ) ;
        uniform.size    = 1                              ;
        uniform.type    = UniformType::SAMPLER           ;

        shader.uniforms.push_back( uniform ) ;
      }

      if( name.find( "image" ) != std::string::npos )
      {
        uniform.name    = program.getUniformName( i )    ;
        uniform.binding = program.getUniformBinding( i ) ;
        uniform.size    = 1                              ;
        uniform.type    = UniformType::IMAGE             ;

        shader.uniforms.push_back( uniform ) ;
      }
    }

    for( unsigned i = 0; i < static_cast<unsigned>( program.getNumUniformBlocks() ); i++ )
    {
      std::string complete_string = program.getUniformBlock( i ).getType()->getCompleteString().c_str() ;

      name = program.getUniformBlockName( i ) ;

      uniform.name    = name                                                                                           ;
      uniform.binding = program.getUniformBlockBinding( i )                                                            ;
      uniform.size    = complete_string.find( " buffer " ) != std::string::npos ? 1 : program.getUniformArraySize( i ) ;
      uniform.type    = complete_string.find( " buffer " ) != std::string::npos ? UniformType::SSBO : UniformType::UBO ;
      
      std::cout << name << " BINDING " << uniform.binding << std::endl ;
      if( uniform.binding < INT_MAX ) 
      {
        shader.uniforms.push_back( uniform ) ;
      }
    }
  }

  void UWUWriterData::loadShader( const char* data, ShaderStage type )
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
      std::cout << program.getInfoLog()                       << std::endl ;
      std::cout << program.getInfoDebugLog()                  << std::endl ;
    }

    glslang::GlslangToSpv( *program.getIntermediate( lang_type ), shader.spirv, &logger, &options ) ;

    program.buildReflection() ;
    shader.stage = type ;
    this->generateDescriptorSetBindings( shader, program ) ;
    this->map.insert( { type, shader } ) ;
   }
  
  UWUWriter::UWUWriter()
  {
    this->compiler_data = new UWUWriterData() ;
  }

  UWUWriter::~UWUWriter()
  {
    delete this->compiler_data ;
  }

  void UWUWriter::save( const char* path )
  {
    unsigned  spirv_size     ;
    unsigned* spirv          ;
    unsigned  stage          ;
    unsigned  num_uniforms   ;
    unsigned  num_attributes ;
    
    std::string uniform_name    ;
    unsigned    uniform_type    ;
    unsigned    uniform_binding ;
    unsigned    uniform_size    ;
    
    std::string attribute_name     ;
    std::string attribute_type     ;
    unsigned    attribute_bytesize ;
    unsigned    attribute_location ;
    bool        attribute_input    ;
    
    std::ofstream stream ;

    stream.open( path, std::ios::binary ) ;

    if( stream )
    {
      data().writeMagic   ( stream, MAGIC   ) ;
      data().writeUnsigned( stream, size()  ) ;

      for( auto it = data().map.begin(); it != data().map.end(); ++it )
      {
        spirv_size     = it->second.spirv.size()      ;
        spirv          = it->second.spirv.data()      ;
        stage          = it->second.stage             ;
        num_uniforms   = it->second.uniforms.size()   ;
        num_attributes = it->second.attributes.size() ;

        data().writeUnsigned( stream, spirv_size        ) ; // Size of SPIRV code.
        data().writeSpirv   ( stream, spirv_size, spirv ) ; // SPIRV Code.

        data().writeUnsigned( stream, stage          ) ; // Shader stage.
        data().writeUnsigned( stream, num_uniforms   ) ; // Number of Uniforms.
        data().writeUnsigned( stream, num_attributes ) ; // Number of Attributes.
        for( unsigned index = 0; index < num_uniforms; index++ )
        {
          uniform_name    = it->second.uniforms[ index ].name    ;
          uniform_size    = it->second.uniforms[ index ].size    ;
          uniform_type    = it->second.uniforms[ index ].type    ;
          uniform_binding = it->second.uniforms[ index ].binding ;
          
          data().writeString  ( stream, uniform_name    ) ; // Uniform Name.
          data().writeUnsigned( stream, uniform_type    ) ; // Uniform Type.
          data().writeUnsigned( stream, uniform_binding ) ; // Uniform Binding.
          data().writeUnsigned( stream, uniform_size    ) ; // Uniform Size
        }

        for( unsigned index = 0; index < num_attributes; index ++ )
        {
          attribute_name     = it->second.attributes[ index ].name     ;
          attribute_type     = it->second.attributes[ index ].type     ;
          attribute_bytesize = it->second.attributes[ index ].size     ;
          attribute_location = it->second.attributes[ index ].location ;
          attribute_input    = it->second.attributes[ index ].input    ;

          data().writeString  ( stream, attribute_name     ) ;
          data().writeString  ( stream, attribute_type     ) ;
          data().writeUnsigned( stream, attribute_bytesize ) ;
          data().writeUnsigned( stream, attribute_location ) ;
          data().writeBoolean ( stream, attribute_input    ) ;
        }
      }
    }
  }

  void UWUWriter::setIncludeDirectory( const char* include_directory )
  {
    data().include_directory = include_directory ;
  }

  unsigned UWUWriter::size() const
  {
    return data().map.size() ;
  }

  void UWUWriter::compile( ShaderStage stage, const char* shader_data )
  {
    this->data().loadShader                                         ( shader_data, stage ) ;
    if( stage != ShaderStage::COMPUTE ) this->data().parseAttributes( shader_data, stage ) ;
  }

  UWUWriterData& UWUWriter::data()
  {
    return *this->compiler_data ;
  }

  const UWUWriterData& UWUWriter::data() const
  {
    return *this->compiler_data ;
  }
}

