#ifndef CASPER_DESCRIPTOR_POOL_H
#define CASPER_DESCRIPTOR_POOL_H
#include <vector>

typedef struct VkDescriptorSetLayout_T* VkDescriptorSetLayout ;
typedef struct VkDescriptorSetPool_T*   VkDescriptorSetPool   ;

namespace casper
{
  namespace vulkan
  {
    class Device ;

    class DescriptorPool
    {
      public:

        enum DescriptorType
        {
          UNIFORM     = 0,
          SAMPLER     = 1,
          TOTAL_TYPES = 2,
        };

        DescriptorPool() ;
        ~DescriptorPool() ;
        void initialize( const Device& device, const char* pipeline_name, unsigned num_layouts, const VkDescriptorSetLayout* layout ) ;
        void setAmount( const char* pipeline, DescriptorPool::DescriptorType type, unsigned count ) ; 
    };
  }
}
#endif