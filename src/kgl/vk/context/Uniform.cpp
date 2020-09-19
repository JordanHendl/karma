 #include "Uniform.h"
#include "Context.h"
#include "Buffer.h"
#include "Image.h"
#include <vulkan/vulkan.hpp>
#include <string>
#include <map>
#include <vector>

namespace kgl
{
  namespace vk
  { 
    
    /**
     * @param type
     * @return 
     */
    static Buffer::Type bufferTypeFromUni( Uniform::Type type ) ;
    
    struct UniformBuffer 
    {
      ::kgl::vk::Buffer buffer  ;
      ::kgl::vk::Image  image   ;
      Uniform::Type     type    ;
    };
    
    typedef std::map<std::string, UniformBuffer*> BufferMap ;
    
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

    Buffer::Type bufferTypeFromUni( Uniform::Type type )
    {
      switch( type )
      {
        case Uniform::Type::SSBO    : return Buffer::Type::SSBO    ;
        case Uniform::Type::UBO     : return Buffer::Type::UNIFORM ;
        case Uniform::Type::Sampler : return Buffer::Type::UNIFORM ;
        default                     : return Buffer::Type::UNIFORM ;
      }
    }
    UniformIterator::UniformIterator()
    {
      this->iter_data = new UniformIteratorData() ;
    }

    UniformIterator::UniformIterator( const UniformIterator& iter )
    {
      this->iter_data = new UniformIteratorData() ;
      
      *this->iter_data = *iter.iter_data ;
    }
    
    bool UniformIterator::isImage() const
    {
      return data().iter->second->type == Uniform::Type::Sampler ;
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
      return data().iter->second->type ;
    }
    
    const ::kgl::vk::Buffer& UniformIterator::buffer() const
    {
      return data().iter->second->buffer ;
    }
    
    const ::kgl::vk::Image& UniformIterator::image() const
    {
      return data().iter->second->image ;
    }

    void UniformIterator::operator++()
    {
      ++data().iter ;
    }

    bool UniformIterator::operator==( const UniformIterator& iter )
    {
      return data().iter == iter.data().iter ;
    }
    
    bool UniformIterator::operator!=( const UniformIterator& iter )
    {
      return data().iter != iter.data().iter ;
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
    
    Uniform::Uniform( const Uniform& uniform )
    {
      this->uni_data = new UniformData() ;
      
      *this->uni_data = *uniform.uni_data ;
    }
    
    Uniform::~Uniform()
    {
      delete this->uni_data ;
    }
    
    Uniform& Uniform::operator=( const Uniform& uniform )
    {
      *this->uni_data = *uniform.uni_data ;

      return *this ;
    }

    void Uniform::initialize( unsigned gpu )
    {
      const ::kgl::vk::compute::Context context ; 
      
      data().gpu_id = gpu                          ;
      data().gpu    = context.virtualDevice( gpu ) ;
      data().map.clear() ;
    }
    
    void Uniform::addImage( const char* name, const ::kgl::vk::Image& image )
    {
      BufferMap::iterator iter ;

      if( data().map.find( name ) == data().map.end() )
      {
        data().map.insert( { std::string( name ), new UniformBuffer() } ) ;
        
        iter = data().map.find( name ) ;
      }
      else
      {
        iter = data().map.find( name ) ;
      }
      
      iter->second->type = Uniform::Type::Sampler ;
      iter->second->image = ( image ) ;
    }

    void Uniform::addBase( const char* name, Type type, const void* val, unsigned element_size, unsigned count, bool host_local )
    {
      BufferMap::iterator iter ;
      
      
      if( data().map.find( name ) == data().map.end() )
      {
        data().map.insert( { std::string( name ), new UniformBuffer() } ) ;
        
        iter = data().map.find( name ) ;
        
        iter->second->buffer.initialize<char>( data().gpu_id, bufferTypeFromUni( type ), element_size * count, host_local ) ;
        iter->second->type = type ;
      }
      else
      {
        iter = data().map.find( name ) ;
      }
      
      iter->second->buffer.copyToDevice( val ) ;
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