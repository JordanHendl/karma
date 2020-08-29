#ifndef KGLIOFONT_H
#define KGLIOFONT_H

namespace kgl
{
  namespace io
  { 
    class Font ;
    
    /** Object for encapsulating a character from the font map.
     */
    class Glyph
    {
      public:
        Glyph() ;
        ~Glyph() ;
        unsigned advance() ;
        float topLeftX() ;
        float topLeftY() ;
        float topRightX() ;
        float topRightY() ;
        float bottomLeftX() ;
        float bottomLeftY() ;
        float bottomRightX() ;
        float bottomRightY() ;
        unsigned value() ;
      private:
        friend class Font ;
        unsigned adv           ;
        unsigned val           ;
        float    top_leftx     ;
        float    top_lefty     ;
        float    top_rightx    ;
        float    top_righty    ;
        float    bottom_leftx  ;
        float    bottom_lefty  ;
        float    bottom_rightx ;
        float    bottom_righty ;
    };

    /**
     */
    class Font
    {
      public:
        
        /**
         */
        Font() ;
        
        /**
         * @param path
         */
        ~Font() ;
        
        /**
         * @return 
         */
        void load( const char* path, unsigned resx, unsigned resy, unsigned size ) ;
        
        /**
         * @return 
         */
        const Glyph* glyphs( const char* text, float x, float y ) ;
        
        /**
         */
        void reset() ;
        
        /**
         * @return 
         */
        unsigned width() const ;
        
        /**
         * @return 
         */
        unsigned height() const ;
        
        /**
         * @return 
         */
        const unsigned char* pixels() const ;
      private:
        
        /**
         */
        struct FontData* loader_data ;
        
        /**
         * @return 
         */
        FontData& data() ;
        
        /**
         * @return 
         */
        const FontData& data() const ;
    };
  }
}

#endif /* IMAGELOADER_H */

