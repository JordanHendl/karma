#include "casper_shader.h"
#include <glslang/Public/ShaderLang.h>
#include <SPIRV/GlslangToSpv.h>
#include <StandAlone/DirStackFileIncluder.h>
#include <vulkan/vulkan.h>
#include <vector>
#include <fstream>
#include <ostream>
#include <istream>
#include <cerrno>
#include <string> 
#include <iostream>
#include <memory>
#include <map>

namespace casper
{
  namespace tools
  {
    const unsigned long long MAGIC   = 27973171129184778 ;
    const unsigned long long VERSION = 100               ;

    static std::map< std::string, uint32_t > TypeToStageFlag = 
    {
      { "frag", VK_SHADER_STAGE_FRAGMENT_BIT                },
      { "geom", VK_SHADER_STAGE_GEOMETRY_BIT                },
      { "tesc", VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT    },
      { "tess", VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT },
      { "comp", VK_SHADER_STAGE_COMPUTE_BIT                 },
      { "vert", VK_SHADER_STAGE_VERTEX_BIT                  },
    } ;

    const static TBuiltInResource DefaultTBuiltInResource = 
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
    /* .maxMeshViewCountNV = */ 4,

    /* .limits = */ {
        /* .nonInductiveForLoops = */ 1,
        /* .whileLoops = */ 1,
        /* .doWhileLoops = */ 1,
        /* .generalUniformIndexing = */ 1,
        /* .generalAttributeMatrixVectorIndexing = */ 1,
        /* .generalVaryingIndexing = */ 1,
        /* .generalSamplerIndexing = */ 1,
        /* .generalVariableIndexing = */ 1,
        /* .generalConstantMatrixVectorIndexing = */ 1,
    } } ;

    static bool glsl_init = false ;

   void writeString( std::ofstream& stream, std::string out ) ;
   std::string readString( std::ifstream& stream ) ;

    std::string GetSuffix( const std::string& name )
    {
        const size_t pos = name.rfind( '.' ) ;
        return ( pos == std::string::npos ) ? "" : name.substr(name.rfind( '.' ) + 1 ) ;
    }

    std::string GetFilePath(const std::string& str)
    {
    	size_t found = str.find_last_of( "/\\" ) ;
    	return str.substr( 0,found ) ;
    }

    EShLanguage GetShaderStage(const std::string& stage)
    {
      if (stage == "vert")
      {
        return EShLangVertex;
      }
      else if (stage == "tesc")
      {
        return EShLangTessControl;
      }
      else if (stage == "tess")
      {
        return EShLangTessEvaluation;
      }
      else if (stage == "geom")
      {
        return EShLangGeometry;
      }
      else if (stage == "frag")
      {
        return EShLangFragment;
      }
      else if (stage == "comp")
      {
        return EShLangCompute;
      }
      else
      {
        assert(0 && "Unknown shader stage" ) ;
        return EShLangCount;
      }
    }

    std::string loadPath( const char* path )
    {
      std::string   contents ;
      std::ifstream input    ;
      
      input.open( path, std::ios::in ) ;

      if( input )
      {
        std::string InputGLSL((std::istreambuf_iterator<char>(input)),
                        std::istreambuf_iterator<char>());
        // input.seekg( 0, std::ios::end ) ;

        // contents.resize( input.tellg() ) ;

        // input.seekg( 0, std::ios::beg ) ;
        
        // input.read( &contents[0], contents.size() ) ;
        // input.close() ;

        return InputGLSL ;
      }
      throw( errno ) ;
    };
   
   void writeDescriptors( std::map<std::string, VkDescriptorSetLayoutBinding> &map, std::ofstream &stream )
   {
     unsigned sz ;

     sz = map.size() ;

     stream.write( (char*)&sz, sizeof( unsigned ) ) ;

     for( auto b : map )
     {
       writeString( stream, b.first ) ;
       stream.write( (char*)&b.second.binding        , sizeof( uint32_t         ) ) ;
       stream.write( (char*)&b.second.descriptorType , sizeof( VkDescriptorType ) ) ;
       stream.write( (char*)&b.second.stageFlags     , sizeof( uint32_t         ) ) ;
       stream.write( (char*)&b.second.descriptorCount, sizeof( uint32_t         ) ) ;
     }
   }

   void readDescriptors( const char* name, std::map< std::string, VkDescriptorSetLayoutBinding> &name_map, std::ifstream &stream ) 
   {
     VkDescriptorSetLayoutBinding bind         ;
     unsigned                     iterator     ;
     unsigned                     num_elements ;     
     uint32_t                     desc_count   ;
     uint32_t                     flags        ;
     VkDescriptorType             type         ;
     uint32_t                     binding      ;

     iterator = 0 ;

     stream.read( (char*)&num_elements, sizeof( unsigned ) ) ;
     name_map.clear() ;

     for( unsigned i = 0; i < num_elements; i++ )
     {
       std::string desc_name = readString( stream ) ;
       stream.read( (char*)&binding   , sizeof( uint32_t         ) ) ;
       stream.read( (char*)&type      , sizeof( VkDescriptorType ) ) ;
       stream.read( (char*)&flags     , sizeof( uint32_t         ) ) ;
       stream.read( (char*)&desc_count, sizeof( uint32_t         ) ) ;
       
       bind                    = {}         ;
       bind.binding            = binding    ;
       bind.descriptorType     = type       ;
       bind.descriptorCount    = desc_count ;
       bind.stageFlags         = flags      ;
       bind.pImmutableSamplers = nullptr    ;
       
       if( name_map.find( desc_name ) == name_map.end() )
       {
         name_map.insert( { desc_name, bind } ) ;
       }
     }
   }

   void writeString( std::ofstream& stream, std::string out )
   {
     unsigned sz ;

     sz = out.size() + 1;
     stream.write( (char*)&sz, sizeof( unsigned ) ) ;
     stream.write( (char*)out.data(), sz          ) ;
   }

   std::string readString( std::ifstream& stream )
   {
     unsigned    sz  ;
     std::string out ;
     
     stream.read( (char*)&sz, sizeof( unsigned ) ) ;
     out.resize( sz - 1 ) ;
     stream.read( (char*)out.data(), sz ) ;

     return out ;
   }

   void writeVector( std::ofstream& stream, std::vector<unsigned> out )
   {
     unsigned sz ;

     sz = out.size() ;
     stream.write( (char*)&sz, sizeof( unsigned ) ) ;
     stream.write( (char*)out.data(), sz * sizeof( unsigned ) ) ;
   }

   std::vector<unsigned> readVector( std::ifstream& stream )
   {
      unsigned                   sz  ;
      std::vector<unsigned int> out ;
     
     stream.read( (char*)&sz, sizeof( unsigned ) ) ;
     out.resize( sz ) ;
     stream.read( (char*)out.data(), sz * sizeof( unsigned ) ) ;

     return out ;
   }

    struct ShaderData
    {
      typedef std::map<std::string, VkDescriptorSetLayoutBinding> DescriptorNameMap    ;
      typedef std::vector<VkDescriptorSetLayoutBinding>           DescriptorBindings   ;
      typedef std::vector<VkDescriptorSetLayout>                  DescriptorArray      ;

      typedef std::vector<unsigned int>          SpirVData   ;
      typedef std::unique_ptr<glslang::TShader>  pShader     ;
      typedef std::map<std::string, SpirVData>   SpirV       ;
      typedef std::vector<pShader>               Shaders     ;
      typedef std::map<std::string, std::string> ShaderPaths ;

      VkDescriptorSetLayout desc_layout      ; ///< Descriptor Layout for this shader.
      DescriptorNameMap     desc_name_map    ; ///< Map of name to binding description.
      bool                  has_device       ; ///< Whether or not this shader has a device referenced.
      VkDevice              device           ; ///< The device to create uniform descriptions onto.
      ShaderPaths           paths            ; ///< The paths to all GLSL files used in this shader unit.
      Shaders               shaders          ; ///< The GLSLang Shaders used for processing and compiling.
      SpirV                 spirv            ; ///< The compiled SPIRV of this object's combined shaders.

      void generateBindingLayout()
      {
        VkDescriptorSetLayoutCreateInfo           info     ;
        std::vector<VkDescriptorSetLayoutBinding> bindings ;

        for( auto a : this->desc_name_map ) bindings.push_back( a.second ) ;

        info              = {}                                                  ;
        info.sType        = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO ;
        info.bindingCount = 0                                                   ;
        info.pNext        = NULL                                                ;

        info.bindingCount = bindings.size() ;
        info.pBindings    = bindings.data() ;
        info.pNext        = NULL            ;

        if( this->has_device )
        {
          if( vkCreateDescriptorSetLayout(  this->device, &info, nullptr, &this->desc_layout ) != VK_SUCCESS )
          {
            std::cout << " Failed to create descriptor set layout for shader stage " << std::endl ;
            throw errno ;
          }
        }
        
      }

      ShaderData()
      {
        this->has_device = false ;
      }
    };

    Shader::Shader()
    {
      this->shader_data = new ShaderData() ;
    }

    Shader::~Shader()
    {
      delete this->shader_data ;
    }

    void Shader::loadShader( const char* path, const char* type )
    {
      const int default_version = 100 ;
      const int input_version   = 100 ;

      std::vector<unsigned int>         spirv          ;
      glslang::EshTargetClientVersion   vulkan_version ;
      glslang::TProgram                 program        ;
      glslang::EShTargetLanguageVersion glsl_version   ;
      glslang::SpvOptions               options        ;
      spv::SpvBuildLogger               logger         ;
      std::string                       glsl_code      ;
      std::string                       pre_vertex     ;
      std::string                       relative_path  ;
      EShLanguage                       lang_type      ;
      TBuiltInResource                  resources      ;
      EShMessages                       messages       ;
      DirStackFileIncluder              includer       ;

      if( !glsl_init )
      {
        glslang::InitializeProcess() ;
        glsl_init = true ;
      }

      glsl_code   = loadPath      ( path                             ) ;
      lang_type   = GetShaderStage( GetSuffix( std::string( path ) ) ) ;
      glslang::TShader shader( lang_type ) ;


      const char* vertbuff = glsl_code.c_str() ;

      shader.setStrings( &vertbuff, 1 ) ;


      vulkan_version = glslang::EShTargetVulkan_1_2 ;
      glsl_version   = glslang::EShTargetSpv_1_0 ;

      shader.setEnvInput ( glslang::EShSourceGlsl, lang_type, glslang::EShClientVulkan, input_version ) ;
      shader.setEnvClient( glslang::EShClientVulkan, vulkan_version                                   ) ;
      shader.setEnvTarget( glslang::EshTargetSpv, glsl_version                                        ) ;

      resources = DefaultTBuiltInResource ;

      messages = static_cast<EShMessages>( EShMsgSpvRules | EShMsgVulkanRules ) ;
      
      relative_path = GetFilePath( std::string( path ) ) ;
      includer.pushExternalLocalDirectory( relative_path ) ;

      if( !shader.preprocess( &resources, default_version, ENoProfile, false, false, messages, &pre_vertex, includer ) )
      {
        std::cout << "GLSL Preprocessing Failed for: " << path << std::endl ;
        std::cout << shader.getInfoLog()                       << std::endl ;
        std::cout << shader.getInfoDebugLog()                  << std::endl ;
      }

      const char* preprocc = pre_vertex.c_str() ;
      shader.setStrings( &preprocc, 1 ) ;

      if( !shader.parse( &resources, default_version, false, messages ) )
      {
        std::cout << "GLSL Parsing Failed for: " << path << std::endl ;
        std::cout << shader.getInfoLog()                 << std::endl ;
        std::cout << shader.getInfoDebugLog()            << std::endl ;
      }

      program.addShader( &shader ) ;
      if( !program.link( messages ) )
      {
        std::cout << "GLSL Program Linking Failed for: " << path << std::endl ;
        std::cout << shader.getInfoLog()                         << std::endl ;
        std::cout << shader.getInfoDebugLog()                    << std::endl ;
      }
      
      glslang::GlslangToSpv( *program.getIntermediate( lang_type ), spirv, &logger, &options ) ;

      program.buildReflection() ;

      generateDescriptorSetBindings( type, program, TypeToStageFlag.at( type ) ) ;

      data().spirv.insert( { type, spirv } ) ;
   }

    void Shader::initialize( const VkDevice& device )
    {
      data().has_device = true   ;
      data().device     = device ;

      data().desc_name_map.clear() ;
      
      for( auto path : data().paths )
      {
        this->loadShader( path.second.c_str(), path.first.c_str() ) ;
      }

      data().generateBindingLayout() ;
    }

    void Shader::initialize()
    {
      data().has_device = false  ;

      data().desc_name_map.clear() ;

      for( auto path : data().paths )
      {
        this->loadShader( path.second.c_str(), path.first.c_str() ) ;
      }

      data().generateBindingLayout() ;
    }

    void Shader::addShader( const char* path )
    {
      data().paths.insert( {GetSuffix( std::string( path ) ), std::string( path ) } ) ;
    }

    void Shader::write( const char* path )
    {
      std::ofstream output          ;
      unsigned      num_elements    ;
      unsigned      num_descriptors ;

      output.open( path, std::ios::out | std::ios::binary ) ;


      if( output )
      {
        num_elements = data().spirv.size() ;


        output.write( (char*)&MAGIC       ,  sizeof( unsigned long long ) ) ;
        output.write( (char*)&VERSION     ,  sizeof( unsigned long long ) ) ;
        output.write( (char*)&num_elements,  sizeof( unsigned           ) ) ;

        writeDescriptors( data().desc_name_map, output ) ;

        for( auto path : data().spirv )
        {
          writeString( output, path.first  ) ;
          writeVector( output, path.second ) ;
        }


        output.close() ;
      }
    }

    const VkDescriptorSetLayout* Shader::descriptors() const
    {
      return &data().desc_layout ;
    }

    unsigned Shader::numDescriptors() const
    {
      return 1 ;
    }

    void Shader::read( const VkDevice &device, const char* path )
    {
      std::ifstream         input          ;
      std::string           str            ;
      std::vector<unsigned> spirv_data     ;
      unsigned              num_elements   ;
      unsigned long long    magic_number   ;
      unsigned long long    version_number ;

      data().device     = device ;
      data().has_device = true   ;

      data().desc_name_map   .clear() ;
      input.open( path, std::ios::out | std::ios::binary ) ;

      if( input )
      {
        data().spirv.clear() ;

        input.read( (char*)&magic_number  , sizeof( unsigned long long ) ) ;
        input.read( (char*)&version_number, sizeof( unsigned long long ) ) ;
        input.read( (char*)&num_elements  , sizeof( unsigned           ) ) ;

        if( magic_number != MAGIC ) return ;

        readDescriptors( str.c_str(), data().desc_name_map, input ) ;
        
        for( unsigned i = 0; i < num_elements; i++ )
        {
          str        = readString( input ) ;
          spirv_data = readVector( input ) ;

          data().spirv.insert( { str, spirv_data } ) ;
        }

        data().generateBindingLayout() ;

        input.close() ;
      }
      else
      {
        throw std::runtime_error( std::string( "Could not load shader" ) + path ) ;
      }
    }

   
   void Shader::generateDescriptorSetBindings( const char* name, glslang::TProgram& program, uint32_t stage_flag )
   {
     VkDescriptorSetLayout           layout        ;
     VkDescriptorSetLayoutCreateInfo layout_info   ;
     VkDescriptorSetLayoutBinding    binding       ;
     bool                            found_binding ;
     

    //  for( unsigned i = 0; i < program.getNumLiveAttributes(); i++ )
    //  {
    //    auto ttype = program.getAttributeType( i ) ;
       
    //    printf( "%d ", program.getAttributeTType( i )->isVector() ) ;
    //  }

     for( unsigned i = 0; i < program.getNumUniformVariables(); i++ )
     {

       std::string type = std::string( program.getUniformTType( i )->getCompleteString() ) ;
       if( type.find( "sampler2D") != std::string::npos )
       {
         
         if( data().desc_name_map.find( program.getUniformName( i ) ) == data().desc_name_map.end() )
         {
            data().desc_name_map[ program.getUniformName( i ) ] = {} ;
         }
         data().desc_name_map[ program.getUniformName( i ) ].binding            = program.getUniformBinding( i ) ;
         data().desc_name_map[ program.getUniformName( i ) ].descriptorCount    = 1 ;
         data().desc_name_map[ program.getUniformName( i ) ].descriptorType     = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER ;
         data().desc_name_map[ program.getUniformName( i ) ].pImmutableSamplers = NULL ;
         data().desc_name_map[ program.getUniformName( i ) ].stageFlags         |= stage_flag ;

         std::cout << name << " SAMPLER NAME: " << program.getUniformName( i ) << std::endl ;
       }
     }

     for( unsigned i = 0; i < program.getNumUniformBlocks(); i++ )
     {
       int sz                 = program.getUniformArraySize( i ) ;
       std::string block_name = program.getUniformBlockName( i ) ;

       if( data().desc_name_map.find( block_name ) == data().desc_name_map.end() ) data().desc_name_map[ block_name ] = {} ;

       data().desc_name_map[ block_name ].binding            = program.getUniformBlockBinding( i ) ;
       data().desc_name_map[ block_name ].descriptorCount    = program.getUniformArraySize( i )    ;
       data().desc_name_map[ block_name ].descriptorType     = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER   ;
       data().desc_name_map[ block_name ].pImmutableSamplers = NULL                                ;
       data().desc_name_map[ block_name ].stageFlags         |= stage_flag                         ;

       std::cout << name << " UNIFORM BLOCK NAME: " << block_name << std::endl ;
     }
   }

    ShaderIterator Shader::begin()
    {
      return data().spirv.begin() ;
    }

    ShaderIterator Shader::end()
    {
      return data().spirv.end() ;
    }

    ShaderData& Shader::data()
    {
      return *this->shader_data ;
    }

    const ShaderData& Shader::data() const
    {
      return *this->shader_data ;
    }
  }
}