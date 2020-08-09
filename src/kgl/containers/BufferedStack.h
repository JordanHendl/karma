#ifndef KGL_DOUBLEBUFFEREDSTACK_H
#define KGL_DOUBLEBUFFEREDSTACK_H

namespace kgl
{
  namespace containers
  {
    template<typename T, unsigned LAYERS>
    class BufferedStack ;
    
    /**
     */
    class BufferedStackImpl
    {
      private:
        template<typename, unsigned> friend class BufferedStack ;
        
        /**
         */
        void initialize( unsigned layers, unsigned element_size ) ;

        /**
         */
        BufferedStackImpl() ;
        
        /**
         */
        ~BufferedStackImpl() ;
        /**
         * @return 
         */
        bool empty() ;
        
        /**
         */
        void clear() ;
        
        /**
         * @return 
         */
        unsigned size( int layer ) const ;

        /**
         */
        void swap( int layer ) ;
        
        /**
         * @param val
         * @param element_size
         */
        void insertBase( void* val, unsigned layer ) ;

        /**
         * 
         * @return 
         */
        void* popBase() ;
        
        /**
         */
        struct BufferedStackImplData* bstack_data ;
        
        /**
         * @return 
         */
        unsigned currentSwap() const ;
        
        /**
         * @return 
         */
        unsigned nextSwap() const ;

        /**
         * @return 
         */
        BufferedStackImplData& data() ;
        
        /**
         * @return 
         */
        const BufferedStackImplData& data() const ;
    };

    template<typename T, unsigned LAYERS>
    class BufferedStack
    {
      public:
        
        /**
         */
        BufferedStack() ;

        /**
         */
        ~BufferedStack() = default ;
        
        /**
         * @param val
         */
        void insert( const T& val, unsigned layer = 0 ) ;
        
        /**
         * @return 
         */
        unsigned current() const ;
        
        /**
         * @return 
         */
        unsigned next() const ;
        
        /**
         * @return 
         */
        unsigned size( int layer = -1 ) const ;

        /**
         * @return 
         */
        void clear() ;

        /**
         * @return 
         */
        T pop() ;
        
        /**
         * @return 
         */
        bool empty() ;
        
        /**
         */
        void swap( int layer = -1 ) ;
      private:
        BufferedStackImpl impl ;
    };
    
    
    template<typename T, unsigned LAYERS>
    BufferedStack<T, LAYERS>::BufferedStack()
    {
      impl.initialize( LAYERS, sizeof( T ) ) ;
    }
    
    template<typename T, unsigned LAYERS>
    void BufferedStack<T, LAYERS>::insert( const T& val, unsigned layer )
    {
      T* tmp = new T( val ) ;

      impl.insertBase( static_cast<void*>( tmp ), layer ) ;
    }

    template<typename T, unsigned LAYERS>
    unsigned BufferedStack<T, LAYERS>::next() const
    {
      return impl.nextSwap() ;
    }
    
    template<typename T, unsigned LAYERS>
    unsigned BufferedStack<T, LAYERS>::size( int layer ) const
    {
      return impl.size( layer ) ;
    }

    template<typename T, unsigned LAYERS>
    unsigned BufferedStack<T, LAYERS>::current() const
    {
      return impl.currentSwap() ;
    }
    
    template<typename T, unsigned LAYERS>
    T BufferedStack<T, LAYERS>::pop()
    {
      T  val ;
      T* ptr ;
      
      if( !this->empty() )
      {
        ptr = static_cast<T*>( impl.popBase() ) ;
        val = *ptr                              ;
        delete ptr                              ;
      }
      
      return val ;
    }
    
    template<typename T, unsigned LAYERS>
    bool BufferedStack<T, LAYERS>::empty()
    {
      return impl.empty() ;
    }

    template<typename T, unsigned LAYERS>
    void BufferedStack<T, LAYERS>::clear()
    {
      impl.clear() ;
    }
    
    template<typename T, unsigned LAYERS>
    void BufferedStack<T, LAYERS>::swap( int layer )
    {
      impl.swap( layer ) ;
    }
  }
  
  template<typename T, unsigned LAYERS>
  using BufferedStack = containers::BufferedStack<T, LAYERS> ;
}
#endif /* DOUBLEBUFFEREDSTACK_H */

