#include "casper_vulkan_pipeline_configuration.h"
#include "Swapchain.h"
#include "Window.h"
#include <Bus.h>
#include <Signal.h>
#include <string>

namespace casper
{
  namespace vulkan
  {
    
    void operator<<( VkPolygonMode& op, const char* str )
    {
      
    }

    void operator<<( VkSampleCountFlagBits& op, const char* str )
    {
      
    }

    void operator<<( VkBlendOp& op, const char* str )
    {
      
    }

    void operator<<( VkBlendFactor& op, const char* str )
    {
      
    }
//
//    void operator<<( VkPipelineColorBlendStateCreateFlags& op, const char* str )
//    {
//      
//    }

    void operator<<( VkLogicOp& op, const char* str )
    {
      
    }
    
    /**
     */
    struct ColorBlendCreateInfoConfiguration
    {
      VkPipelineColorBlendStateCreateInfo color_blend ;
      
      /**
       * @param param
       */
      void setColorBlendLogicOperationEnable( bool param ) ;
      
      /**
       * @param param
       */
      void setColorBlendingLogicalOperation( const char* param ) ;
      
      /**
       * @param id
       * @param param
       */
      void setColorBlendConstant( unsigned id, float param ) ;
      
      /**
       * @param param
       */
      void setColorBlendFlags( const char* param ) ;
    };

    struct ColorBlendAttachmentConfiguration
    {
      VkPipelineColorBlendAttachmentState color_blend ;
      
      /**
       * @param param
       */
      void setColorBlendChannels( const char* param ) ;
      
      /**
       * @param param
       */
      void setColorBlendEnable( bool param ) ;
      
      /**
       * @param param
       */
      void setColorBlendSourceFactor( const char* param ) ;
      
      /**
       * @param param
       */
      void setColorBlendDistanceFactor( const char* param ) ;
      
      /**
       * @param param
       */
      void setColorBlendOperation( const char* param ) ;
      
      /**
       * @param param
       */
      void setColorBlendSourceAlphaFactor( const char* param ) ;
      
      /**
       * @param param
       */
      void setColorBlendDistanceAlphaFactor( const char* param ) ;
      
      /**
       * @param param
       */
      void setColorBlendAlphaOperation( const char* param ) ;
    };

    struct AssemblyConfiguration
    {
      VkPipelineInputAssemblyStateCreateInfo assembly_info ;
      
      /**
       * @param param
       */
      void setAssemblyTopology( const char* param ) ;
      
      /**
       * @param param
       */
      void setAssemblyPrimitiveRestartEnable( bool param ) ;
    };
    
    /**
     */
    struct ScissorConfiguration
    {
      VkRect2D scissor ;
      
      /**
       * @param param
       */
      void setScissorOffsetX( float param ) ;
      
      /**
       * @param param
       */
      void setScissorOffsetY( float param ) ;
    };
    
    /**
     */
    struct ViewportConfiguration
    {
      VkViewport viewport ;
      
      /**
       * @param param
       */
      void setViewportX( float param ) ;
      
      /**
       * @param param
       */
      void setViewportY( float param ) ;
      
      /**
       * @param param
       */
      void setViewportMinDepth( float param ) ;
      
      /**
       * @param param
       */
      void setViewportMaxDepth( float param ) ;
    };
    
    /**
     */
    struct MultisamplerConfiguration
    {
      VkPipelineMultisampleStateCreateInfo multisample_info ;
      
      /**
       * @param param
       */
      void setSamplerShadingEnable( bool param ) ;
      
      /**
       * @param param
       */
      void setSamplerMinSampleShading( float param ) ;
      
      /**
       * @param param
       */
      void setSamplerAlphaToOneEnable( bool param ) ;
      
      /**
       * @param param
       */
      void setSamplerAlphaToCoverageEnable( bool param ) ;
      
      /**
       * @param param
       */
      void setSamplerRasterizationSamples( const char* param ) ;
    };
    
    /**
     */
    struct RasterizerConfiguration
    {
      VkPipelineRasterizationStateCreateInfo rasterizer_info  ;

      /**
       * @param param
       */
      void setRasterizerDepthClampEnable( bool clamp ) ;
      
      /**
       * @param param
       */
      void setRasertizerDiscardEnable( bool param ) ;
      
      /**
       * @param param
       */
      void setRasertizerPolygonMode( const char* param ) ;
      
      /**
       * @param param
       */
      void setRasterizerLineWidth( float param ) ;
      
      /**
       * @param param
       */
      void setRasterizerCullMode( const char* param ) ;
      
      /**
       * @param param
       */
      void setRasterizerFrontFace( const char* param ) ;
      
      /**
       * @param param
       */
      void setRasterizerDepthBiasEnable( bool param ) ;
      
      /**
       * @param param
       */
      void setRasterizerDepthBiasConstantFactor( float param ) ;
      
      /**
       * @param param
       */
      void setRasterizerDepthBiasClamp( float param ) ;
      
      /**
       * @param param
       */
      void setRasterizerDepthBiasSlopeFactor( float param ) ;
    };

    struct PipelineConfigurationData
    {
      ColorBlendAttachmentConfiguration color_blend_attachment ; ///< TODO
      ColorBlendCreateInfoConfiguration color_blend_info       ; ///< TODO
      RasterizerConfiguration           rasterization          ; ///< TODO
      MultisamplerConfiguration         multisampler           ; ///< TODO
      ViewportConfiguration             viewport               ; ///< TODO
      AssemblyConfiguration             assembly               ; ///< TODO
      ScissorConfiguration              scissor                ; ///< TODO
      VkPipelineViewportStateCreateInfo viewport_info          ;
      
      std::string                       name             ; ///< TODO
      unsigned                          image_width      ; ///< TODO
      unsigned                          image_height     ; ///< TODO
      unsigned                          num_framebuffers ; ///< TODO

      data::module::Bus bus ;
      
      /** TODO
       */
      PipelineConfigurationData() ;
      
      /** TODO
       */
      void makeRasterizerInfo() ;
      
      /** TODO
       */
      void makeViewport() ;
      
      /** TODO
       */
      void makeColorAttachInfo() ;
      
      /** TODO
       */
      void makeColorInfo() ;
      
      /** TODO
       */
      void makeScissor() ;
      
      /** TODO
       */
      void makeViewportInfo() ;
      
      /** TODO
       */
      void makeSamplerInfo() ;
      
      /** TODO
       */
      void makeAssemblyInfo() ;
    };

    PipelineConfigurationData::PipelineConfigurationData()
    {
      color_blend_attachment.color_blend      = {} ;
      color_blend_info      .color_blend      = {} ;
      rasterization         .rasterizer_info  = {} ;
      multisampler          .multisample_info = {} ;
      viewport              .viewport         = {} ;
      assembly              .assembly_info    = {} ;
      scissor               .scissor          = {} ;
      viewport_info                           = {} ;
      name                                    = "" ;
      image_width                             = 0  ;
      image_height                            = 0  ;
      num_framebuffers                        = 0  ;

      this->makeRasterizerInfo () ;
      this->makeSamplerInfo    () ;
      this->makeViewport       () ;
      this->makeScissor        () ;
      this->makeAssemblyInfo   () ;
      this->makeColorAttachInfo() ;
      this->makeViewportInfo   () ;
      this->makeColorInfo      () ;
    };

    void ColorBlendCreateInfoConfiguration::setColorBlendLogicOperationEnable( bool param )
    {
      this->color_blend.logicOpEnable = param ;
    }

    void ColorBlendCreateInfoConfiguration::setColorBlendingLogicalOperation( const char* param )
    {
      this->color_blend.logicOp << param ;
    }

    void ColorBlendCreateInfoConfiguration::setColorBlendConstant( unsigned id, float param )
    {
      if( id < 4 ) this->color_blend.blendConstants[ id ] = param ;
    }

    void ColorBlendCreateInfoConfiguration::setColorBlendFlags( const char* param )
    {
//      this->color_blend.flags << param ;
    }

    void ColorBlendAttachmentConfiguration::setColorBlendChannels( const char* param )
    {
      this->color_blend.dstColorBlendFactor << param ;
    }

    void ColorBlendAttachmentConfiguration::setColorBlendEnable( bool param )
    {
      this->color_blend.blendEnable = param ;
    }

    void ColorBlendAttachmentConfiguration::setColorBlendSourceFactor( const char* param )
    {
      this->color_blend.srcColorBlendFactor << param ;
    }

    void ColorBlendAttachmentConfiguration::setColorBlendDistanceFactor( const char* param )
    {
      this->color_blend.dstColorBlendFactor << param ;
    }

    void ColorBlendAttachmentConfiguration::setColorBlendOperation( const char* param )
    {
      this->color_blend.colorBlendOp << param ;
    }

    void ColorBlendAttachmentConfiguration::setColorBlendSourceAlphaFactor( const char* param )
    {
      this->color_blend.srcColorBlendFactor<< param ;
    }

    void ColorBlendAttachmentConfiguration::setColorBlendDistanceAlphaFactor( const char* param )
    {
      this->color_blend.dstAlphaBlendFactor << param ;
    }

    void ColorBlendAttachmentConfiguration::setColorBlendAlphaOperation( const char* param )
    {
      this->color_blend.alphaBlendOp << param ;
    }

    void AssemblyConfiguration::setAssemblyTopology( const char* param )
    {
//      this->
    }

    void AssemblyConfiguration::setAssemblyPrimitiveRestartEnable( bool param )
    {
      this->assembly_info.primitiveRestartEnable = param ;
    }

    void ScissorConfiguration::setScissorOffsetX( float param )
    {
      this->scissor.offset.x = param ;
    }

    void ScissorConfiguration::setScissorOffsetY( float param )
    {
      this->scissor.offset.y = param ;
    }

    void ViewportConfiguration::setViewportX( float param )
    {
      this->viewport.x = param ;
    }

    void ViewportConfiguration::setViewportY( float param )
    {
      this->viewport.y = param ;
    }

    void ViewportConfiguration::setViewportMinDepth( float param )
    {
      this->viewport.minDepth = param ;
    }

    void ViewportConfiguration::setViewportMaxDepth( float param )
    {
      this->viewport.maxDepth = param ;
    }

    void MultisamplerConfiguration::setSamplerShadingEnable( bool param )
    {
      this->multisample_info.sampleShadingEnable = param ;
    }

    void MultisamplerConfiguration::setSamplerMinSampleShading( float param )
    {
      this->multisample_info.minSampleShading = param ;
    }

    void MultisamplerConfiguration::setSamplerAlphaToOneEnable( bool param )
    {
      this->multisample_info.alphaToOneEnable = param ;
    }

    void MultisamplerConfiguration::setSamplerAlphaToCoverageEnable( bool param )
    {
      this->multisample_info.alphaToCoverageEnable = param ;
    }

    void MultisamplerConfiguration::setSamplerRasterizationSamples( const char* param )
    {
      this->multisample_info.rasterizationSamples << param ;
    }

    void RasterizerConfiguration::setRasterizerDepthClampEnable( bool param )
    {
      this->rasterizer_info.depthClampEnable = param ;
    }

    void RasterizerConfiguration::setRasertizerDiscardEnable( bool param )
    {
      this->rasterizer_info.rasterizerDiscardEnable = param ;
    }

    void RasterizerConfiguration::setRasertizerPolygonMode( const char* param )
    {
      this->rasterizer_info.polygonMode << param ;
    }

    void RasterizerConfiguration::setRasterizerLineWidth( float param )
    {
      this->rasterizer_info.lineWidth = param ;
    }

    void RasterizerConfiguration::setRasterizerCullMode( const char* param )
    {
    
    }

    void RasterizerConfiguration::setRasterizerFrontFace( const char* param )
    {
    
    }

    void RasterizerConfiguration::setRasterizerDepthBiasEnable( bool param )
    {
      this->rasterizer_info.depthBiasEnable = param ;
    }

    void RasterizerConfiguration::setRasterizerDepthBiasConstantFactor( float param )
    {
      this->rasterizer_info.depthBiasConstantFactor = param ;
    }

    void RasterizerConfiguration::setRasterizerDepthBiasClamp( float param )
    {
      this->rasterizer_info.depthBiasClamp = param ;
    }

    void RasterizerConfiguration::setRasterizerDepthBiasSlopeFactor( float param )
    {
      this->rasterizer_info.depthBiasSlopeFactor = param ;
    }

    void PipelineConfigurationData::makeRasterizerInfo()
    {
      this->rasterization.rasterizer_info.sType                   = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO ;
      this->rasterization.rasterizer_info.depthClampEnable        = VK_FALSE                                                   ;
      this->rasterization.rasterizer_info.rasterizerDiscardEnable = VK_FALSE                                                   ;
      this->rasterization.rasterizer_info.polygonMode             = VK_POLYGON_MODE_FILL                                       ;
      this->rasterization.rasterizer_info.lineWidth               = 1.0f                                                       ;
      this->rasterization.rasterizer_info.cullMode                = VK_CULL_MODE_BACK_BIT                                      ;
      this->rasterization.rasterizer_info.frontFace               = VK_FRONT_FACE_CLOCKWISE                                    ;
      this->rasterization.rasterizer_info.depthBiasEnable         = VK_FALSE                                                   ;
      this->rasterization.rasterizer_info.depthBiasConstantFactor = 0.0f                                                       ;
      this->rasterization.rasterizer_info.depthBiasClamp          = 0.0f                                                       ;
      this->rasterization.rasterizer_info.depthBiasSlopeFactor    = 0.0f                                                       ;
    }
    
    void PipelineConfigurationData::makeSamplerInfo()
    {
      this->multisampler.multisample_info.sType                 = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO ;
      this->multisampler.multisample_info.sampleShadingEnable   = VK_FALSE                                                 ;
      this->multisampler.multisample_info.minSampleShading      = 1.0f                                                     ;
      this->multisampler.multisample_info.alphaToOneEnable      = VK_FALSE                                                 ;
      this->multisampler.multisample_info.alphaToCoverageEnable = VK_FALSE                                                 ;
      this->multisampler.multisample_info.pSampleMask           = nullptr                                                  ;
      this->multisampler.multisample_info.rasterizationSamples  = VK_SAMPLE_COUNT_1_BIT                                    ;
    }

    void PipelineConfigurationData::makeViewport()
    {
      this->viewport.viewport.x        = 0.0f ;
      this->viewport.viewport.y        = 0.0f ;
      this->viewport.viewport.minDepth = 0.0f ;
      this->viewport.viewport.maxDepth = 1.0f ;
    }

    void PipelineConfigurationData::makeScissor()
    {
      this->scissor.scissor.offset = { 0, 0 } ;
    }

    void PipelineConfigurationData::makeViewportInfo()
    {
      this->viewport_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO ;
    }

    void PipelineConfigurationData::makeColorInfo()
    {
      this->color_blend_info.color_blend.sType             = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO ;
      this->color_blend_info.color_blend.logicOpEnable     = VK_FALSE                                                 ;
      this->color_blend_info.color_blend.logicOp           = VK_LOGIC_OP_COPY                                         ;
      this->color_blend_info.color_blend.blendConstants[0] = 0.0f                                                     ;
      this->color_blend_info.color_blend.blendConstants[1] = 0.0f                                                     ;
      this->color_blend_info.color_blend.blendConstants[2] = 0.0f                                                     ;
      this->color_blend_info.color_blend.blendConstants[3] = 0.0f                                                     ;
      this->color_blend_info.color_blend.pNext             = NULL                                                     ;
      this->color_blend_info.color_blend.flags             = 0                                                        ;
    }

    void PipelineConfigurationData::makeColorAttachInfo()
    {
      this->color_blend_attachment.color_blend.colorWriteMask      = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT ;
      this->color_blend_attachment.color_blend.blendEnable         = VK_FALSE                                                                                                  ;
      this->color_blend_attachment.color_blend.srcColorBlendFactor = VK_BLEND_FACTOR_ONE                                                                                       ;
      this->color_blend_attachment.color_blend.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO                                                                                      ;
      this->color_blend_attachment.color_blend.colorBlendOp        = VK_BLEND_OP_ADD                                                                                           ;
      this->color_blend_attachment.color_blend.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE                                                                                       ;
      this->color_blend_attachment.color_blend.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO                                                                                      ;
      this->color_blend_attachment.color_blend.alphaBlendOp        = VK_BLEND_OP_ADD                                                                                           ;
    }

    void PipelineConfigurationData::makeAssemblyInfo()
    {
      this->assembly.assembly_info.sType                  = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO ;
      this->assembly.assembly_info.topology               = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST                         ;
      this->assembly.assembly_info.primitiveRestartEnable = VK_FALSE                                                    ;
    }
    
    PipelineConfiguration::PipelineConfiguration()
    {
      this->conf_data = new PipelineConfigurationData() ;
    }

    PipelineConfiguration::~PipelineConfiguration()
    {
      delete this->conf_data ;
    }

    void PipelineConfiguration::initialize( unsigned screen_width, unsigned screen_height, unsigned num_framebuffers )
    {
      data().image_width      = screen_width     ;
      data().image_height     = screen_height    ;
      data().num_framebuffers = num_framebuffers ;

      data().scissor.scissor.extent   = { data().image_width, data().image_height } ;
      data().viewport.viewport.width  = static_cast<float>( data().image_width  )   ;
      data().viewport.viewport.height = static_cast<float>( data().image_height )   ;
    }

    void PipelineConfiguration::subscribe( const char* name, unsigned id )
    {
      data().name = std::string( name ) ;
      data().bus.setId( id ) ;


    }

    VkViewport PipelineConfiguration::viewport() const
    {
      return data().viewport.viewport ;
    }

    VkPipelineViewportStateCreateInfo PipelineConfiguration::viewportCreateInfo()
    {
      // TODO: make this be able to support more than 1.
      data().viewport_info.viewportCount = 1                         ;
      data().viewport_info.pViewports    = &data().viewport.viewport ;
      data().viewport_info.scissorCount  = 1                         ;
      data().viewport_info.pScissors     = &data().scissor.scissor   ;

      return data().viewport_info ;
    }

    VkRect2D PipelineConfiguration::scissor() const
    {
      return data().scissor.scissor ;
    }

    VkPipelineColorBlendStateCreateInfo PipelineConfiguration::colorBlendInfo()
    {
      data().color_blend_info.color_blend.attachmentCount = 1                                          ;
      data().color_blend_info.color_blend.pAttachments    = &data().color_blend_attachment.color_blend ;

      return data().color_blend_info.color_blend ;
    }

    VkPipelineRasterizationStateCreateInfo PipelineConfiguration::rasterizerInfo() const
    {
      return data().rasterization.rasterizer_info ;
    }

    VkPipelineInputAssemblyStateCreateInfo PipelineConfiguration::assemblyInfo() const
    {
      return data().assembly.assembly_info ;
    }

    VkPipelineMultisampleStateCreateInfo PipelineConfiguration::multisampleInfo() const
    {
      return data().multisampler.multisample_info ;
    }

    const char* PipelineConfiguration::name() const
    {
      return data().name.c_str() ;
    }

    PipelineConfigurationData& PipelineConfiguration::data()
    {
      return *this->conf_data ;
    }

    const PipelineConfigurationData& PipelineConfiguration::data() const
    {
      return *this->conf_data ;
    }
  }
}