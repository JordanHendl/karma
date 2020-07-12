#include "Descriptor.h"
#include "CommandBuffer.h"
#include <vulkan/vulkan.hpp>

namespace kgl
{
  namespace vk
  {
    struct DescriptorSetData
    {
      
    };
    
    struct DescriptorPoolData
    {
      
    };

    DescriptorSet::DescriptorSet()
    {
      this->set_data = new DescriptorSetData() ;
    }

    DescriptorSet::DescriptorSet( const DescriptorSet& set )
    {
      this->set_data = new DescriptorSetData() ;
      
      *this->set_data = *set.set_data ;
    }

    DescriptorSet::~DescriptorSet()
    {
      delete this->set_data ;
    }

    void DescriptorSet::bind( const render::CommandBuffer& buffer )
    {
    
    }

    void DescriptorSet::bind( const compute::CommandBuffer& buffer )
    {
    
    }

    void DescriptorSet::operator=( const DescriptorSet& set )
    {
    
    }

    void DescriptorSet::set( const Uniform& uniform )
    {
    
    }

    const ::vk::DescriptorSet DescriptorSet::set() const
    {
    
    }

    DescriptorSetData& DescriptorSet::data()
    {
      return *this->set_data ;
    }

    const DescriptorSetData& DescriptorSet::data() const
    {
      return *this->set_data ;
    }

    DescriptorPool::DescriptorPool()
    {
      this->pool_data = new DescriptorPoolData() ;
    }

    DescriptorPool::~DescriptorPool()
    {
      delete this->pool_data ;
    }

    const ::vk::DescriptorPool DescriptorPool::pool() const
    {
    
    }

    DescriptorSet DescriptorPool::makeDescriptorSet( unsigned count )
    {
    
    }

    void DescriptorPool::initialize( unsigned gpu, unsigned max_amt, const Shader& shader )
    {
    
    }

    DescriptorPoolData& DescriptorPool::data()
    {
      return *this->pool_data ;
    }

    const DescriptorPoolData& DescriptorPool::data() const
    {
      return *this->pool_data ;
    }
  }
}