#include "Pipeline.h"
#include "PipelineConfig.h"
#include "Context.h"
#include "Shader.h"
#include "Descriptor.h"
#include "Uniform.h"
#include "CommandBuffer.h"
#include "Context.h"
#include "Device.h"
#include <shader/UWUShader.h>
#include <vulkan/vulkan.hpp>
#include <string>
#include <limits.h>

namespace kgl
{
  namespace vk
  {
    namespace render
    {
      struct PipelineData
      {
        ::kgl::vk::PipelineConfiguration config   ; ///< 
        ::kgl::vk::DescriptorPool        pool     ; ///< 
        ::kgl::vk::DescriptorSet         set      ; ///< 
        ::kgl::vk::Shader                shader   ; ///< 
        ::vk::Pipeline                   pipeline ; ///< 
        ::vk::PipelineLayout             layout   ; ///< 
        ::vk::RenderPass                 pass     ; ///< 
        std::string                      name     ; ///<
        unsigned                         num_desc ; ///<
        unsigned                         gpu      ; ///<

        /**
         */
        PipelineData() ;
        
        /**
         */
        void createLayout() ;
        
        /**
         * @param buffer
         */
        void createPipeline() ;
        
        /**
         */
        void bind( const ::kgl::vk::render::CommandBuffer& buffer ) const ;
      };
    }
    
    namespace compute
    {
      struct PipelineData
      {
        ::kgl::vk::PipelineConfiguration config   ; ///< 
        ::kgl::vk::DescriptorPool        pool     ; ///< 
        ::kgl::vk::DescriptorSet         set      ; ///< 
        ::kgl::vk::Shader                shader   ; ///< 
        ::vk::Pipeline                   pipeline ; ///< 
        ::vk::PipelineLayout             layout   ; ///< 
        std::string                      name     ; ///<
        unsigned                         num_desc ; ///<
        unsigned                         gpu      ; ///<

        /**
         */
        PipelineData() ;
        
        /**
         */
        void createLayout() ;
        
        /**
         * @param buffer
         */
        void createPipeline() ;
        
        /**
         * @param buffer
         */
        void bind( const ::kgl::vk::compute::CommandBuffer& buffer ) const ;
      };
    }

    namespace render
    {
      PipelineData::PipelineData()
      {
        this->gpu = UINT_MAX ;
      }

      void PipelineData::createLayout()
      {
        const ::kgl::vk::render::Context context                                          ;
        const ::vk::Device               device      = context.virtualDevice( this->gpu ) ;
        const auto                       desc_layout = this->shader.layout()              ;
        ::vk::PipelineLayoutCreateInfo info ;
        
        info.setSetLayoutCount( 1            ) ;
        info.setPSetLayouts   ( &desc_layout ) ;
        
        this->layout = device.createPipelineLayout( info, nullptr ) ;
      }

      void PipelineData::createPipeline()
      {
        const ::kgl::vk::render::Context context ;
        const ::vk::Device               device           = context.virtualDevice( this->gpu )        ;
        const auto                       viewport_state   = this->config.viewportCreateInfo()         ;
        const auto                       color_blend      = this->config.colorBlendCreateInfo()       ;
        const auto                       raster_info      = this->config.rasterizerCreateInfo()       ;
        const auto                       assembly_info    = this->config.assemblyCreateInfo()         ;
        const auto                       multisample_info = this->config.multisampleStateCreateInfo() ;
        ::vk::GraphicsPipelineCreateInfo         info        ;
        ::vk::PipelineVertexInputStateCreateInfo vertex_info ;
        
        vertex_info.setVertexAttributeDescriptionCount( this->shader.numVertexAttributes() ) ;
        vertex_info.setPVertexAttributeDescriptions   ( this->shader.vertexAttributes()    ) ;
        vertex_info.setVertexBindingDescriptionCount  ( this->shader.numVertexBindings()   ) ;
        vertex_info.setPVertexBindingDescriptions     ( this->shader.vertexBindings()      ) ;
        
        info.setPStages            ( this->shader.infos()      ) ;
        info.setStageCount         ( this->shader.numShaders() ) ;
        info.setLayout             ( this->layout              ) ;
        info.setPVertexInputState  ( &vertex_info              ) ;
        info.setPInputAssemblyState( &assembly_info            ) ;
        info.setPViewportState     ( &viewport_state           ) ;
        info.setPRasterizationState( &raster_info              ) ;
        info.setPMultisampleState  ( &multisample_info         ) ;
        info.setPColorBlendState   ( &color_blend              ) ;
        info.setRenderPass         ( this->pass                ) ;

        this->pipeline = device.createGraphicsPipeline( ::vk::PipelineCache(), info ) ;
      }

      void PipelineData::bind( const ::kgl::vk::render::CommandBuffer& buffer ) const
      {
        const auto bind_point = ::vk::PipelineBindPoint::eGraphics ;
        
        ::vk::CommandBuffer cmd ;
        ::vk::DescriptorSet set ;
        for( unsigned index = 0; index < buffer.count(); index++ )
        {
          cmd = buffer.buffer       ( index ) ;
          set = this->set.descriptor( index ) ;
          
          cmd.bindDescriptorSets( bind_point, this->layout, 0, 1, &set, 0, nullptr ) ;
          cmd.bindPipeline      ( bind_point, this->pipeline                       ) ;
        }
      }

      Pipeline::Pipeline()
      {
        this->pipe_data = new PipelineData() ;
      }

      Pipeline::~Pipeline()
      {
        delete this->pipe_data ;
      }

      void Pipeline::initialize( const char* uwu_path, unsigned gpu, unsigned num_descriptors, const ::vk::RenderPass pass )
      {
        data().num_desc = num_descriptors ;
        
        if( uwu_path ) 
        {
          data().name = uwu_path ;
          data().pass = pass     ;
          data().gpu  = gpu      ;
          data().shader .load( gpu, uwu_path )                             ;
          data().pool   .initialize( gpu, num_descriptors, data().shader ) ;
          data().config .initialize( 1280, 1024                          ) ;
          data().set = data().pool.makeDescriptorSet( num_descriptors )    ;
          data().createLayout()                                            ;
          data().createPipeline()                                          ;
        }
      }
      
      void Pipeline::subscribe( const char* name, unsigned channel )
      {
        data().config.subscribe( name, channel ) ;
      }

      void Pipeline::bind( const ::kgl::vk::render::CommandBuffer& buffer ) const
      {
        data().bind( buffer ) ;
      }

      void Pipeline::set( const ::kgl::vk::Uniform& uniform )
      {
        data().set.set( uniform ) ;
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

    namespace compute
    {
      PipelineData::PipelineData()
      {
        this->gpu = UINT_MAX ;
      }

      void PipelineData::createLayout()
      {
        const ::kgl::vk::render::Context context                                          ;
        const ::vk::Device               device      = context.virtualDevice( this->gpu ) ;
        const auto                       desc_layout = this->shader.layout()              ;
        ::vk::PipelineLayoutCreateInfo info ;
        
        info.setSetLayoutCount( 1            ) ;
        info.setPSetLayouts   ( &desc_layout ) ;
        
        this->layout = device.createPipelineLayout( info, nullptr ) ;
      }

      void PipelineData::createPipeline()
      {
        const ::kgl::vk::render::Context context                                     ;
        const ::vk::Device               device = context.virtualDevice( this->gpu ) ;
        ::vk::ComputePipelineCreateInfo info ;
        
        info.setStage ( *this->shader.infos() ) ;
        info.setLayout( this->layout          ) ;
        
        this->pipeline = device.createComputePipeline( ::vk::PipelineCache(), info ) ;
      }

      void PipelineData::bind( const ::kgl::vk::compute::CommandBuffer& buffer ) const
      {
        const auto bind_point = ::vk::PipelineBindPoint::eCompute ;
        
        ::vk::CommandBuffer cmd ;
        ::vk::DescriptorSet set ;
        for( unsigned index = 0; index < buffer.count(); index++ )
        {
          cmd = buffer.buffer       ( index ) ;
          set = this->set.descriptor( index ) ;
          
          cmd.bindDescriptorSets( bind_point, this->layout, 0, 1, &set, 0, nullptr ) ;
          cmd.bindPipeline      ( bind_point, this->pipeline                       ) ;
        }
      }

      Pipeline::Pipeline()
      {
        this->pipe_data = new PipelineData() ;
      }

      Pipeline::~Pipeline()
      {
        delete this->pipe_data ;
      }

      void Pipeline::initialize( const char* uwu_path, unsigned gpu, unsigned num_descriptors )
      {
        data().num_desc = num_descriptors ;
        
        if( uwu_path ) 
        {
          data().name = uwu_path ;

          data().shader .load( gpu, uwu_path )                             ;
          data().pool   .initialize( gpu, num_descriptors, data().shader ) ;
          data().config .initialize( 1280, 1024                          ) ;
          data().set = data().pool.makeDescriptorSet( num_descriptors )    ;
          data().createLayout()                                            ;
          data().createPipeline()                                          ;
        }
      }

      void Pipeline::bind( const ::kgl::vk::compute::CommandBuffer& buffer ) const
      {
        this->data().bind( buffer ) ;
      }

      void Pipeline::set( const ::kgl::vk::Uniform& uniform )
      {
        data().set.set( uniform ) ;
      }

      void Pipeline::set( const char* key, ::vk::Buffer ssbo )
      {
      
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
}