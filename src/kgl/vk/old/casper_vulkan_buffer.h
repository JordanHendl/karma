#ifndef CASPER_VULKAN_BUFFER_H
#define CASPER_VULKAN_BUFFER_H

typedef struct VkBuffer_T *VkBuffer ;

namespace casper
{
  namespace vulkan
  {
    class Buffer ;
    
    /** TODO
     */
    enum class BufferType
    {
      VERTEX  = 0,
      UNIFORM = 1,
      SSBO    = 2,
      INDEX   = 3
    };
    
    /**
     */
    class BufferImpl
    {
      private:
        
        /**
         */
        BufferImpl() ;
        
        /**
         */
        ~BufferImpl() ;
        
        /**
         * @param device
         * @param type
         * @param element_sz
         * @param count
         */
        void initializeBase( unsigned device, BufferType type, unsigned element_sz, unsigned count, bool compute ) ;
        
        /**
         * @param data
         * @param offset
         */
        void copyToDevice( const void* data, unsigned offset ) ;
        
        /**
         * @param device
         * @param sz
         */
        void allocate( unsigned device, unsigned sz ) ;
        
        /**
         */
        void reset() ;
        
        /**
         * @return 
         */
        unsigned size() const ;
        
        /**
         * @return 
         */
        unsigned byteSize() const ;
        
        /**
         * @return 
         */
        const VkBuffer& buffer() const ;
        
        /**
         * @return 
         */
        BufferType type() const ;

        /**
         */
        struct BufferData *buffer_data ;
        
        /**
         * @return 
         */
        const BufferData& data() const ;
        
        /**
         * @return 
         */
        BufferData& data() ;
        
        /**
         */
        friend class Buffer ;
    };
    
    /**
     */
    class Buffer
    {
      public:
        
        /**
         * @param device
         * @param type
         * @param count
         */
        template<typename T>
        void initialize( unsigned device, BufferType type, unsigned count, bool compute = false ) ;
        
        /**
         * @param data
         * @param offset
         */
        template<typename T>
        void copyToDevice( const T& data, unsigned offset = 0 ) ;

        /**
         * @param data
         * @param offset
         */
        template<typename T>
        void copyToDevice( const T* data, unsigned offset = 0 ) ;
        
        /**
         * @return 
         */
        const VkBuffer& buffer() const ;
        
        /**
         * @return 
         */
        unsigned elementSize() const ;

        /**
         * @return 
         */
        unsigned byteSize() const ;
        
        /**
         * @return 
         */
        BufferType type() const ;
        
        /**
         * @return 
         */
        unsigned size() const ;
        
        /**
         */
        void reset() ;
      private:
        
        /**
         */
        BufferImpl impl ;
    };

    template<typename T>
    void Buffer::copyToDevice( const T& data, unsigned offset )
    {
       this->impl.copyToDevice( static_cast<const void*>( &data ), offset ) ;
    }

    template<typename T>
    void Buffer::copyToDevice( const T* data, unsigned offset )
    {
       this->impl.copyToDevice( reinterpret_cast<const void*>( data ), offset ) ;
    }
    
    template<typename T>
    void Buffer::initialize( unsigned device, BufferType type, unsigned count, bool compute )
    {
      this->impl.initializeBase( device, type, sizeof( T ), count, compute ) ;
    } ;
  }
}
#endif
