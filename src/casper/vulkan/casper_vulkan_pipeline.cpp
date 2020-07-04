#include "casper_vulkan_pipeline.h"
#include "casper_vulkan_device.h"
#include "casper_vulkan_pipeline_configuration.h"
#include "casper_vulkan_error_handler.h"
#include "casper_vulkan_window.h"
#include "casper_vulkan_swapchain.h"
#include "casper_vulkan_descriptor_pool.h"
#include "casper_vulkan_command_buffer.h"
#include "casper_vulkan_shader.h"
#include "shader/UWUShader.h"
#include <vulkan/vulkan.h>
#include <cerrno>
#include <vector>
#include <memory>

namespace casper
{
  namespace vulkan
  {
    static inline VkShaderStageFlagBits vulkanBitFromStage( const ::tools::shader::ShaderStage& stage ) ;
    
    struct PipelineData
    {
      VkPipeline                            pipeline       ; ///< TODO
      VkPipelineLayout                      layout         ; ///< TODO
      casper::vulkan::PipelineConfiguration configuration  ; ///< TODO
      casper::vulkan::DescriptorPool        pool           ; ///< TODO
      casper::vulkan::DescriptorSet         descriptor     ; ///< TODO
      casper::vulkan::Shader                shader         ; ///< TODO
      std::string                           name           ; ///< TODO
      unsigned                              num_framebuffs ;
      /**
       * @param window
       * @param vertex
       */
      void createPipelineLayout( const VkDevice& device ) ;
      
      /**
       * @param window
       * @param vertex
       */
      void createPipeline( const Window& window ) ;
      
      /**
       * @param window
       */
      void bind( const CommandBuffer& buffer ) const ;
    };
    
    VkShaderStageFlagBits vulkanBitFromStage( const ::tools::shader::ShaderStage& stage )
    {
      switch( stage )
      {
        case ::tools::shader::ShaderStage::FRAGMENT      : return VK_SHADER_STAGE_FRAGMENT_BIT                ;
        case ::tools::shader::ShaderStage::GEOMETRY      : return VK_SHADER_STAGE_GEOMETRY_BIT                ;
        case ::tools::shader::ShaderStage::TESSALATION_C : return VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT    ;
        case ::tools::shader::ShaderStage::TESSELATION_E : return VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT ;
        case ::tools::shader::ShaderStage::COMPUTE       : return VK_SHADER_STAGE_COMPUTE_BIT                 ;
        case ::tools::shader::ShaderStage::VERTEX        : return VK_SHADER_STAGE_VERTEX_BIT                  ;
        default                                          : return VK_SHADER_STAGE_VERTEX_BIT                  ;
      }
    }

    void PipelineData::bind( const CommandBuffer& buffer ) const
    {
      for( unsigned i = 0; i < this->num_framebuffs; i++ )
      {
        vkCmdBindDescriptorSets( buffer.buffer( i ), VK_PIPELINE_BIND_POINT_GRAPHICS, this->layout, 0, 1, this->descriptor[ i ], 0, nullptr ) ;
        vkCmdBindPipeline      ( buffer.buffer( i ), VK_PIPELINE_BIND_POINT_GRAPHICS, this->pipeline                                        ) ;
      }
    }

    void PipelineData::createPipeline( const Window& window )
    {
      const VkPipelineViewportStateCreateInfo      viewport_state_info = this->configuration.viewportCreateInfo() ;
      const VkRect2D                               scissor             = this->configuration.scissor()            ;
      const VkViewport                             viewport            = this->configuration.viewport()           ;
      const VkPipelineColorBlendStateCreateInfo    color_blend         = this->configuration.colorBlendInfo()     ;
      const VkPipelineRasterizationStateCreateInfo raster_info         = this->configuration.rasterizerInfo()     ;
      const VkPipelineInputAssemblyStateCreateInfo assembly_info       = this->configuration.assemblyInfo()       ;
      const VkPipelineMultisampleStateCreateInfo   multisample_info    = this->configuration.multisampleInfo()    ;
      std::vector<VkPipelineShaderStageCreateInfo> shader_stage_infos ;
      VkPipelineShaderStageCreateInfo              shader_stage_info  ;
      VkPipelineVertexInputStateCreateInfo         vertex_info        ;
      VkGraphicsPipelineCreateInfo                 info               ;
      VkShaderStageFlagBits                        bits               ;

      for( auto it = this->shader.begin(); it != this->shader.end(); ++it ) 
      {
        bits = vulkanBitFromStage( it.stage() ) ;
        shader_stage_info        =                                                  {} ;
        shader_stage_info.sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO ;
        shader_stage_info.stage  =  bits                                               ;
        shader_stage_info.module = this->shader.module( it.stage() )                   ;
        shader_stage_info.pName  = "main"                                              ;
        
        shader_stage_infos.push_back( shader_stage_info ) ;
      }

      vertex_info                                 = {}                                                        ;
      vertex_info.sType                           = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO ;
      vertex_info.vertexAttributeDescriptionCount = this->shader.numVertexAttr()                              ;
      vertex_info.pVertexAttributeDescriptions    = this->shader.attributeVertexDesc()                        ;
      vertex_info.vertexBindingDescriptionCount   = this->shader.numVertexDesc()                              ;
      vertex_info.pVertexBindingDescriptions      = this->shader.bindingVertexDesc()                          ;

      info                     = {}                                              ;
      info.sType               = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO ;
      info.stageCount          = shader_stage_infos.size()                       ;
      info.pStages             = shader_stage_infos.data()                       ;
      info.pVertexInputState   = &vertex_info                                    ;
      info.pInputAssemblyState = &assembly_info                                  ;
      info.pViewportState      = &viewport_state_info                            ;
      info.pRasterizationState = &raster_info                                    ;
      info.pMultisampleState   = &multisample_info                               ;
      info.pColorBlendState    = &color_blend                                    ;
      info.layout              = this->layout                                    ;
      info.subpass             = 0                                               ;
      info.renderPass          = window.pass()                                   ;
      info.basePipelineHandle  = VK_NULL_HANDLE                                  ;
      info.pNext               = NULL                                            ;

      HANDLE_ERROR( vkCreateGraphicsPipelines( window.device().device(), VK_NULL_HANDLE, 1, &info, NULL, &this->pipeline ) ) ;
    }

    void PipelineData::createPipelineLayout( const VkDevice& device )
    {
      VkPipelineLayoutCreateInfo info ;

      info                        = {}                                            ;
      info.sType                  = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO ;
      info.setLayoutCount         = 1                                             ;
      info.pSetLayouts            = this->shader.layout()                         ;
      info.pushConstantRangeCount = 0                                             ;
      info.pPushConstantRanges    = NULL                                          ;

      HANDLE_ERROR( vkCreatePipelineLayout( device, &info, NULL, &this->layout ) ) ;
    }

    Pipeline::Pipeline()
    {
      this->pipe_data = new PipelineData() ;
    }

    Pipeline::~Pipeline()
    {
      delete this->pipe_data ;
    }

    void Pipeline::subscribe( const char* name, unsigned id )
    {
      data().name = name ; 
      data().configuration.subscribe( name, id ) ;
    }

    void Pipeline::initialize( const char* path, const Window& window )
    {
      data().num_framebuffs = window.chain().numBuffers() ;

      if( path )
      {
        data().name = path ;
        data().shader.read( window.device(), path )                                                                     ;  
        data().pool         .initialize( window, data().shader, path                                                  ) ;
        data().configuration.initialize( window.chain().width(), window.chain().height(), window.chain().numBuffers() ) ;
        data().descriptor = data().pool.makeDescriptorSet( window.chain().numBuffers() )                                ;
        data().createPipelineLayout( window.device().device() )                                                         ;
        data().createPipeline      ( window                   )                                                         ;
      }
    }

    void Pipeline::bind( const CommandBuffer& buffer ) const 
    {
      this->data().bind( buffer ) ;
    }
    
    const DescriptorSet& Pipeline::set()
    {
      return data().descriptor ;
    }

    const char* Pipeline::name() const
    {
      return data().name.c_str() ;
    }

    PipelineData& Pipeline::data()
    {
      return *this->pipe_data ;
    }

    const PipelineData& Pipeline::data() const
    {
      return *this->pipe_data ;
    }
  }
}