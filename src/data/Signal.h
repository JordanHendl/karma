/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   casper_signal.h
 * Author: jhendl
 *
 * Created on February 24, 2020, 1:52 AM
 */

#ifndef DATA_SIGNAL_H
#define DATA_SIGNAL_H

namespace data
{
  namespace module
  {
    class SignalType
    {
      public:
        SignalType() ;
        bool operator<( SignalType type ) const ;
      private:
        typedef SignalType (*Identifier)() ;
        Identifier ident ;
        
        SignalType( Identifier id ) ;
        
        template<class T>
        friend SignalType signalType() ;
    } ;
    
    class Signal
    {
      public:
        Signal() ;
        ~Signal() ;
        
        template<class T>
        void operator()( const T& val ) ;
        
        template<class T>
        void operator()( unsigned idx, const T& val ) ;
        
        template<class T>
        void attach( void (*method)( T ) ) ;
        
        template<class T>
        void attach( void (*method)( const T& ) ) ;
        
        template<class T>
        void attach( void (*method)( unsigned, T ) ) ;

        template<class T>
        void attach( void (*method)( unsigned, const T& ) ) ;
        
        template<class C, class T>
        void attach( C* object, void (C::*method)( const T& ) ) ;
        
        template<class C, class T>
        void attach( C* object, void (C::*method)( T ) ) ;
        
        template<class C, class T>
        void attach( C* object, void (C::*method)( unsigned, T ) ) ;
        
        template<class C, class T>
        void attach( C* object, void (C::*method)( unsigned, const T& ) ) ;
        
        template<class T>
        void emit( const T& val ) ;
        
        template<class T>
        void emit( unsigned id, const T& val ) ;
        
      private:

        class Callback
        {
          public:
            virtual ~Callback() {} 
            
            virtual void execute( const void* pointer ) = 0 ;
        };
        
        class IndexedCallback
        {
          public:
            virtual ~IndexedCallback() {} ;
            
            virtual void execute( unsigned idx, const void* pointer ) = 0 ;
        };
        
        template<class T>
        class Method : public Callback
        {
          public:
            typedef void (*Callback)( T ) ;
            
            Method( Callback method ) ;
            
            virtual void execute( const void* p ) ;
            
          private:
            Callback method ;
        };
        
        template<class T>
        class ReferenceMethod : public Callback
        {
          public:
            typedef void (*Callback)( const T& ) ;
            ReferenceMethod( Callback method ) ;
            virtual void execute( const void* pointer ) ;
          private:
            Callback method ;
        };
        
        template<class T>
        class IndexedMethod : public IndexedCallback
        {
          public:
            typedef void(*Callback)( unsigned, T ) ;
            
            IndexedMethod( Callback method ) ;
            virtual void execute( unsigned idx, const void* pointer ) ;
          private:
            Callback method ;
        };
        
        template<class T>
        class IndexedReferenceMethod : public IndexedCallback
        {
          public:
            typedef void(*Callback)( unsigned, T ) ;
            
            IndexedReferenceMethod( Callback method ) ;
            virtual void execute( unsigned idx, const void* pointer ) ;
          private:
            Callback method ;
        };
        
        template<class C, class T>
        class ObjectMethod : public Callback
        {
          public:
            typedef void (C::*Callback)( const T ) ;
          
            ObjectMethod( C* object, Callback method ) ;
            virtual void execute( const void* p ) ;
          private:
            Callback method ;
            C*       object ;
        } ;
        
        template<class C, class T>
        class IndexedObjectMethod : public IndexedCallback
        {
          public:
            typedef void (C::*Callback)( unsigned, const T ) ;
          
            IndexedObjectMethod( C* object, Callback method ) ;
            virtual void execute( unsigned idx, const void* p ) ;
          private:
            Callback method ;
        } ;
        
        template<class C, class T>
        class ObjectReferencedMethod : public Callback
        {
          public:
            typedef void (C::*Callback)( const T& ) ;
            
            ObjectReferencedMethod( C* object, Callback method ) ;
            virtual void execute( const void* p ) ;
          private:
            Callback method ;
            C*       object ;
        } ;
        
        template<class C, class T>
        class IndexedObjectReferencedMethod : public IndexedCallback
        {
          public:
            typedef void (C::*Callback)( unsigned, const T& ) ;
          
            IndexedObjectReferencedMethod( C* object, Callback method ) ;
            virtual void execute( unsigned idx, const void* p ) ;
          private:
            Callback method ;
            C*       object ;
        } ;
        
        struct SignalData* signal_data ;
        
        Signal( const Signal& cpy ) ;
        Signal& operator=( const Signal& cpy ) ;
        
        void attachBase( SignalType type, Callback* callback ) ;
        void attachBase( SignalType type, IndexedCallback* callback ) ;
        void emitBase( SignalType type, const void* ptr ) ;
        void emitBase( SignalType type, unsigned idx, const void* ptr ) ;
        
        SignalData& data() ;
        const SignalData& data() const ;
        
        friend SignalData ;
    } ;
    
    template<class T>
    SignalType signalType() ;
    
    template<class T>
    void Signal::attach( void (*method)( T ) )
    {
      typedef Signal::Callback  Callback       ;
      typedef Signal::Method<T> CallbackMethod ;
      
      attachBase( signalType<T>(), dynamic_cast<Callback*>( new CallbackMethod( method ) ) ) ;
    }

    template<class T>
    void Signal::attach( void (*method)( unsigned, T ) )
    {
      typedef Signal::IndexedCallback  Callback       ;
      typedef Signal::IndexedMethod<T> CallbackMethod ;
      
      attachBase( signalType<T>(), dynamic_cast<Callback*>( new CallbackMethod( method ) ) ) ;
    }
    
    template<class T>
    void Signal::attach( void (*method)( const T& ) )
    {
      typedef Signal::Callback           Callback       ;
      typedef Signal::ReferenceMethod<T> CallbackMethod ;
      
      attachBase( signalType<T>(), dynamic_cast<Callback*>( new CallbackMethod( method ) ) ) ;
    }
    
    template<class T>
    void Signal::attach( void (*method)( unsigned, const T& ) )
    {
      typedef Signal::IndexedCallback           Callback       ;
      typedef Signal::IndexedReferenceMethod<T> CallbackMethod ;

      attachBase( signalType<T>(), dynamic_cast<Callback*>( new CallbackMethod( method ) ) ) ;
    }
    
    template<class C, class T>
    void Signal::attach( C* object, void (C::*method)( const T& ) )
    {
      typedef Signal::Callback                     Callback       ;
      typedef Signal::ObjectReferencedMethod<C, T> CallbackMethod ;

      attachBase( signalType<T>(), dynamic_cast<Callback*>( new CallbackMethod( object, method ) ) ) ;
    }
    
    template<class C, class T>
    void Signal::attach( C* object, void (C::*method)( unsigned, const T& ) )
    {
      typedef Signal::IndexedCallback                     Callback       ;
      typedef Signal::IndexedObjectReferencedMethod<C, T> CallbackMethod ;

      attachBase( signalType<T>(), dynamic_cast<Callback*>( new CallbackMethod( object, method ) ) ) ;
    }
    
    template<class C, class T>
    void Signal::attach( C* object, void (C::*method)( T ) )
    {
      typedef Signal::Callback           Callback       ;
      typedef Signal::ObjectMethod<C, T> CallbackMethod ;

      attachBase( signalType<T>(), dynamic_cast<Callback*>( new CallbackMethod( object, method ) ) ) ;
    }
    
    template<class T>
    void Signal::emit( const T& val )
    {
      emitBase( signalType<T>(), static_cast<const void*>( &val ) ) ;
    }
    
    template<class T>
    void Signal::emit( unsigned idx, const T& val )
    {
      emitBase( signalType<T>(), idx, static_cast<const void*>( &val ) ) ;
    }
    
    template<class T>
    Signal::Method<T>::Method( Callback method )
    {
      this->method = method ;
    }
    
    template<class T>
    void Signal::Method<T>::execute( const void* p )
    {
      ( this->method )( *static_cast<const T*>( p ) ) ;
    }
    
    template <class T>
    Signal::ReferenceMethod<T>::ReferenceMethod( Callback method )
    {
      this->method = method ;
    }
    
    template<class T>
    void Signal::ReferenceMethod<T>::execute( const void* pointer )
    {
      ( this->method )( *static_cast<const T*>( pointer ) ) ;
    }
    
    template<class T>
    Signal::IndexedMethod<T>::IndexedMethod( Callback method )
    {
      this->method = method ;
    }
    
    template<class T>
    void Signal::IndexedMethod<T>::execute( unsigned idx, const void* pointer )
    {
      ( this->method )( idx, *static_cast<const T*>( pointer ) ) ;
    }
    
    template <class T>
    Signal::IndexedReferenceMethod<T>::IndexedReferenceMethod( Callback method )
    {
      this->method = method ;
    }
    
    template <class T>
    void Signal::IndexedReferenceMethod<T>::execute( unsigned idx, const void* p )
    {
      ( this->method )( idx, *static_cast<const T*>( p ) ) ;
    }
    
    template <class C, class T>
    Signal::ObjectReferencedMethod<C, T>::ObjectReferencedMethod( C* object, Callback method )
    {
      this->method = method ;
      this->object = object ;
    }
    
    template <class C, class T>
    void Signal::ObjectReferencedMethod<C, T>::execute( const void* p )
    {
      ( ( this->object )->*( this->method ) )( *static_cast<const T*>( p ) ) ;
    }
    
    template <class C, class T>
    Signal::ObjectMethod<C, T>::ObjectMethod( C* object, Callback method )
    {
      this->method = method ;
      this->object = object ;
    }
    
    template <class C, class T>
    void Signal::ObjectMethod<C, T>::execute( const void* p )
    {
      ( ( this->object )->*( this->method ) )( *static_cast<const T*>( p ) ) ;
    }
    
    template<class T>
    SignalType signalType()
    {
      return SignalType( &signalType<T> ) ;
    }
  }
}

#endif /* CASPER_SIGNAL_H */

