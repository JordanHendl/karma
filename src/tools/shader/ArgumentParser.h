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
#ifndef TOOLS_SHADER_ARG_PARSER_H
#define TOOLS_SHADER_ARG_PARSER_H

namespace tools
{
  namespace shader
  {
    /** TODO
     */
    class ArgumentParser
    {
      public:
      
        /**
         */
        ArgumentParser() ;

        /**
         */
        ~ArgumentParser() ;

        /**
         * @param argc 
         * @param argv 
         */
        void parse( int argc, const char** argv ) ;

        /**
         * @return unsigned 
         */
        unsigned getNumberOfInputs() const ;

        /**
         * @return true 
         * @return false 
         */
        bool recursive() const ;

        /**
         * @return const char* 
         */
        const char* recursionDirectory() const ;
        
        /**
         * @return 
         */
        bool verbose() const ;
        

        /**
         * @param index 
         * @return const char* 
         */
        const char* getFilePath( unsigned index ) const ;
        
        /**
         * @param index
         * @return 
         */
        const char* output() const ;

        /**
         * @return const char* 
         */
        const char* usage() const ;

        /**
         * @param index 
         * @return int 
         */
        int getShaderType( unsigned index ) const ;

        /**
         * @return true 
         * @return false 
         */
        bool valid() const ;
      private:

        /**
         */
        struct ArgParserData* arg_data ;

        /**
         * @return ArgParserData& 
         */
        ArgParserData& data() ;

        /**
         * @return const ArgParserData& 
         */
        const ArgParserData& data() const ;
    };
  }
}
#endif