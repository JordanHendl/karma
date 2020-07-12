#include "PipelineConfig.h"
#include <vulkan/vulkan.hpp>

namespace kgl
{
  namespace vk
  {
    struct PipelineConfigurationData
    {
      
    };

    PipelineConfiguration::PipelineConfiguration()
    {
      this->config_data = new PipelineConfigurationData() ;
    }

    PipelineConfiguration::~PipelineConfiguration()
    {
      delete this->config_data ;
    }

    void PipelineConfiguration::initialize( unsigned width, unsigned height, unsigned num_framebuffers )
    {
    
    }

    void PipelineConfiguration::subscribe( const char* name, unsigned channel )
    {
    
    }

    const ::vk::Viewport PipelineConfiguration::viewport() const
    {
    
    }

    const ::vk::PipelineViewportStateCreateInfo PipelineConfiguration::viewportCreateInfo() const
    {
    
    }

    const ::vk::Rect2D PipelineConfiguration::scissor() const
    {
    
    }

    const ::vk::PipelineColorBlendStateCreateInfo PipelineConfiguration::colorBlendCreateInfo() const
    {
    
    }

    const ::vk::PipelineRasterizationStateCreateInfo PipelineConfiguration::rasterizerCreateInfo() const
    {
    
    }

    const ::vk::PipelineInputAssemblyStateCreateInfo PipelineConfiguration::assemblyCreateInfo() const
    {
    
    }

    const ::vk::PipelineMultisampleStateCreateInfo PipelineConfiguration::multisampleStateCreateInfo() const
    {
    
    }

    const char* PipelineConfiguration::name() const
    {
    
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

