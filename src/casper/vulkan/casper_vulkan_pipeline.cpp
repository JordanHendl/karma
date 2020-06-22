#include "casper_vulkan_pipeline.h"
#include "casper_vulkan_device.h"
#include "casper_vulkan_vertex.h"
#include "casper_vulkan_swapchain.h"
#include "casper_vulkan_command_buffer.h"
#include <casper_shader.h>
#include <vulkan/vulkan.h>
#include <cerrno>
#include <vector>
#include <memory>

namespace casper
{
  namespace vulkan
  {
    VkShaderStageFlagBits GetShaderStage( const std::string& stage )
    {
      if (stage == "vert")
      {
        return VK_SHADER_STAGE_VERTEX_BIT ;
      }
      else if (stage == "tesc")
      {
        return VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
      }
      else if (stage == "tess")
      {
        return VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT ;
      }
      else if (stage == "geom")
      {
        return VK_SHADER_STAGE_GEOMETRY_BIT ;
      }
      else if (stage == "frag")
      {
        return VK_SHADER_STAGE_FRAGMENT_BIT ;
      }
      else if (stage == "comp")
      {
        return VK_SHADER_STAGE_COMPUTE_BIT ;
      }
      else
      {
        throw std::runtime_error("Unknown shader stage" ) ;
        return VK_SHADER_STAGE_COMPUTE_BIT ;
      }
    }

    VkShaderModule createModule( const std::vector<unsigned>& code, const Device& device )
    {
      VkShaderModuleCreateInfo info   ;
      VkShaderModule           module ;

      info          = {}                                          ;
      info.sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO ;
      info.codeSize = code.size() * sizeof( unsigned )            ;
      info.pCode    = code.data()                                 ;
      info.pNext    = NULL                                        ;

      if( vkCreateShaderModule( device.device(), &info, nullptr, &module ) != VK_SUCCESS )
      {
        throw std::runtime_error( "Failed to create shader module.") ;
      } 
      return module ;
    }

    VkPipelineVertexInputStateCreateInfo genVertexInfo( const VertexType& type )
    {
      VkPipelineVertexInputStateCreateInfo info ;

      info                                 = {}                                                        ;
      info.sType                           = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO ;
      info.vertexAttributeDescriptionCount = type.numAttr()                                            ;
      info.pVertexAttributeDescriptions    = type.attr()                                               ;
      info.vertexBindingDescriptionCount   = type.numDesc()                                            ;
      info.pVertexBindingDescriptions      = type.desc()                                               ;

      return info ;
    }

    VkPipelineInputAssemblyStateCreateInfo genAssInfo()
    {
      VkPipelineInputAssemblyStateCreateInfo ass ;

      ass       = {} ;
      ass.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO ;
      ass.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST ;
      ass.primitiveRestartEnable = VK_FALSE ;

      return ass ;
    }
    struct PipelineData
    {
      typedef std::vector<VkShaderModule                 > ShaderModules ;
      typedef std::vector<VkPipelineShaderStageCreateInfo> CreateInfos   ;

      VkPipeline                              pipeline           ;
      VkGraphicsPipelineCreateInfo            pipe_info          ;
      VkPipelineLayout                        layout             ;
      VkAttachmentDescription                 desc               ;
      VkPipelineColorBlendAttachmentState     color_blend_info   ;
      VkPipelineDepthStencilStateCreateInfo   depth_stencil_info ;
      VkPipelineRasterizationStateCreateInfo  rasta_info         ;
      VkPipelineMultisampleStateCreateInfo    sampler_info       ;
      VkAttachmentReference                   attach_ref         ;
      VkSubpassDescription                    subpass_desc       ;
      VkRenderPass                            render_pass        ;
      VkRect2D              scissor  ;
      VkViewport            viewport ;
      ShaderModules         modules  ;
      CreateInfos           infos    ;
      casper::tools::Shader shader   ;
    };
    
    Pipeline::Pipeline()
    {
      this->pipe_data = new PipelineData() ;
    }

    Pipeline::~Pipeline()
    {
      delete this->pipe_data ;
    }

    void Pipeline::initialize( const char* path, SwapChain& chain, const Device& device, const VertexType& type )
    {
      
      VkPipelineShaderStageCreateInfo         info             ;
      VkPipelineVertexInputStateCreateInfo    v_info           ;
      VkPipelineVertexInputStateCreateInfo    state_info       ; 
      VkPipelineInputAssemblyStateCreateInfo  ass_info         ;
      VkPipelineLayoutCreateInfo              layout_info      ;
      VkPipelineViewportStateCreateInfo       viewport_info    ;
      VkRenderPassCreateInfo                  render_pass_info ;
      VkPipelineColorBlendStateCreateInfo     color_info       ;
      VkSubpassDependency                     dependency       ;

      unsigned i ;

      if( path )
      {
        data().shader.read( device.device(), path ) ;
        data().modules.clear() ;
        i = 0 ;

        for( auto it = data().shader.begin(); it != data().shader.end(); ++it )
        {
          data().modules.push_back( createModule( it->second, device ) )    ;
          info        =                                                  {} ;
          info.sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO ;
          info.stage  = GetShaderStage( it->first )                         ;
          info.module = data().modules.at( i )                              ;
          info.pName  = "main"                                              ;

          i++ ;

          data().infos.push_back( info ) ;
        }
        
        state_info = genVertexInfo( type ) ;
        ass_info   = genAssInfo()          ;

        data().viewport.x        = 0.0f                                   ;
        data().viewport.y        = 0.0f                                   ;
        data().viewport.width    = static_cast<float>( chain.width()  )   ;
        data().viewport.height   = static_cast<float>( chain.height(  ) ) ;
        data().viewport.minDepth = 0.0f                                   ;
        data().viewport.maxDepth = 1.0f                                   ;

        data().scissor.offset = { 0            , 0              } ;
        data().scissor.extent = { chain.width(), chain.height() } ;

        viewport_info               = {}                                                     ;
        viewport_info.sType         = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO ;
        viewport_info.viewportCount = 1                                                     ;
        viewport_info.pViewports    = &data().viewport                                      ;
        viewport_info.scissorCount  = 1                                                     ;
        viewport_info.pScissors     = &data().scissor                                       ;

        layout_info                        = {}                                            ;
        layout_info.sType                  = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO ;
        layout_info.setLayoutCount         = 0 ;//data().shader.numDescriptors()                ;
        layout_info.pSetLayouts            = NULL ;//data().shader.descriptors()                   ;
        layout_info.pushConstantRangeCount = 0                                             ;
        layout_info.pPushConstantRanges    = NULL                                          ;

        data().desc                = {}                                      ;
        data().desc.format         = static_cast<VkFormat>( chain.format() ) ;
        data().desc.samples        = VK_SAMPLE_COUNT_1_BIT                   ;
        data().desc.loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR             ;
        data().desc.storeOp        = VK_ATTACHMENT_STORE_OP_STORE            ;
        data().desc.stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE         ;
        data().desc.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE        ;
        data().desc.initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED               ;
        data().desc.finalLayout    = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR         ;

        data().attach_ref = {}                                                  ;
        data().attach_ref.attachment = 0                                        ;
        data().attach_ref.layout     = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL ;

        data().subpass_desc                      = {}                              ;
        data().subpass_desc.pipelineBindPoint    = VK_PIPELINE_BIND_POINT_GRAPHICS ;
        data().subpass_desc.colorAttachmentCount = 1                               ;
        data().subpass_desc.pColorAttachments    = &data().attach_ref              ;

        dependency               = {}                                            ;
        dependency.srcSubpass    = VK_SUBPASS_EXTERNAL                           ;
        dependency.dstSubpass    = 0                                             ;
        dependency.srcStageMask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT ;
        dependency.srcAccessMask = 0                                             ;
        dependency.dstStageMask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT ;
        dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT          ;

        render_pass_info                 = {}                                        ;
        render_pass_info.sType           = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO ;
        render_pass_info.attachmentCount = 1                                         ;
        render_pass_info.pAttachments    = &data().desc                              ;
        render_pass_info.subpassCount    = 1                                         ;
        render_pass_info.pSubpasses      = &data().subpass_desc                      ;
        render_pass_info.dependencyCount = 1                                         ;
        render_pass_info.pDependencies   = &dependency                               ;

        color_info.sType             = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO ;
        color_info.logicOpEnable     = VK_FALSE                                                 ;
        color_info.logicOp           = VK_LOGIC_OP_COPY                                         ;
        color_info.attachmentCount   = 1                                                        ;
        color_info.pAttachments      = &data().color_blend_info                                 ;
        color_info.blendConstants[0] = 0.0f                                                     ;
        color_info.blendConstants[1] = 0.0f                                                     ;
        color_info.blendConstants[2] = 0.0f                                                     ;
        color_info.blendConstants[3] = 0.0f                                                     ;
        color_info.pNext             = NULL                                                     ;
        color_info.flags             = 0                                                        ;

        buildRasterizer() ;

        if( vkCreateRenderPass( device.device(), &render_pass_info, NULL, &data().render_pass ) != VK_SUCCESS )
        {
          throw std::runtime_error( "Failed to create Vulkan Render Pass.") ;
        }

        if( vkCreatePipelineLayout( device.device(), &layout_info, NULL, &data().layout ) != VK_SUCCESS )
        {
          throw std::runtime_error( "Failed to create Vulkan Pipeline Layout!" ) ;
        }

        data().sampler_info.sType                = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO ;
        data().sampler_info.sampleShadingEnable  = VK_FALSE                                                 ;
        data().sampler_info.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT                                    ;
        data().sampler_info.pNext                = NULL                                                     ;

        data().pipe_info                     = {}                                              ;
        data().pipe_info.sType               = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO ;
        data().pipe_info.stageCount          = data().infos.size()                             ;
        data().pipe_info.pStages             = data().infos.data()                             ;
        data().pipe_info.pVertexInputState   = &state_info                                     ;
        data().pipe_info.pInputAssemblyState = &ass_info                                       ;
        data().pipe_info.pViewportState      = &viewport_info                                  ;
        data().pipe_info.pRasterizationState = &data().rasta_info                              ;
        data().pipe_info.pMultisampleState   = &data().sampler_info                            ;
        data().pipe_info.pColorBlendState    = &color_info                                     ;
        data().pipe_info.layout              = data().layout                                   ;
        data().pipe_info.subpass             = 0                                               ;
        data().pipe_info.renderPass          = data().render_pass                              ;
        data().pipe_info.basePipelineHandle  = VK_NULL_HANDLE                                  ;
        data().pipe_info.pNext               = NULL                                            ;

        if( vkCreateGraphicsPipelines( device.device(), VK_NULL_HANDLE, 1, &data().pipe_info, NULL, &data().pipeline ) != VK_SUCCESS )
        {
          throw std::runtime_error( "Failed to create Vulkan Pipeline!" ) ;
        }

        chain.createFrameBuffers( data().render_pass, device ) ;
      }
    }
    
    void Pipeline::buildRasterizer()
    {
        data().rasta_info                         = {}                                                         ;
        data().rasta_info.sType                   = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO ;
        data().rasta_info.depthClampEnable        = VK_FALSE                                                   ;
        data().rasta_info.rasterizerDiscardEnable = VK_FALSE                                                   ;
        data().rasta_info.polygonMode             = VK_POLYGON_MODE_FILL                                       ;
        data().rasta_info.lineWidth               = 1.0f                                                       ;
        data().rasta_info.cullMode                = VK_CULL_MODE_BACK_BIT                                      ;
        data().rasta_info.frontFace               = VK_FRONT_FACE_CLOCKWISE                                    ;
        data().rasta_info.depthBiasEnable         = VK_FALSE                                                   ;
        data().rasta_info.depthBiasConstantFactor = 0.0f                                                       ;
        data().rasta_info.depthBiasClamp          = 0.0f                                                       ;
        data().rasta_info.depthBiasSlopeFactor    = 0.0f                                                       ;
    }

    void Pipeline::buildMultisampler()
    {
      data().sampler_info                       = {}                                                       ;
      data().sampler_info.sType                 = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO ;
      data().sampler_info.sampleShadingEnable   = VK_FALSE                                                 ;
      data().sampler_info.minSampleShading      = 1.0f                                                     ;
      data().sampler_info.alphaToOneEnable      = VK_FALSE                                                 ;
      data().sampler_info.alphaToCoverageEnable = VK_FALSE                                                 ;
      data().sampler_info.pSampleMask           = nullptr                                                  ;
      data().sampler_info.rasterizationSamples  = VK_SAMPLE_COUNT_1_BIT                                    ;
    }

    void Pipeline::buildDepthStencilBuffers()
    {

    }

    const VkRenderPass& Pipeline::renderPass() const
    {
      return data().render_pass ;
    }

    void Pipeline::buildColorBlend()
    {
      data().color_blend_info                     = {}                                                                                                        ;
      data().color_blend_info.colorWriteMask      = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT ;
      data().color_blend_info.blendEnable         = VK_FALSE                                                                                                  ;
      data().color_blend_info.srcColorBlendFactor = VK_BLEND_FACTOR_ONE                                                                                       ;
      data().color_blend_info.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO                                                                                      ;
      data().color_blend_info.colorBlendOp        = VK_BLEND_OP_ADD                                                                                           ;
      data().color_blend_info.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE                                                                                       ;
      data().color_blend_info.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO                                                                                      ;
      data().color_blend_info.alphaBlendOp        = VK_BLEND_OP_ADD                                                                                           ;
    }

    void Pipeline::bind( const CommandBuffer& buffer, unsigned framebuffer ) const 
    {
      vkCmdBindPipeline( buffer.buffer( framebuffer ), VK_PIPELINE_BIND_POINT_GRAPHICS, data().pipeline ) ;
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