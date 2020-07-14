#include "Uniform.h"
#include "Context.h"
#include "Buffer.h"
#include <vulkan/vulkan.hpp>
#include <string>
#include <map>
#include <vector>

namespace kgl
{
  namespace vk
  { 
    typedef std::map<std::string, ::kgl::vk::Buffer> BufferMap ;
    
    struct UniformIteratorData
    {
      BufferMap::const_iterator iter ;
    };
    
    struct UniformData
    {
      BufferMap    map    ;
      ::vk::Device gpu    ;
      unsigned     gpu_id ;
    };

    UniformIterator::UniformIterator()
    {
      this->iter_data = new UniformIteratorData() ;
    }

    UniformIterator::UniformIterator( const UniformIterator& iter )
    {
      this->iter_data = new UniformIteratorData() ;
      
      *this->iter_data = *iter.iter_data ;
    }

    UniformIterator::~UniformIterator()
    {
      delete this->iter_data ;
    }

    const char* UniformIterator::name() const
    {
      return data().iter->first.c_str() ;
    }

    unsigned UniformIterator::type() const
    {
      return data().iter->second.type() ;
    }

    const ::vk::Buffer UniformIterator::buffer() const
    {
      return data().iter->second.buffer() ;
    }

    void UniformIterator::operator++()
    {
      ++data().iter ;
    }

    bool UniformIterator::operator==( const UniformIterator& iter )
    {
      return data().iter == iter.data().iter ;
    }

    void UniformIterator::operator=( const UniformIterator& iter )
    {
      *this->iter_data = *iter.iter_data ;
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
      const ::kgl::vk::compute::Context context ; 
      
      data().gpu_id = gpu                          ;
      data().gpu    = context.virtualDevice( gpu ) ;
      data().map.clear() ;
    }
    
    void Uniform::addBase( const char* name, Type type, const void* val, unsigned element_size, unsigned count )
    {
      BufferMap::iterator iter ;

      if( data().map.find( name ) == data().map.end() )
      {
        data().map.insert( { std::string( name ), ::kgl::vk::Buffer() } ) ;
        
        iter = data().map.find( name ) ;
        iter->second.initialize<void*>( data().gpu_id, Buffer::Type::UNIFORM, element_size * count ) ;
      }
      else
      {
        iter = data().map.find( name ) ;
      }
      
      iter->second.copyToDevice( val ) ;
    }
    
    UniformIterator Uniform::begin() const
    {
      UniformIterator iter ;
      
      iter.data().iter = data().map.begin() ;
      return iter ;
    }

    UniformIterator Uniform::end() const
    {
      UniformIterator iter ;
      
      iter.data().iter = data().map.end() ;
      return iter ;
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