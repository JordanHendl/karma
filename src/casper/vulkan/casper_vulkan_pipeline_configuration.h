#ifndef CASPER_VULKAN_PIPELINE_CONFIGURATION_H
#define CASPER_VULKAN_PIPELINE_CONFIGURATION_H
#include <vulkan/vulkan.h>

namespace casper
{
  namespace vulkan
  {
    class Window ;
    
    /** TODO
     */
    class PipelineConfiguration
    {
      public:
        
        /**
         */
        PipelineConfiguration() ;
        
        /**
         * @param window
         */
        ~PipelineConfiguration() ;
        
        /**
         * @param window
         */
        void initialize( unsigned screen_width, unsigned screen_height, unsigned num_framebuffers ) ;
        
        /**
         * @param name
         * @param id
         */
        void subscribe( const char* name, unsigned id ) ;
        
        /**
         * @return 
         */
        VkViewport viewport() const ;
        
        /**
         * @return 
         */
        VkPipelineViewportStateCreateInfo viewportCreateInfo() ;
        
        /**
         * @return 
         */
        VkRect2D scissor() const ;
        
        /**
         * @return 
         */
        VkPipelineColorBlendStateCreateInfo colorBlendInfo() ;
        
        /**
         * @return 
         */
        VkPipelineRasterizationStateCreateInfo rasterizerInfo() const ;
        
        /**
         * @return 
         */
        VkPipelineInputAssemblyStateCreateInfo assemblyInfo() const ;
        
        /**
         * @return 
         */
        VkPipelineMultisampleStateCreateInfo multisampleInfo() const ;
        
        /**
         * @return 
         */
        const char* name() const ;
      private:
        
        /**
         */
        struct PipelineConfigurationData* conf_data ;
        
        /**
         * @return 
         */
        PipelineConfigurationData& data() ;
        
        /**
         */
        const PipelineConfigurationData& data() const ;
    };
  }
}

#endif