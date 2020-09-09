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
        float topLeftX() const ;
        float topLeftY() const ;
        float topRightX() const ;
        float topRightY() const ;
        float bottomLeftX() const ;
        float bottomLeftY() const ;
        float bottomRightX() const ;
        float bottomRightY() const ;
        float xpos() const ;
        float ypos() const ;
        unsigned value() const ;
        
        float v0 ;
        float v1 ;
        float v2 ;
        float v3 ;
      private:
        friend class Font ;
        unsigned val           ;
        float    top_leftx     ;
        float    top_lefty     ;
        float    top_rightx    ;
        float    top_righty    ;
        float    bottom_leftx  ;
        float    bottom_lefty  ;
        float    bottom_rightx ;
        float    bottom_righty ;
        float    x_pos         ;
        float    y_pos         ;
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
        const Glyph* glyphs( const char* text, float x, float y ) const ;
        
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

