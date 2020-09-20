#ifndef KGL_VK_UNIFORM
#define KGL_VK_UNIFORM

namespace vk
{
  class Buffer    ;
  class Semaphore ;
}

namespace kgl
{
  namespace vk
  {
    class Uniform ;
    class Image   ;
    class Buffer  ;
    class Synchronization ;
    class UniformIterator
    {
      public:
        UniformIterator() ;
        UniformIterator( const UniformIterator& iter ) ;
        ~UniformIterator() ;
        const char* name() const ;
        unsigned type() const ;
        bool isImage() const ;
        const ::kgl::vk::Buffer& buffer() const ;
        const ::kgl::vk::Image&  image() const ;
        void operator++() ;
        bool operator==( const UniformIterator& iter ) ;
        bool operator!=( const UniformIterator& iter ) ;
        void operator=( const UniformIterator& iter ) ;
      private:
        friend class Uniform ;
        struct UniformIteratorData* iter_data ;
        UniformIteratorData& data() ;
        const UniformIteratorData& data() const ;
    };

    class Uniform
    {
      public:
        
        enum Type : unsigned
        {
          SSBO    = 0 , 
          UBO     = 1 ,
          Sampler = 2 
        };
        
        Uniform() ;
        Uniform( const Uniform& uniform ) ;
        Uniform& operator=( const Uniform& uniform ) ;
        ~Uniform() ;
        void initialize( unsigned gpu ) ;
        UniformIterator begin() const ;
        UniformIterator end() const ;
        
        void addImage( const char* name, const ::kgl::vk::Image& image ) ;
        
        template<typename T>
        void add( const char* name, Type type, const T& val, bool host_local = false ) ;
        
        template<typename T>
        void add( const char* name, Type type, T* const val, unsigned count, bool host_local = false ) ;
      private:
        void addBase( const char* name, Type type, const void* val, unsigned element_size, unsigned count, bool host_local ) ;
        struct UniformData* uni_data ;
        UniformData& data() ;
        const UniformData& data() const ;
    };

    template<typename T>
    void Uniform::add( const char* name, Type type, const T& val, bool host_local )
    {
      this->addBase( name, type, static_cast<const void*>( &val ), sizeof( val ), 1, host_local ) ;
    }

    template<typename T>
    void Uniform::add( const char* name, Type type, T* const val, unsigned count, bool host_local )
    {
      this->addBase( name, type, static_cast<const void*>( val ), sizeof( T ), count, host_local ) ;
    }
  }
}
#endif
