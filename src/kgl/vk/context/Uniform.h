#ifndef KGL_VK_UNIFORM
#define KGL_VK_UNIFORM

namespace vk
{
  class Buffer ;
}

namespace kgl
{
  namespace vk
  {
    class Uniform ;

    class UniformIterator
    {
      public:
        UniformIterator() ;
        UniformIterator( const UniformIterator& iter ) ;
        ~UniformIterator() ;
        const char* name() const ;
        unsigned type() const ;
        const ::vk::Buffer buffer() const ;
        void operator++() ;
        bool operator==( const UniformIterator& iter ) ;
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
        ~Uniform() ;
        void initialize( unsigned gpu ) ;
        UniformIterator begin() const ;
        UniformIterator end() const ;

        template<typename T>
        void add( const char* name, Type type, T val ) ;
        
        template<typename T>
        void add( const char* name, Type type, const T& val ) ;
        
        template<typename T>
        void add( const char* name, Type type, T* const val, unsigned count ) ;
      private:
        void addBase( const char* name, Type type, const void* val, unsigned element_size, unsigned count ) ;
        struct UniformData* uni_data ;
        UniformData& data() ;
        const UniformData& data() const ;
    };
  }
}
#endif
