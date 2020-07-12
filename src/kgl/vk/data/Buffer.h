#ifndef KGL_VK_BUFFER_H
#define KGL_VK_BUFFER_H

namespace vk
{
  class Buffer ;
}

namespace kgl
{
  namespace vk
  {
    class Buffer ;
    
    class BufferImpl
    {
      private:
        void initializeBase( unsigned device, unsigned type, unsigned element_sz, unsigned count ) ;
        void copyToDevice( const void* data, unsigned offset ) ;
        
        
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
        
        template<typename T>
        void initialize( unsigned device, Type type, unsigned count ) ;

        template<typename T>
        void copyToDevice( const T& data, unsigned offset = 0 ) ;

        template<typename T>
        void copyToDevice( T* const data, unsigned offset = 0 ) ;
        
        unsigned byteSize() const ;
        unsigned elementSize() const ;
      private:
        BufferImpl impl ;
    };
    
    template<typename T>
    void Buffer::initialize( unsigned device, Type type, unsigned count )
    {
      this->impl.initializeBase( device, type, sizeof( T ), count ) ;
    }

    template<typename T>
    void Buffer::copyToDevice( const T& data, unsigned offset )
    {
      this->impl.copyToDevice( static_cast<const void*>( &data ), offset ) ;
    }

    template<typename T>
    void Buffer::copyToDevice( T* const data, unsigned offset )
    {
      this->impl.copyToDevice( static_cast<const void*>( &data ), offset ) ;
    }
  }
}

#endif /* BUFFER_H */

