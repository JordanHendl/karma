#ifndef KGL_IO_OBJECT_LOADER_H
#define KGL_IO_OBJECT_LOADER_H

namespace kgl
{
  namespace io
  {
    /**
     */
    template<class RETURN, class ... ARGS>
    class Symbol
    {
      public:
        
        /**
         */
        Symbol() {} ;
        
        /**
         */
        ~Symbol() {} ;
        
        /**
         * @param args
         * @return 
         */
        RETURN operator()( ARGS ... args ) ;
        
        /**
         * @param func
         * @return 
         */
        Symbol& operator=( void (*func)() ) ;

      private:
        RETURN (*func)(ARGS ... args ) ;
    };

    /**
     */
    class ObjectLoader
    {
      public: 
        typedef void (*DL_FUNC)() ;
        
        /**
         */
        ObjectLoader() ;
        
        /**
         * @param lib_path
         */
        ~ObjectLoader() ;
        
        /**
         * @param lib_path
         */
        void load( const char* lib_path ) ;
        
        /**
         */
        void reset() ;

        /**
         * @param symbol_name
         * @return 
         */
        DL_FUNC symbol( const char* symbol_name ) ;
      private:
        
        /**
         */
        struct ObjectLoaderData *loader_data ;
        
        /**
         * @return 
         */
        ObjectLoaderData& data() ;
        
        /**
         * @return 
         */
        const ObjectLoaderData& data() const ;
    };
    
    template<class RETURN, class ... ARGS>
    RETURN Symbol<RETURN, ARGS... >::operator()( ARGS ... args )
    {
      return ( this->func )( args... ) ;
    }
    
    template<class RETURN, class ... ARGS>
    Symbol<RETURN, ARGS...>& Symbol<RETURN, ARGS...>::operator=( void (*func)() )
    {
      this->func = reinterpret_cast<RETURN(*) (ARGS ... )>( func ) ;
      return *this ;
    }
  }
}

#endif