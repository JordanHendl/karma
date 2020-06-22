#ifndef CASPER_VULKAN_VERTEX_H
#define CASPER_VULKAN_VERTEX_H

typedef struct VkVertexInputBindingDescription VkVertexInputBindingDescription ;
typedef struct VkVertexInputAttributeDescription VkVertexInputAttributeDescription ;

namespace casper
{
  namespace vulkan
  {
    class VertexType
    {
      public:
        virtual ~VertexType() {} ;
        virtual unsigned numDesc() const = 0 ;
        virtual unsigned numAttr() const = 0 ;
        virtual const VkVertexInputBindingDescription* desc()  const = 0 ;
        virtual const VkVertexInputAttributeDescription* attr() const = 0 ;
    };
  }
}
#endif