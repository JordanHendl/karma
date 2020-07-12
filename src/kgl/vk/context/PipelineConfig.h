/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   PipelineConfig.h
 * Author: jhendl
 *
 * Created on July 12, 2020, 1:59 AM
 */

#ifndef KGL_VK_PIPELINECONFIG_H
#define KGL_VK_PIPELINECONFIG_H

namespace vk
{
  class Viewport ;
  class PipelineViewportStateCreateInfo ;
  class Rect2D ;
  class PipelineColorBlendStateCreateInfo ;
  class PipelineRasterizationStateCreateInfo ;
  class PipelineInputAssemblyStateCreateInfo ;
  class PipelineMultisampleStateCreateInfo ;
}

namespace kgl
{
  namespace vk
  {
    class PipelineConfiguration
    {
      public:
        PipelineConfiguration() ;
        ~PipelineConfiguration() ;
          
        void initialize( unsigned width, unsigned height, unsigned num_framebuffers ) ;
        void subscribe( const char* name, unsigned channel ) ;
        const ::vk::Viewport viewport() const ;
        const ::vk::PipelineViewportStateCreateInfo viewportCreateInfo() const ;
        const ::vk::Rect2D scissor() const ;
        const ::vk::PipelineColorBlendStateCreateInfo colorBlendCreateInfo() const ;
        const ::vk::PipelineRasterizationStateCreateInfo rasterizerCreateInfo() const ;
        const ::vk::PipelineInputAssemblyStateCreateInfo assemblyCreateInfo() const ;
        const ::vk::PipelineMultisampleStateCreateInfo multisampleStateCreateInfo() const ;
        const char* name() const ;
      private:
        struct PipelineConfigurationData *config_data ;
        PipelineConfigurationData& data() ;
        const PipelineConfigurationData& data() const ;
    };
  }
}

#endif /* PIPELINECONFIG_H */

