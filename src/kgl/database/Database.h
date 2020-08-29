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
#ifndef KARMA_DATABASE_H
#define KARMA_DATABASE_H

namespace kgl
{
  namespace database
  {
    /**
     */
    class Database
    {
      public:
        /**
         */
        Database() ;

        /**
         */
        ~Database() ;
        
        /**
         * @param name
         */
        void loadFont( const char* name ) ;
        
        /**
         * @param name
         */
        void loadImage( const char* name ) ;
        
        /**
         * @param name
         */
        void loadSheet( const char* name ) ;
        
        /**
         * @param id 
         */
        void subscribe( const char* base_path, unsigned id ) ;

        /**
         */
        void initialize() ;

      private:

        /**
         */
        struct DatabaseData* db_data ;

        /**
         * @return DatabaseData& 
         */
        DatabaseData& data() ;

        /**
         * @return const DatabaseData& 
         */
        const DatabaseData& data() const ;
    };
  }
}
#endif