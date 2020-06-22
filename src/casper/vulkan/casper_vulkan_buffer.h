#ifndef CASPER_VULKAN_BUFFER_H
#define CASPER_VULKAN_BUFFER_H

typedef struct VkBuffer_T *VkBuffer ;

namespace casper
{
  namespace vulkan
  {
    class Device ;

    template<typename T>
    class Buffer ;

    enum class BufferType
    {
      VERTEX = 0,
    };
    

    class BufferImpl
    {
      private:
      BufferImpl() ;
      ~BufferImpl() ;
      void initializeBase( const Device& device, BufferType type, unsigned element_sz, unsigned count ) ;
      void copyToDevice( void* data, unsigned offset ) ;
      void allocate( const Device& device, unsigned sz ) ;
      void reset() ;
      unsigned size() const ;
      const VkBuffer& buffer() const ;

      struct BufferData *buffer_data ;
      const BufferData& data() const ;
      BufferData& data() ;
      template<typename> friend class Buffer ;
    };
    
    template<typename T>
    class Buffer
    {
      public:
        void intialize( const Device& device, BufferType type, unsigned count ) ;
        void copyToDevice( T* data, unsigned offset = 0 ) ;
        const VkBuffer& buffer() const ;
        unsigned size() const ;
        void reset() ;
      private:
        BufferImpl impl ;
    };
    

    template<typename T>
    const VkBuffer& Buffer<T>::buffer() const
    {
      return impl.buffer() ;
    }

    template<typename T>
    unsigned Buffer<T>::size() const
    {
      return impl.size() ;
    }

    template<typename T>
    void Buffer<T>::copyToDevice( T* data, unsigned offset )
    {
       impl.copyToDevice( reinterpret_cast<void*>( data ), offset ) ;
    }

    template<typename T>
    void Buffer<T>::intialize( const Device& device, BufferType type, unsigned count )
    {
      this->impl.initializeBase( device, type, sizeof( T ), count ) ;
    } ;
  }
}
#endif