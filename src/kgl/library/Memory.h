#ifndef KGL_MEMORY_H
#define KGL_MEMORY_H

namespace kgl
{
  class List ;

  template<typename T>
  T combine( T first, T second )
  {
    return first | second ;
  }

  template<typename T, typename... TYPES>
  T combine( T first, T second, TYPES... types )
  {
    return first | second | combine( types... ) ;
  }
    
  /** Class to manage memory on the GPU & CPU.
   */
  template<typename IMPL>
  class Buffer
  {
    public:
      
      Buffer() ;
      
      ~Buffer() = default ;
      
      template<typename TYPE, typename ... BUFFER_FLAGS, typename ... MEMORY_FLAGS>
      void initialize( unsigned sz, unsigned gpu, BUFFER_FLAGS... buff_flags, MEMORY_FLAGS... mem_flags, bool host_alloc = true ) ;
      
      template<typename TYPE>
      void initialize( unsigned sz, unsigned gpu, bool host_alloc = true ) ;
        
      template<typename TYPE>
      void copySynced( TYPE* src, unsigned amt, unsigned offset = 0 ) ;
      
      template<typename TYPE>
      void copyToHost( TYPE* src, unsigned amt, unsigned offset = 0 ) ;
      
      template<typename TYPE>
      void copyToDevice( TYPE* src, unsigned amt, unsigned offset = 0 ) ;
      
      const typename IMPL::Buffer& buffer() const ;
      
      void deallocate() ;
      
      void syncToDevice() ;
      
      void syncToHost() ;
      
      bool dirty() const ;
      
      unsigned size() const ;
      
      unsigned byteSize() const ;
      
    private:
      typedef unsigned Size   ;
      typedef unsigned Device ;
      typedef bool     Flag   ;
      typedef void*    Data   ;
      
      Flag                  dirty_bit  ;
      Flag                  host_alloc ;
      Data                  data       ;
      Size                  element_sz ;
      Size                  count      ;
      Device                gpu        ;
      IMPL                  impl       ;
      typename IMPL::Buffer *buff      ;
      typename IMPL::Memory *memory    ;
  };
  
  /** Class to manage memory on the GPU & CPU.
   */
  template<typename IMPL>
  Buffer<IMPL>::Buffer()
  {
    this->dirty_bit  = false   ;
    this->data       = nullptr ;
    this->count      = 0       ;
    this->element_sz = 0       ;
    this->buff       = nullptr ;
    this->memory     = nullptr ;
    this->host_alloc = false   ;
  }
  
  template<typename IMPL>
  void Buffer<IMPL>::deallocate()
  {
    delete this->data ;
    impl.freeBuffer( this->buff, this->memory, this->gpu ) ;
    
    this->buff   = nullptr ;
    this->memory = nullptr ;
  }

  template<typename IMPL>
  template<typename TYPE, typename ... BUFFER_FLAGS, typename ... MEMORY_FLAGS>
  void Buffer<IMPL>::initialize( unsigned sz, unsigned gpu, BUFFER_FLAGS... buff_flags, MEMORY_FLAGS... mem_flags, bool host_alloc ) 
  {
    this->count      = sz             ;
    this->element_sz = sizeof( TYPE ) ;
    this->gpu        = gpu            ;
    
    this->buff = impl.createBuffer( sz * sizeof( TYPE ), gpu ) ;
    
    if( host_alloc )
    {
      this->data = new unsigned char[ sz * sizeof( TYPE ) ] ;
    }
  }

  template<typename IMPL>
  template<typename TYPE>
  void Buffer<IMPL>::initialize( unsigned sz, unsigned gpu, bool host_alloc ) 
  {
    this->count      = sz             ;
    this->element_sz = sizeof( TYPE ) ;
    this->gpu        = gpu            ;
    
    this->buff = impl.createBuffer( sz * sizeof( TYPE ), gpu ) ;
    
    if( host_alloc )
    {
      this->data = new unsigned char[ sz * sizeof( TYPE ) ] ;
    }
  }

  template<typename IMPL>
  template<typename TYPE>
  void Buffer<IMPL>::copySynced( TYPE* src, unsigned amt, unsigned offset )
  {
    this->copyToDevice( src, amt, offset ) ;
    this->copyToHost  ( src, amt, offset ) ;
  }

  template<typename IMPL>
  template<typename TYPE>
  void Buffer<IMPL>::copyToHost( TYPE* src, unsigned token_amt, unsigned offset )
  {
    auto amt = token_amt > this->count ? this->count : token_amt ;
    
    static_assert( sizeof( TYPE ) == this->element_sz, "The size of the input is different than this object. Stop." ) ;

    memcpy( this->data, src, amt * this->element_sz ) ;
  }

  template<typename IMPL>
  template<typename TYPE>
  void Buffer<IMPL>::copyToDevice( TYPE* src, unsigned token_amt, unsigned offset )
  {
    auto amt = token_amt > this->count ? this->count : token_amt ;
    
    static_assert( sizeof( TYPE ) == this->element_sz, "The size of the input is different than this object. Stop." ) ;

    this->impl.copyTo( static_cast<void*>( src ), this->buff, amt ) ;
  }

  template<typename IMPL>
  const typename IMPL::Buffer& Buffer<IMPL>::buffer() const
  {
    return this->buff ;
  }
  
  template<typename IMPL>
  void Buffer<IMPL>::syncToDevice() 
  {
    this->impl.copyTo( this->data, this->buff, this->element_sz * this->count ) ;
  }
  
  template<typename IMPL>
  void Buffer<IMPL>::syncToHost() 
  {
    this->impl.copyTo( this->buff, this->data, this->element_sz * this->count ) ;
  } 

  template<typename IMPL>
  unsigned Buffer<IMPL>::size() const
  {
    return this->count * this->element_sz ;
  }
  
  template<typename IMPL>
  unsigned Buffer<IMPL>::byteSize() const
  {
    return this->count * this->element_sz ;
  }
}

//  template<typename TYPE, typename IMPL>
//  class ConstIterator
//  {
//    public:
//      typedef Buffer<IMPL> APIBuffer ;
//      
//      ConstIterator& operator=( const ConstIterator& iter ) ;
//      void seek( unsigned index ) const ;
//      const TYPE& operator*() const ;
//      unsigned index() const ;
//      ConstIterator<TYPE> begin() const ;
//      ConstIterator<TYPE> end() const ;
//      const TYPE* pointer() const ;
//      const APIBuffer& buffer() const ;
//    private:
//      friend class List ;
//      ConstIterator() ;
//      APIBuffer*       buffer       ;
//      TYPE*            data         ;
//      mutable unsigned index        ;
//      unsigned         element_size ;
//      unsigned         count        ;
//  };
//
//  template<typename TYPE, typename IMPL>
//  class Iterator
//  {
//    public:
//      typedef Buffer<IMPL> APIBuffer ;
//      
//      Iterator& operator=( const Iterator& iter ) ;
//      void seek( unsigned index ) ;
//      TYPE& operator*() ;
//      unsigned index() const ;
//      Iterator<TYPE> begin() ;
//      Iterator<TYPE> end() ;
//      TYPE* pointer() ;
//      APIBuffer& buffer() ;
//    private:
//      friend class List ;
//      Iterator() ;
//      APIBuffer* buffer       ;
//      TYPE*      data         ;
//      unsigned   index        ;
//      unsigned   element_size ;
//      unsigned   count        ;
//  };
//
//  Buffer ;
//  template<typename TYPE, typename IMPL>
//  class List
//  {
//    public:
//      typedef Buffer<IMPL> APIBuffer ;
//      
//      List() ;
//      ~List() ;
//      void allocate( unsigned device, unsigned count = 1 ) ;
//      void copy( TYPE* data ) ;
//      List( const List& list ) ;
//      Iterator<TYPE, IMPL> begin() const ;
//      ConstIterator<TYPE, IMPL> begin() const ;
//
//      Iterator<TYPE, IMPL> end() const ;
//      ConstIterator<TYPE, IMPL> end() const ;
//    private:
//      APIBuffer* buffer       ;
//      TYPE*      data         ;
//      IMPL       impl         ;
//      unsigned   element_size ;
//      unsigned   count        ;
//  };
//}
#endif 

