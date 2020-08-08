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
#ifndef KARMA_SHADER_COMPILER_H
#define KARMA_SHADER_COMPILER_H

namespace tools
{
  namespace shader
  {
    class UWUShader ;

    /**
     */
    enum ShaderStage : unsigned
    {
      VERTEX,
      FRAGMENT,
      GEOMETRY,
      TESSALATION_C,
      TESSELATION_E,
      COMPUTE,
    };

    /**
     */
    enum UniformType : unsigned
    {
      None,
      UBO,
      SAMPLER,
      IMAGE,
      SSBO
    };

    /**
     */
    class ShaderIterator
    {
      public:
        /**
         */
        ShaderIterator() ;

        /**
         * @param input 
         */
        ShaderIterator( const ShaderIterator& input ) ;

        /**
         */
        ~ShaderIterator() ;

        /**
         * @return unsigned 
         */
        unsigned numUniforms() const ;
        
        /**
         * @return 
         */
        unsigned numAttributes() const ;
        
        /**
         * @param index
         * @return 
         */
        const char* attributeName( unsigned index ) ;
        
        /**
         * @param index
         * @return 
         */
        unsigned attributeLocation( unsigned index ) ;
        
        /**
         * @param index
         * @return 
         */
        unsigned attributeByteSize( unsigned index ) ;
        
        /**
         * @return 
         */
        bool attributeIsInput( unsigned index ) ;

        /**
         * @return 
         */
        const char* attributeType( unsigned index ) ;
        
        /**
         * @return const unsigned* 
         */
        const unsigned* spirv() const ;

        /**
         * @return unsigned 
         */
        unsigned spirvSize() const ;

        /**
         * @return ShaderStage 
         */
        ShaderStage stage() const ;

        /**
         * @param id 
         * @return UniformType 
         */
        UniformType uniformType( unsigned id ) const ;

        /**
         * @param id 
         * @return unsigned 
         */
        unsigned uniformSize( unsigned id ) const ;

        /**
         * @param id 
         * @return unsigned 
         */
        unsigned uniformBinding( unsigned id ) const ;

        /**
         * @param id 
         * @return const char* 
         */
        const char* uniformName( unsigned id ) const ;

        /**
         */
        void operator++() ;

        /**
         * @param input 
         */
        void operator=( const ShaderIterator& input ) ;

        /**
         * @param input 
         * @return true 
         * @return false 
         */
        bool operator!=( const ShaderIterator& input ) ;
      private:

        /**
         */
        struct ShaderIteratorData *shader_iterator_data ;

        /**
         * @return ShaderIteratorData& 
         */
        ShaderIteratorData& data() ;

        /**
         * @return const ShaderIteratorData& 
         */
        const ShaderIteratorData& data() const  ;

        /**
         */
        friend class UWUShader ;
    };

    /**
     */
    class UWUShader
    {
      public:
        /**
         */
        UWUShader() ;

        /**
         */
        ~UWUShader() ;

        /**
         * @param path 
         */
        void load( const char* path ) ;

        /**
         * @return ShaderIterator 
         */
        ShaderIterator begin() const ;

        /**
         * @return ShaderIterator 
         */
        ShaderIterator end() const ;

        /**
         * @return unsigned 
         */
        unsigned size() const ;
      private:

        /**
         */
        struct UWUShaderData* compiler_data ;

        /**
         * @return UWUShaderData& 
         */
        UWUShaderData& data() ;

        /**
         * @return const UWUShaderData& 
         */
        const UWUShaderData& data() const ;
    };
  }
}
#endif