#ifndef CASPER_VULKAN_VALIDATION_LAYER_H
#define CASPER_VULKAN_VALIDATION_LAYER_H

namespace casper
{
  namespace vulkan
  {
    class ValidationLayer
    {
      public:
        ValidationLayer() ;
        ~ValidationLayer() ;
        bool supported() ;
        const char** names() ;
        unsigned count() ;
        
      private:
        struct LayerData* layer_data ;
        LayerData& data() ;
        const LayerData& data() const ;
    };
  }
}
#endif
