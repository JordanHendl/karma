#ifndef CASPER_DESCRIPTOR_SET_H
#define CASPER_DESCRIPTOR_SET_H

typedef struct VkDescriptorSetLayout_T* VkDescriptorSetLayout ;
namespace casper
{
  namespace vulkan
  {
    class Device ;
    class SwapChain ;

    class DescriptorSet
    {
      public:
        DescriptorSet() ;
        ~DescriptorSet() ;
        void initialize( unsigned count, const VkDescriptorSetLayout* layout ) ;
        void bind( const char* command_buffer_name ) ;
        void subscribe( unsigned id ) ;

      private:
      struct DescriptorSetData* descriptor_set_data ;
      DescriptorSetData& data() ;
      const DescriptorSetData& data() const ;
    };
  }
}
#endif