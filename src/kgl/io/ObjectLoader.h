#ifndef KGL_IO_OBJECT_LOADER_H
#define KGL_IO_OBJECT_LOADER_H

namespace kgl
{
  namespace io
  {
    template<class RETURN, class ... ARGS>
    class Symbol
    {
      public:

        Symbol() {} ;

        ~Symbol() {} ;

        RETURN operator()( ARGS ... args )
        {
          return ( this->func )( args... ) ;
        }
        Symbol& operator=( void (*func)() )
        {
          this->func = reinterpret_cast<RETURN(*) (ARGS ... )>( func ) ;
          return *this ;
        };

      private:
        RETURN (*func)(ARGS ... args ) ;
    };

    class ObjectLoader
    {
      public: 
        typedef void (*DL_FUNC)() ;
        ObjectLoader() ;
        ~ObjectLoader() ;
        void load( const char* lib_path ) ;
        DL_FUNC symbol( const char* symbol_name ) ;
      private:
        struct ObjectLoaderData *loader_data ;
        ObjectLoaderData& data() ;
        const ObjectLoaderData& data() const ;
    };
  }
}

#endif