/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   casper_bus.h
 * Author: jhendl
 *
 * Created on February 24, 2020, 1:52 AM
 */

#ifndef DATA_BUS_H
#define DATA_BUS_H

namespace data
{
  namespace module
  {
    class Signal ;
    class Bus
    {
      public:
        Bus( unsigned id = 0 ) ;
        Signal& operator[]( const char* key ) ;
        const Signal& operator[]( const char* key ) const ;
        Signal& get( const char* key ) ;
        const Signal& get( const char* key ) const ;
        unsigned id() ;
        void setId( unsigned id ) ;
        
      private:
        
        unsigned identifier ;
        Bus( const Bus& cpy ) ;
        Bus& operator=( const Bus& cpy ) ;
    };
  }
}

#endif /* CASPER_BUS_H */

