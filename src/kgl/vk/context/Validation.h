#ifndef CASPER_VULKAN_VALIDATION_LAYER_H
#define CASPER_VULKAN_VALIDATION_LAYER_H

namespace kgl
{
  namespace vk
  {
    class Validation
    {
      public:
        Validation() ;
        ~Validation() ;
        bool supported() ;
        const char** names() ;
        unsigned count() ;
        
      private:
        struct ValidationData* layer_data ;
        ValidationData& data() ;
        const ValidationData& data() const ;
    };
  }
}
#endif
