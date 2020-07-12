#include "Uniform.h"
#include <vulkan/vulkan.hpp>
#include <string>
namespace kgl
{
  namespace vk
  { 
    struct UniformIteratorData
    {

    };
    
    struct UniformData
    {
      
    };

    UniformIterator::UniformIterator()
    {
      this->iter_data = new UniformIteratorData() ;
    }

    UniformIterator::UniformIterator( const UniformIterator& iter )
    {
      this->iter_data = new UniformIteratorData() ;
    }

    UniformIterator::~UniformIterator()
    {
      delete this->iter_data ;
    }

    const char* UniformIterator::name() const
    {
    
    }

    unsigned UniformIterator::type() const
    {
    
    }

    const ::vk::Buffer UniformIterator::buffer() const
    {
    
    }

    void UniformIterator::operator++()
    {
    
    }

    bool UniformIterator::operator==( const UniformIterator& iter )
    {
    
    }

    void UniformIterator::operator=( const UniformIterator& iter )
    {
    
    }

    UniformIteratorData& UniformIterator::data()
    {
      return *this->iter_data ;
    }

    const UniformIteratorData& UniformIterator::data() const
    {
      return *this->iter_data ;
    }

    Uniform::Uniform()
    {
      this->uni_data = new UniformData() ;
    }

    Uniform::~Uniform()
    {
      delete this->uni_data ;
    }

    void Uniform::initialize( unsigned gpu )
    {
    
    }

    UniformIterator Uniform::begin() const
    {
    
    }

    UniformIterator Uniform::end() const
    {
    
    }

    UniformData& Uniform::data()
    {
      return *this->uni_data ;
    }

    const UniformData& Uniform::data() const
    {
      return *this->uni_data ;
    }
  }
}