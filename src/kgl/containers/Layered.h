/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Buffered.h
 * Author: jhendl
 *
 * Created on August 10, 2020, 1:43 PM
 */

#ifndef KGL_LAYERED_H
#define KGL_LAYERED_H

namespace kgl
{
  namespace containers
  {
    template<class TYPE, unsigned CHANNELS>
    class Layered
    {
      public:
        /**
         */
        Layered() ;
        
        /**
         * @return 
         */
        ~Layered() ;
        
        /**
         * @return 
         */
        TYPE& value() ;
        
        /**
         * @return 
         */
        const TYPE& value() const ;
        
        /**
         * @param index
         * @return 
         */
        TYPE& seek( unsigned index ) ;
        
        /**
         * @param index
         * @return 
         */
        const TYPE& seek( unsigned index ) const ;
        
        /**
         * @return 
         */
        unsigned current() const ;
        
        /**
         * @return 
         */
        unsigned next() const ;
        
        /**
         */
        void swap() ;
      private:
        unsigned index ;
        TYPE data[ CHANNELS + 1 ] ;
    };
    
    template<class TYPE, unsigned CHANNELS>
    Layered<TYPE, CHANNELS>::Layered()
    {
      this->index = 0 ;
    }

    template<class TYPE, unsigned CHANNELS>
    Layered<TYPE, CHANNELS>::~Layered()
    {
      
    }
    
    template<class TYPE, unsigned CHANNELS>
    TYPE& Layered<TYPE, CHANNELS>::value()
    {
      return this->data[ this->index ] ;
    }
    
    template<class TYPE, unsigned CHANNELS>
    const TYPE& Layered<TYPE, CHANNELS>::value() const
    {
      return this->data[ this->index ] ;
    }
    
    template<class TYPE, unsigned CHANNELS>
    TYPE& Layered<TYPE, CHANNELS>::seek( unsigned index )
    {
      return index < CHANNELS ? this->data[ index ] : this->data[ CHANNELS ] ;
    }
    
    template<class TYPE, unsigned CHANNELS>
    const TYPE& Layered<TYPE, CHANNELS>::seek( unsigned index ) const
    {
      return index < CHANNELS ? this->data[ index ] : this->data[ CHANNELS ] ;
    }
    
    template<class TYPE, unsigned CHANNELS>
    unsigned Layered<TYPE, CHANNELS>::current() const
    {
      return this->index ;
    }
    
    template<class TYPE, unsigned CHANNELS>
    unsigned Layered<TYPE, CHANNELS>::next() const
    {
      return this->index + 1 >= CHANNELS ? 0 : this->index + 1 ;
    }
    
    template<class TYPE, unsigned CHANNELS>
    void Layered<TYPE, CHANNELS>::swap()
    {
       this->index = this->next() ;
    }
  }
}

#endif /* BUFFERED_H */

