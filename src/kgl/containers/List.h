#ifndef KGL_LIST_H
#define KGL_LIST_H

namespace kgl
{
  namespace containers
  {
    template<typename T>
    class List ;

    template<typename T>
    class ListIterator
    {
      public:
        
        /**
         */
        ListIterator() ;
        
        /**
         */
        ~ListIterator() ;

        /**
         * @return 
         */
        ListIterator begin() const ;
        
        /**
         * @return 
         */
        ListIterator end() const ;
        
        /**
         * @return 
         */
        void seek( unsigned index ) ;
        
        /**
         * @return 
         */
        unsigned size() const ;
        
        /**
         * @param iter
         * @return 
         */
        ListIterator& operator=( const ListIterator& iter ) ;
        
        /**
         * @return 
         */
        const T& operator*() const ;
        
        /**
         * @return 
         */
        T& operator*() ;
        
        /**
         * @param iter
         * @return 
         */
        bool operator!=( const ListIterator& iter ) ;
        
        /**
         * @return 
         */
        ListIterator& operator++() ;
        
        
      private:
        /**
         */
        template<typename> friend class List ;
        
        T*       data     ; ///< 
        unsigned data_sz  ; ///< 
        unsigned location ; ///< 
    };
    
    /**
     */
    template<typename T>
    class List
    {
      public:
        
        /**
         */
        List() ;
        
        /**
         * @param list
         */
        List( const List& list ) ;
        
        
        /**
         * @param size
         */
        ~List() ;
        
        /**
         */
        void clear() ;

        /**
         * @return 
         */
        T* data() ;
        
        /**
         * @param size
         */
        const T* data() const ;
        
        /**
         * @param list
         */
        void copy( const List<T>& list ) ;

        /**
         * @param size
         */
        void initialize( unsigned size ) ;

        /**
         * @param val
         */
        void insert( T val, unsigned index ) ;
        
        /**
         * @return 
         */
        ListIterator<T> begin() ;
        
        /**
         * @return 
         */
        ListIterator<T> begin() const ;
        
        /**
         * @param val
         */
        void fill( T val ) ;
        
        /**
         * 
         * @return 
         */
        unsigned size() const ;

        /**
         * @return 
         */
        ListIterator<T> end() ;
        
        /**
         * @return 
         */
        ListIterator<T> end() const ;
        
        /**
         * @param index
         * @return 
         */
        T& operator[]( unsigned index ) ;
        
        /**
         * @param index
         * @return 
         */
        const T& operator[]( unsigned index ) const ;
        
        /**
         * @param list
         * @return 
         */
        List& operator=( const List& list ) ;

      private:
        T*       data_ptr ;
        unsigned sz       ;
    };
    
    template<typename T>
    ListIterator<T>& ListIterator<T>::operator=( const ListIterator& iter )
    {
      this->data     = iter.data     ;
      this->location = iter.location ;
      this->data_sz  = iter.data_sz  ;

      return *this ;
    }
    
    template<typename T>
    bool ListIterator<T>::operator!=( const ListIterator& iter )
    {
      return ( this->location != iter.location || this->data != iter.data ) ;
    }

    template<typename T>
    unsigned ListIterator<T>::size() const 
    {
      return this->data_sz ;
    }

    template<typename T>
    ListIterator<T> ListIterator<T>::begin() const
    {
      ListIterator<T> iter ;
      
      iter.data     = this->data    ;
      iter.data_sz  = this->data_sz ;
      iter.location = 0             ;
      
      return iter ;
    }

    template<typename T>
    ListIterator<T> ListIterator<T>::end() const
    {
      ListIterator<T> iter ;
      
      iter.data     = this->data    ;
      iter.data_sz  = this->data_sz ;
      iter.location = this->data_sz ;
    }

    template<typename T>
    void ListIterator<T>::seek( unsigned index )
    {
      this->location = location < this->data_sz ? location : this->data_sz ;
    }

    template<typename T>
    ListIterator<T>& ListIterator<T>::operator++()
    {
      this->location++ ;
      return *this ;
    }
    
    template<typename T>
    T& ListIterator<T>::operator*()
    {
      return this->location < this->data_sz ? ( *( this->data + this->location ) ) :  this->data[ 0 ] ;
    }
    
    template<typename T>
    const T& ListIterator<T>::operator*() const
    {
      return this->location < this->data_sz ? ( *( this->data + this->location ) ) :  this->data[ 0 ] ;
    }

    template<typename T>
    ListIterator<T>::~ListIterator()
    {
      this->data = nullptr ;
    }
    
    template<typename T>
    ListIterator<T>::ListIterator()
    {
      this->data     = nullptr ;
      this->location = 0       ;
      this->data_sz     = 0    ;
    }

    template<typename T>
    List<T>::List()
    {
      this->data_ptr = nullptr ;
    }

    template<typename T>
    List<T>::List( const List& list )
    {
      this->copy( list ) ;
    }

    template<typename T>
    List<T>::~List()
    {
      this->clear() ;
    }
    
    template<typename T>
    void List<T>::clear()
    {
      delete this->data_ptr ;
      this->sz = 0 ;
    }

    template<typename T>
    T* List<T>::data()
    {
      return this->data_ptr ;
    }
    
    template<typename T>
    const T* List<T>::data() const
    {
      return this->data_ptr ;
    }
    
    template<typename T>
    unsigned List<T>::size() const
    {
      return this->sz ;
    }
    
    template<typename T>
    void List<T>::copy( const List<T>& list )
    {
      if( this->sz != list.sz )
      {
        if( this->data_ptr != nullptr )
        {
          delete this->data_ptr ;
        }
        
        this->data_ptr = new T[ list.sz ] ;
        this->sz       = list.sz          ;
      }
      
      for( unsigned i = 0; i < list.sz; i++ ) this->data_ptr[ i ] = list.data_ptr[ i ] ;
    }
    
    template<typename T>
    void List<T>::initialize( unsigned size )
    {
      this->data_ptr = new T[ size ] ;
      this->sz       = size          ;
    }
    
    template<typename T>
    void List<T>::insert( T val, unsigned index )
    {
      if( index < this->sz ) this->data_ptr[ index ] = val ;
    }
    
    template<typename T>
    ListIterator<T> List<T>::begin()
    {
      ListIterator<T> iter ;
      
      iter.data     = this->data_ptr ;
      iter.location = 0              ;
      iter.data_sz     = this->sz    ;
      
      return iter ;
    }
    
    template<typename T>
    ListIterator<T> List<T>::begin() const 
    {
      ListIterator<T> iter ;
      
      iter.data     = this->data_ptr ;
      iter.location = 0              ;
      iter.data_sz     = this->sz    ;
      
      return iter ;
    }
    
    template<typename T>
    ListIterator<T> List<T>::end()
    {
      ListIterator<T> iter ;
      
      iter.data     = this->data_ptr ;
      iter.location = this->sz       ;
      iter.data_sz     = this->sz       ;
      
      return iter ;
    }
    
    template<typename T>
    ListIterator<T> List<T>::end() const
    {
      ListIterator<T> iter ;
      
      iter.data     = this->data_ptr ;
      iter.location = this->sz       ;
      iter.data_sz     = this->sz       ;
      
      return iter ;
    }
    
    template<typename T>
    void List<T>::fill( T val )
    {
      for( unsigned i = 0; i < this->sz; i++ )
      {
        this->data_ptr[ i ] = val ;
      }
    }

    template<typename T>
    T& List<T>::operator[]( unsigned index )
    {
      return index < this->sz ? this->data_ptr[ index ] : this->data_ptr[ 0 ] ;
    }
    
    template<typename T>
    const T& List<T>::operator[]( unsigned index ) const
    {
      return index < this->sz ? this->data_ptr[ index ] : this->data_ptr[ 0 ] ;
    }
    
    template<typename T>
    List<T>& List<T>::operator=( const List& list )
    {
      this->copy( list ) ;

      return *this ;
    }
  }
  
  template<typename T>
  using List         = containers::List<T> ;
  
  template<typename T>
  using ListIterator = containers::ListIterator<T> ;
}
#endif /* LIST_H */

