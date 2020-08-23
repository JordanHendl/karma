#include "PipelineConfig.h"
#include <vulkan/vulkan.hpp>
#include <Bus.h>
#include <Signal.h>

namespace kgl
{
  namespace vk
  {
    static inline void operator<<( ::vk::PolygonMode& op, const char* str ) ;
    static inline void operator<<( ::vk::SampleCountFlagBits& op, const char* str ) ;
    static inline void operator<<( ::vk::BlendOp& op, const char* str ) ;
    static inline void operator<<( ::vk::BlendFactor& op, const char* str ) ;  
//    static inline void operator<<( ::vk::PipelineColorBlendStateCreateFlags& op, const char* str ) ;
    static inline void operator<<( ::vk::LogicOp& op, const char* str ) ;

    /**
     */
    struct ColorBlendCreateInfoConfiguration
    {
      ::vk::PipelineColorBlendStateCreateInfo color_blend ;
      
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
      ::vk::PipelineColorBlendAttachmentState color_blend ;
      
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
      ::vk::PipelineInputAssemblyStateCreateInfo assembly_info ;
      
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
      ::vk::Rect2D scissor ;
      
      /**
       * @param param
       */
      void setScissorExtentX( unsigned param ) ;
      
      /**
       * @param param
       */
      void setScissorExtentY( unsigned param ) ;

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
      struct ViewportRatios
      {
        float width_ratio   = 1.0f ;
        float height_ratio  = 1.0f ;
      };

      typedef std::vector<::vk::Viewport> Viewports ;
      typedef std::vector<ViewportRatios> Ratios    ;
      
      
      Viewports viewports ;
      Ratios    ratios    ;

      /**
       * @param param
       */
      void setViewportX( unsigned id, float param ) ;
      
      /**
       * @param param
       */
      void setViewportY( unsigned id, float param ) ;
      
      /**
       * @param id
       * @param param
       */
      void setViewportWidth ( unsigned id, float param ) ;
      
      /**
       * @param id
       * @param param
       */
      void setViewportHeight( unsigned id, float param ) ;

      /**
       * @param param
       */
      void setViewportMinDepth( unsigned id, float param ) ;
      
      /**
       * @param param
       */
      void setViewportMaxDepth( unsigned id, float param ) ;
    };
    
    /**
     */
    struct MultisamplerConfiguration
    {
      ::vk::PipelineMultisampleStateCreateInfo multisample_info ;
      
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
      ::vk::PipelineRasterizationStateCreateInfo rasterizer_info  ;

      /**
       * @param param
       */
      void setRasterizerDepthClampEnable( bool clamp ) ;
      
      /**
       * @param param
       */
      void setRasterizerDiscardEnable( bool param ) ;
      
      /**
       * @param param
       */
      void setRasterizerPolygonMode( const char* param ) ;
      
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
      ColorBlendAttachmentConfiguration             color_blend_attachment ; ///< TODO
      ColorBlendCreateInfoConfiguration             color_blend_info       ; ///< TODO
      RasterizerConfiguration                       rasterization          ; ///< TODO
      MultisamplerConfiguration                     multisampler           ; ///< TODO
      mutable ViewportConfiguration                 viewport               ; ///< TODO
      AssemblyConfiguration                         assembly               ; ///< TODO
      ScissorConfiguration                          scissor                ; ///< TODO
      mutable ::vk::PipelineViewportStateCreateInfo viewport_info          ;
      
      std::string                       name             ; ///< TODO
      unsigned                          image_width      ; ///< TODO
      unsigned                          image_height     ; ///< TODO
      unsigned                          num_framebuffers ; ///< TODO

      ::data::module::Bus bus ;
      
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

    
    void operator<<( ::vk::PolygonMode& op, const char* str )
    {
      
    }

    void operator<<( ::vk::SampleCountFlagBits& op, const char* str )
    {
      
    }

    void operator<<( ::vk::BlendOp& op, const char* str )
    {
      
    }

    void operator<<( ::vk::BlendFactor& op, const char* str )
    {
      
    }
//
//    void operator<<( ::vk::PipelineColorBlendStateCreateFlags& op, const char* str )
//    {
//      
//    }

    void operator<<( ::vk::LogicOp& op, const char* str )
    {
      
    }

    void ColorBlendCreateInfoConfiguration::setColorBlendLogicOperationEnable( bool param )
    {
      this->color_blend.setLogicOpEnable( param );
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

    void ScissorConfiguration::setScissorExtentX( unsigned param )
    {
      this->scissor.extent.width = param ;
    }

    void ScissorConfiguration::setScissorExtentY( unsigned param )
    {
      this->scissor.extent.height = param ;
    }

    void ScissorConfiguration::setScissorOffsetX( float param )
    {
      this->scissor.offset.x = param ;
    }

    void ScissorConfiguration::setScissorOffsetY( float param )
    {
      this->scissor.offset.y = param ;
    }

    void ViewportConfiguration::setViewportX( unsigned id, float param )
    {
      const unsigned sz = id + 1 ;
      if( sz > this->viewports.size() ) { this->viewports.resize( sz ) ; this->ratios.resize( sz ) ; }
      
      this->viewports[ id ].x = param ;
    }
    
    void ViewportConfiguration::setViewportY( unsigned id, float param )
    {
      const unsigned sz = id + 1 ;
      if( sz > this->viewports.size() ) { this->viewports.resize( sz ) ; this->ratios.resize( sz ) ; }
      
      this->viewports[ id ].y = param ;
    }

    void ViewportConfiguration::setViewportWidth( unsigned id, float param )
    {
      const unsigned sz = id + 1 ;
      if( sz > this->viewports.size() ) { this->viewports.resize( sz ) ; this->ratios.resize( sz ) ; }
      
      this->ratios[ id ].width_ratio = param ;
    }

    void ViewportConfiguration::setViewportHeight( unsigned id, float param )
    {
      const unsigned sz = id + 1 ;
      if( sz > this->viewports.size() ) { this->viewports.resize( sz ) ; this->ratios.resize( sz ) ; }
      
      this->ratios[ id ].height_ratio = param ;
    }

    void ViewportConfiguration::setViewportMinDepth( unsigned id, float param )
    {
      const unsigned sz = id + 1 ;
      if( sz > this->viewports.size() ) { this->viewports.resize( sz ) ; this->ratios.resize( sz ) ; }
      
      this->viewports[ id ].minDepth = param ;
    }

    void ViewportConfiguration::setViewportMaxDepth( unsigned id, float param )
    {
      const unsigned sz = id + 1 ;
      if( sz > this->viewports.size() ) { this->viewports.resize( sz ) ; this->ratios.resize( sz ) ; }
      
      this->viewports[ id ].maxDepth = param ;
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

    void RasterizerConfiguration::setRasterizerDiscardEnable( bool param )
    {
      this->rasterizer_info.rasterizerDiscardEnable = param ;
    }

    void RasterizerConfiguration::setRasterizerPolygonMode( const char* param )
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

    PipelineConfigurationData::PipelineConfigurationData()
    {
      name             = "" ;
      image_width      = 0  ;
      image_height     = 0  ;
      num_framebuffers = 0  ;

      this->makeRasterizerInfo () ;
      this->makeSamplerInfo    () ;
      this->makeViewport       () ;
      this->makeScissor        () ;
      this->makeAssemblyInfo   () ;
      this->makeColorAttachInfo() ;
      this->makeViewportInfo   () ;
      this->makeColorInfo      () ;
    };

    void PipelineConfigurationData::makeRasterizerInfo()
    {
      this->rasterization.rasterizer_info.setDepthClampEnable       ( false                         ) ;
      this->rasterization.rasterizer_info.setRasterizerDiscardEnable( false                         ) ;
      this->rasterization.rasterizer_info.setPolygonMode            ( ::vk::PolygonMode::eFill      ) ;
      this->rasterization.rasterizer_info.setLineWidth              ( 1.0f                          ) ;
      this->rasterization.rasterizer_info.setCullMode               ( ::vk::CullModeFlagBits::eNone ) ;
      this->rasterization.rasterizer_info.setFrontFace              ( ::vk::FrontFace::eClockwise   ) ;
      this->rasterization.rasterizer_info.setDepthBiasEnable        ( false                         ) ;
      this->rasterization.rasterizer_info.setDepthBiasConstantFactor( 0.0f                          ) ;
      this->rasterization.rasterizer_info.setDepthBiasClamp         ( 0.0f                          ) ;
      this->rasterization.rasterizer_info.setDepthBiasSlopeFactor   ( 0.0f                          ) ;
    }
    
    void PipelineConfigurationData::makeSamplerInfo()
    {
      this->multisampler.multisample_info.setSampleShadingEnable  ( false                         ) ;
      this->multisampler.multisample_info.setMinSampleShading     ( 1.0f                          ) ;
      this->multisampler.multisample_info.setAlphaToOneEnable     ( false                         ) ;
      this->multisampler.multisample_info.setAlphaToCoverageEnable( false                         ) ;
      this->multisampler.multisample_info.setPSampleMask          ( nullptr                       ) ;
      this->multisampler.multisample_info.setRasterizationSamples ( ::vk::SampleCountFlagBits::e1 ) ;
    }

    void PipelineConfigurationData::makeViewport()
    {
//      this->viewport.viewport.setX       ( 0.0f ) ;
//      this->viewport.viewport.setY       ( 0.0f ) ;
//      this->viewport.viewport.setMinDepth( 0.0f ) ;
//      this->viewport.viewport.setMaxDepth( 1.0f ) ;
    }

    void PipelineConfigurationData::makeScissor()
    {
      this->scissor.scissor.offset.setX( 0 ) ;
      this->scissor.scissor.offset.setY( 0 ) ;
    }

    void PipelineConfigurationData::makeViewportInfo()
    {
    }

    void PipelineConfigurationData::makeColorInfo()
    {
      this->color_blend_info.color_blend.setLogicOpEnable    ( false                                     ) ;
      this->color_blend_info.color_blend.setLogicOp          ( ::vk::LogicOp::eCopy                      ) ;
      this->color_blend_info.color_blend.setAttachmentCount  ( 1                                         ) ;
      this->color_blend_info.color_blend.setPAttachments     ( &this->color_blend_attachment.color_blend ) ;
    }

    void PipelineConfigurationData::makeColorAttachInfo()
    {
      const ::vk::ColorComponentFlags mask = ::vk::ColorComponentFlagBits::eR | ::vk::ColorComponentFlagBits::eG | ::vk::ColorComponentFlagBits::eB | ::vk::ColorComponentFlagBits::eA ;
      this->color_blend_attachment.color_blend.setColorWriteMask     ( mask                                  ) ;
      this->color_blend_attachment.color_blend.setBlendEnable        ( true                                  ) ;
      this->color_blend_attachment.color_blend.setSrcColorBlendFactor( ::vk::BlendFactor::eSrcAlpha          ) ;
      this->color_blend_attachment.color_blend.setDstColorBlendFactor( ::vk::BlendFactor::eOneMinusSrc1Alpha ) ;
      this->color_blend_attachment.color_blend.setColorBlendOp       ( ::vk::BlendOp::eAdd                   ) ;
      this->color_blend_attachment.color_blend.setSrcAlphaBlendFactor( ::vk::BlendFactor::eOne               ) ;
      this->color_blend_attachment.color_blend.setDstAlphaBlendFactor( ::vk::BlendFactor::eZero              ) ;
      this->color_blend_attachment.color_blend.setAlphaBlendOp       ( ::vk::BlendOp::eAdd                   ) ;
    }

    void PipelineConfigurationData::makeAssemblyInfo()
    {
      this->assembly.assembly_info.setTopology              ( ::vk::PrimitiveTopology::eTriangleList ) ;
      this->assembly.assembly_info.setPrimitiveRestartEnable( false                                  ) ;
    }

    PipelineConfiguration::PipelineConfiguration()
    {
      this->config_data = new PipelineConfigurationData() ;
    }

    PipelineConfiguration::~PipelineConfiguration()
    {
      delete this->config_data ;
    }

    void PipelineConfiguration::initialize( unsigned width, unsigned height )
    {
      data().image_width  = width  ;
      data().image_height = height ;

      data().scissor.setScissorExtentX( width                        ) ;
      data().scissor.setScissorExtentY( height                       ) ;
      
      data().viewport_info.setPScissors    ( &data().scissor.scissor          ) ;
      data().viewport_info.setScissorCount ( 1                                ) ;
      data().viewport_info.setViewportCount( data().viewport.viewports.size() ) ;
      data().viewport_info.setPViewports   ( data().viewport.viewports.data() ) ;
    }

    void PipelineConfiguration::subscribe( const char* name, unsigned channel )
    {
      data().name = std::string( name ) ;
      data().bus.setChannel( channel ) ;
      
      data().bus( name, "::ColorBlendLogicOpEnable" ).attach( &data().color_blend_info, &ColorBlendCreateInfoConfiguration::setColorBlendLogicOperationEnable ) ;
      data().bus( name, "::ColorBlendLogicOp"       ).attach( &data().color_blend_info, &ColorBlendCreateInfoConfiguration::setColorBlendingLogicalOperation  ) ;
      data().bus( name, "::ColorBlendConstant"      ).attach( &data().color_blend_info, &ColorBlendCreateInfoConfiguration::setColorBlendConstant             ) ;
      data().bus( name, "::ColorBlendFlags"         ).attach( &data().color_blend_info, &ColorBlendCreateInfoConfiguration::setColorBlendFlags                ) ;
      
      data().bus( name, "::ColorBlendChannels"     ).attach( &data().color_blend_attachment, &ColorBlendAttachmentConfiguration::setColorBlendChannels            ) ;
      data().bus( name, "::ColorBlendEnable"       ).attach( &data().color_blend_attachment, &ColorBlendAttachmentConfiguration::setColorBlendEnable              ) ;
      data().bus( name, "::ColorBlendSourceFactor" ).attach( &data().color_blend_attachment, &ColorBlendAttachmentConfiguration::setColorBlendSourceFactor        ) ;
      data().bus( name, "::ColorBlendDstFactor"    ).attach( &data().color_blend_attachment, &ColorBlendAttachmentConfiguration::setColorBlendDistanceFactor      ) ;
      data().bus( name, "::ColorBlendOperation"    ).attach( &data().color_blend_attachment, &ColorBlendAttachmentConfiguration::setColorBlendOperation           ) ;
      data().bus( name, "::ColorBlendSourceAlpha"  ).attach( &data().color_blend_attachment, &ColorBlendAttachmentConfiguration::setColorBlendSourceAlphaFactor   ) ; 
      data().bus( name, "::ColorBlendDstAlpha"     ).attach( &data().color_blend_attachment, &ColorBlendAttachmentConfiguration::setColorBlendDistanceAlphaFactor ) ; 
      data().bus( name, "::ColorBlendOpAlpha"      ).attach( &data().color_blend_attachment, &ColorBlendAttachmentConfiguration::setColorBlendAlphaOperation      ) ; 
      
      data().bus( name, "::AssemblyRestartEnable" ).attach( &data().assembly, &AssemblyConfiguration::setAssemblyPrimitiveRestartEnable ) ;
      data().bus( name, "::AssemblyTopology"      ).attach( &data().assembly, &AssemblyConfiguration::setAssemblyTopology               ) ;
      
      data().bus( name, "::ScissorOffsetX" ).attach( &data().scissor, &ScissorConfiguration::setScissorOffsetX ) ;
      data().bus( name, "::ScissorOffsetY" ).attach( &data().scissor, &ScissorConfiguration::setScissorOffsetX ) ;
      data().bus( name, "::ScissorExtentX" ).attach( &data().scissor, &ScissorConfiguration::setScissorExtentX ) ;
      data().bus( name, "::ScissorExtentY" ).attach( &data().scissor, &ScissorConfiguration::setScissorExtentY ) ;
      
      data().bus( name, "::ViewportX"        ).attach( &data().viewport, &ViewportConfiguration::setViewportX        ) ;
      data().bus( name, "::ViewportY"        ).attach( &data().viewport, &ViewportConfiguration::setViewportY        ) ;
      data().bus( name, "::ViewportWidth"    ).attach( &data().viewport, &ViewportConfiguration::setViewportWidth    ) ;
      data().bus( name, "::ViewportHeight"   ).attach( &data().viewport, &ViewportConfiguration::setViewportHeight   ) ;
      data().bus( name, "::ViewportMinDepth" ).attach( &data().viewport, &ViewportConfiguration::setViewportMinDepth ) ;
      data().bus( name, "::ViewportMaxDepth" ).attach( &data().viewport, &ViewportConfiguration::setViewportMaxDepth ) ;
      
      data().bus( name, "::SamplerShadingEnable"         ).attach( &data().multisampler, &MultisamplerConfiguration::setSamplerShadingEnable         ) ;
      data().bus( name, "::SamplerMinShading"            ).attach( &data().multisampler, &MultisamplerConfiguration::setSamplerMinSampleShading      ) ;
      data().bus( name, "::SamplerAlphaToOneEnable"      ).attach( &data().multisampler, &MultisamplerConfiguration::setSamplerAlphaToOneEnable      ) ;
      data().bus( name, "::SamplerAlphaToCoverageEnable" ).attach( &data().multisampler, &MultisamplerConfiguration::setSamplerAlphaToCoverageEnable ) ;
      data().bus( name, "::SamplerRasterSamples"         ).attach( &data().multisampler, &MultisamplerConfiguration::setSamplerRasterizationSamples  ) ;
      
      data().bus( name, "::RasterDiscardEnable"     ).attach( &data().rasterization, &RasterizerConfiguration::setRasterizerDiscardEnable           ) ;
      data().bus( name, "::RasterDepthClampEnable"  ).attach( &data().rasterization, &RasterizerConfiguration::setRasterizerDepthClampEnable        ) ;
      data().bus( name, "::RasterPolygonMode"       ).attach( &data().rasterization, &RasterizerConfiguration::setRasterizerPolygonMode             ) ;
      data().bus( name, "::RasterLineWidth"         ).attach( &data().rasterization, &RasterizerConfiguration::setRasterizerLineWidth               ) ;
      data().bus( name, "::RasterCullMode"          ).attach( &data().rasterization, &RasterizerConfiguration::setRasterizerCullMode                ) ;
      data().bus( name, "::RasterFrontFace"         ).attach( &data().rasterization, &RasterizerConfiguration::setRasterizerFrontFace               ) ;
      data().bus( name, "::RasterDepthBiasEnable"   ).attach( &data().rasterization, &RasterizerConfiguration::setRasterizerDepthBiasEnable         ) ;
      data().bus( name, "::RasterDepthBiasConstant" ).attach( &data().rasterization, &RasterizerConfiguration::setRasterizerDepthBiasConstantFactor ) ;
      data().bus( name, "::RasterDepthBiasClamp"    ).attach( &data().rasterization, &RasterizerConfiguration::setRasterizerDepthBiasClamp          ) ;
      data().bus( name, "::RasterDepthBiasSlope"    ).attach( &data().rasterization, &RasterizerConfiguration::setRasterizerDepthBiasSlopeFactor    ) ;
    }

    const ::vk::Viewport PipelineConfiguration::viewport() const
    {
      return ::vk::Viewport() ; //TODO FIX 
//      return data().viewport.viewport ;
    }

    const ::vk::PipelineViewportStateCreateInfo PipelineConfiguration::viewportCreateInfo() const
    {
      for( unsigned index = 0; index < data().viewport.viewports.size(); index++ )
      {
        data().viewport.viewports[ index ].setWidth ( data().image_width  * data().viewport.ratios[ index ].width_ratio  ) ;
        data().viewport.viewports[ index ].setHeight( data().image_height * data().viewport.ratios[ index ].height_ratio ) ;
      }

      data().viewport_info.setPScissors    ( &data().scissor.scissor          ) ;
      data().viewport_info.setScissorCount ( 1                                ) ;
      data().viewport_info.setViewportCount( data().viewport.viewports.size() ) ;
      data().viewport_info.setPViewports   ( data().viewport.viewports.data() ) ;

      return data().viewport_info ;
    }

    const ::vk::Rect2D PipelineConfiguration::scissor() const
    {
      return data().scissor.scissor ;
    }

    const ::vk::PipelineColorBlendStateCreateInfo PipelineConfiguration::colorBlendCreateInfo() const
    {
      return data().color_blend_info.color_blend ;
    }

    const ::vk::PipelineRasterizationStateCreateInfo PipelineConfiguration::rasterizerCreateInfo() const
    {
      return data().rasterization.rasterizer_info ;
    }

    const ::vk::PipelineInputAssemblyStateCreateInfo PipelineConfiguration::assemblyCreateInfo() const
    {
      return data().assembly.assembly_info ;
    }

    const ::vk::PipelineMultisampleStateCreateInfo PipelineConfiguration::multisampleStateCreateInfo() const
    {
      return data().multisampler.multisample_info ;
    }

    const char* PipelineConfiguration::name() const
    {
      return data().name.c_str() ;
    }

    PipelineConfigurationData& PipelineConfiguration::data()
    {
      return *this->config_data ;
    }

    const PipelineConfigurationData& PipelineConfiguration::data() const
    {
      return *this->config_data ;
    }
  }
}

