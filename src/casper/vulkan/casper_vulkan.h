#ifndef CASPER_VULKAN_H
#define CASPER_VULKAN_H

namespace casper
{
  class Vulkan
  {
    public:
      Vulkan() ;
      ~Vulkan() ;
      void initialize() ;
      bool initialized() ;
      void shutDown() ;
    private:
      struct VulkanData* vk_data ;
      VulkanData& data() ;
      const VulkanData& data() const ;
  } ;
}

#endif