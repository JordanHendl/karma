#ifndef KGL_VK_PIPELINE_CONFIG_H
#define KGL_VK_PIPELINE_CONFIG_H

namespace vk
{
  class Viewport                             ;
  class Rect2D                               ;
  class PipelineViewportStateCreateInfo      ;
  class PipelineColorBlendStateCreateInfo    ;
  class PipelineRasterizationStateCreateInfo ;
  class PipelineInputAssemblyStateCreateInfo ;
  class PipelineMultisampleStateCreateInfo   ;
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
          
        void initialize( unsigned width, unsigned height ) ;
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

