/**********************************************************************
 * Copyright (C) 2020 Jordan Hendl - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the <INSERT_LICENSE>
 *
 * You should have received a copy of the <INSERT_LICENSE> license with
 * this file. If not, please write to: jordiehendl@gmail.com.
 * Author       : Jordan Hendl
 * File         : Bus.h
 * Version      : 1.0
 * Date created : 6/22/2020
 * Purpose      : Declaration for Data bus object.
**********************************************************************/

#ifndef DATA_BUS_H
#define DATA_BUS_H

namespace data
{
  namespace module
  {
    /** Forward Declared class to handle data transfer.
     */
    class Signal ;
    class String ;

    void operator<<( String& first, const char* second ) ;
    void operator<<( String& first, unsigned    second ) ;
    void operator<<( String& first, float       second ) ;
    void operator<<( String& first, const String& second ) ;

    class String
    {
      public:
        String() ;
        String( const String& string ) ;
        void operator=( const String& string ) ;
        ~String() ;
        void setStr( const char* str ) ;
        const char* str() const ;
      private:
        struct StringData* string_data ;
        StringData& data() ;
        const StringData& data() const ;
    };

    template<typename T>
    String concatenate( T type )
    {
      String str ;
      
      str << type ;
      return str ;
    }

    template<typename T, typename... TYPES>
    String concatenate( T first, TYPES... types )
    {
      String str ;
      
      str << first ;
      str << concatenate( types... ) ;
      return str ;
    }

   
    /** Class to handle data transfer between modules.
     * @note This object hashes the type information, which can have collisions.
     * 
     *       E.g.  bus[ "output" ].attach<unsigned>     ( &getterFunction  ) ;
     *             bus[ "output" ].attach<CustomObject1>( &getterFunction2 ) ;
     *             bus[ "output" ].attach<CustomObject2>( &getterFunction3 ) ;
     * 
     *   There is a possibility that the types unsigned, CustomObject1 & CustomObject2 have a collision. 
     *   This would cause all 3 functions to recieve all 3 types, if there is a collision. 
     * 
     *   CRC32 hash is used for the typing so the likeliness of a collision is low, but mass subscription to the same key with many different
     *   types may cause problems.
     */
    class Bus
    {
      public:

        /** Default constructor. Initializes this object's data.
         * @param id The channel to associate with this event bus.
         */
        Bus( unsigned id = 0 ) ;

        /** Operator overload to recieve a reference to a signal of the input key.
         * @param key The key to recieve a reference to a signal for.
         * @return Signal& Reference to the signal cooresponding to the input key.
         */
        template< typename T, typename ... KEYS>
        Signal& operator()( T first, KEYS... args ) ;

        /** Operator overload to recieve a const-reference to a signal of the input key.
         * @param key The key to recieve a const-reference to a signal for.
         * @return Signal& Const-Reference to the signal cooresponding to the input key.
         */
        template< typename T, typename ... KEYS>
        const Signal& operator()( T first, KEYS... args ) const ;

        /** Method to recieve the channel this Bus is on.
         * @return unsigned The channel this bus is sending data on.
         */
        unsigned id() ;

        /** Method to set the channel this object uses for data transfer.
         * @note All objects subscribed to this channel can interact with others ONLY in the same channel.
         * @param id The channel to use for data transfer.
         */
        void setChannel( unsigned id ) ;
        
      private:
        
        /**
         * @param key
         * @return 
         */
        const Signal& getBase( const char* key ) const ;
        
        /**
         * @param key
         * @return 
         */
        Signal& getBase( const char* key ) ;

        /** The identifier for this object.
         */
        unsigned identifier ;

        /** Copying is disallowed.
         */
        Bus( const Bus& cpy ) ;

        /** Copying is disallowed.
         */
        Bus& operator=( const Bus& cpy ) ;
    };
    
    template<typename T, typename... KEYS>
    Signal& Bus::operator()( T first, KEYS... args )
    {
      String key = ::data::module::concatenate( first, args... ) ;
      return this->getBase( key.str() ) ;
    }

    template<typename T, typename... KEYS>
    const Signal& Bus::operator()( T first, KEYS... args ) const
    {
      String key = ::data::module::concatenate( first, args... ) ;
      return this->getBase( key.str() ) ;
    }
  }
}

#endif

