#ifndef KGL_VK_DESCRIPTOR_H
#define KGL_VK_DESCRIPTOR_H

namespace vk
{
  class DescriptorSet  ;
  class DescriptorPool ;
  class CommandBuffer  ;
}
namespace kgl
{
  namespace vk
  {
    class Shader  ;
    class Uniform ;

    namespace render
    {
      class CommandBuffer ;
    }

    namespace compute
    {
      class CommandBuffer ;
    }

    class DescriptorSet
    {
      public:
        DescriptorSet() ;
        DescriptorSet( const DescriptorSet& set ) ;
        ~DescriptorSet() ;
        void operator=( const DescriptorSet& set ) ;
        void set( const Uniform& uniform ) ;
        const ::vk::DescriptorSet descriptor( unsigned index ) const ;
      private:
        struct DescriptorSetData *set_data ;
        DescriptorSetData& data() ;
        const DescriptorSetData& data() const ;
        friend class DescriptorPool ;
    };
    class DescriptorPool
    {
      public:
        DescriptorPool() ;
        ~DescriptorPool() ;
        const ::vk::DescriptorPool pool() const ;
        DescriptorSet makeDescriptorSet( unsigned count ) ;
        void initialize( unsigned gpu, unsigned max_amt, const Shader& shader ) ;
      private:
        struct DescriptorPoolData *pool_data ;
        DescriptorPoolData& data() ;
        const DescriptorPoolData& data() const ;
    };
  }
}
#endif /* DESCRIPTOR_H */

