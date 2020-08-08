/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   UWUWriter.h
 * Author: jhendl
 *
 * Created on August 7, 2020, 9:34 PM
 */

#ifndef UWUWRITER_H
#define UWUWRITER_H
namespace tools
{
  enum ShaderStage : unsigned
  {
    VERTEX,
    FRAGMENT,
    GEOMETRY,
    TESSALATION_C,
    TESSELATION_E,
    COMPUTE,
  };
  
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
  class UWUWriter
  {
    

    public:
      /**
       */
      UWUWriter() ;

      /**
       */
      ~UWUWriter() ;

      /**
       * @param stage 
       * @param data 
       */
      void compile( ShaderStage stage, const char* data ) ;

      /**
       * @param path 
       */
      void save( const char* path ) ;

      /**
       * @param include_dir 
       */
      void setIncludeDirectory( const char* include_dir ) ;

      /**
       * @return unsigned 
       */
      unsigned size() const ;

    private:

      /**
       */
      struct UWUWriterData* compiler_data ;

      /**
       * @return UWUWriterData& 
       */
      UWUWriterData& data() ;

      /**
       * @return const UWUWriterData& 
       */
      const UWUWriterData& data() const ;
  };
}
#endif /* UWUWRITER_H */

