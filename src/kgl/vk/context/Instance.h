#ifndef KGL_VK_INSTANCE_H
#define KGL_VK_INSTANCE_H

namespace vk 
{
  class Instance ;
}

namespace kgl
{
  namespace vk
  {
    class Instance
    {
      public:
        Instance() ;
        ~Instance() ;
        void initialize( bool validation_layers_enabled = true ) ;
        void reset() ;
        const ::vk::Instance instance() const ;
      
      private:
        struct InstanceData* instance_data ;
        InstanceData& data() ;
        const InstanceData& data() const ;
    };
  }
}

#endif /* INSTANCE_H */

