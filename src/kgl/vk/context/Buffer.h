#ifndef KGL_VK_BUFFER_H
#define KGL_VK_BUFFER_H

#include "CommandBuffer.h"


namespace vk
{
  class Buffer        ;
  class CommandBuffer ;
}

namespace kgl
{
  namespace vk
  {
    namespace render
    {
      class CommandBuffer ;
    }
    
    class Buffer ;
    
    class BufferImpl
    {
      private:
        BufferImpl() ;
        ~BufferImpl() ;
        
        void initializeBase( unsigned device, unsigned type, unsigned element_sz, bool host_local, unsigned count ) ;
        void copyToDevice( const void* data, unsigned count, unsigned offset = 0  ) ;
        
        
        struct BufferData *buffer_data ;
        BufferData& data() ;
        const BufferData& data() const ;

        friend class Buffer ;
    };

    class Buffer
    {
      public:
        
        enum Type : unsigned
        {
          VERTEX  = 0,
          INDEX   = 1,
          UNIFORM = 2,
          SSBO    = 3
        };

        Buffer() ;
        ~Buffer() ;
        const ::vk::Buffer buffer() const ;
        
        void operator=( const Buffer& buffer ) ;
        template<typename T>
        void initialize( unsigned device, Type type, unsigned count, bool host_local = false ) ;

        template<typename T>
        void copyToDevice( const T& data, unsigned count = 0, unsigned offset = 0 ) ;
        
        template<typename T>
        void copyToDevice( T* const data, unsigned count = 0, unsigned offset = 0 ) ;

        Type type() const ;
        
        void copy( const Buffer& buffer ) ;
        
        void reset() ;
        unsigned byteSize() const ;
        unsigned elementSize() const ;
      private:
        BufferImpl impl ;
    };
    
    template<typename T>
    void Buffer::initialize( unsigned device, Type type, unsigned count, bool host_local )
    {
      this->impl.initializeBase( device, type, sizeof( T ), host_local, count ) ;
    }

    template<typename T>
    void Buffer::copyToDevice( const T& data, unsigned count, unsigned offset )
    {
      this->impl.copyToDevice( static_cast<const void*>( &data ), count, offset ) ;
    }
    
    template<typename T>
    void Buffer::copyToDevice( T* const data, unsigned count, unsigned offset )
    {
      this->impl.copyToDevice( static_cast<const void*>( data ), count, offset ) ;
    }
  }
}

#endif /* BUFFER_H */

