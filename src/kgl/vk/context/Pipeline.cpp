#include "Pipeline.h"
#include "Shader.h"
#include "Descriptor.h"
#include "Uniform.h"
#include <shader/UWUShader.h>
#include <vulkan/vulkan.hpp>
#include <string>

namespace kgl
{
  namespace vk
  {
    namespace render
    {
      struct PipelineData
      {
//        ::kgl::vk::DescriptorPool pool     ;
//        ::kgl::vk::DescriptorSet  set      ;
//        ::kgl::vk::Shader         shader   ;
        ::vk::Pipeline            pipeline ;
        ::vk::PipelineLayout      layout   ;
      };
    }
    
    namespace compute
    {
      struct PipelineData
      {
//        ::kgl::vk::DescriptorPool pool     ;
//        ::kgl::vk::DescriptorSet  set      ;
//        ::kgl::vk::Shader         shader   ;
        ::vk::Pipeline            pipeline ;
        ::vk::PipelineLayout      layout   ;
        
      };
    }

    namespace render
    {
      Pipeline::Pipeline()
      {
        this->pipe_data = new PipelineData() ;
      }

      Pipeline::~Pipeline()
      {
        delete this->pipe_data ;
      }

      void Pipeline::initialize( const char* uwu_path, const char* window_name )
      {
        
      }

      void Pipeline::bind( const ::kgl::vk::render::CommandBuffer& buffer ) const
      {
      
      }

      void Pipeline::set( const ::kgl::vk::Uniform& uniform )
      {
      
      }

      const char* Pipeline::name() const
      {
      
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
      Pipeline::Pipeline()
      {
        this->pipe_data = new PipelineData() ;
      }

      Pipeline::~Pipeline()
      {
        delete this->pipe_data ;
      }

      void Pipeline::initialize( const char* uwu_path, unsigned gpu )
      {
      
      }

      void Pipeline::bind( const ::kgl::vk::compute::CommandBuffer& buffer ) const
      {
      
      }

      void Pipeline::set( const ::kgl::vk::Uniform& uniform )
      {
      
      }

      void Pipeline::set( const char* key, ::vk::Buffer ssbo )
      {
      
      }

      const char* Pipeline::name() const
      {
      
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