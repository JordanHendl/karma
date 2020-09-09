#include "Font.h"
#define TTF_FONT_PARSER_IMPLEMENTATION
#include "stb_truetype.h"
#include <vector>
#include <iostream>
#include <fstream>

namespace kgl
{
  namespace io
  {
    struct FontData
    {
      typedef std::vector<unsigned char  > Pixels ;
      typedef std::vector<stbtt_bakedchar> Codes  ;
     
      Codes           char_buff  ;
      Pixels          pixels     ;
      unsigned        width      ;
      unsigned        height     ;
      unsigned        pix_height ;
    };

    Glyph::Glyph()
    {
    
    }

    Glyph::~Glyph()
    {
    
    }

    float Glyph::topLeftX() const 
    {
      return this->top_leftx ;
    }
    
    float Glyph::topLeftY() const
    {
      return this->top_lefty ;
    }

    float Glyph::topRightX() const
    {
      return this->top_rightx ;
    }
    
    float Glyph::topRightY() const
    {
      return this->top_righty ;
    }

    float Glyph::bottomLeftX() const
    {
      return this->bottom_leftx ;
    }
    
    float Glyph::bottomLeftY() const
    {
      return this->bottom_lefty ;
    }

    float Glyph::bottomRightX() const
    {
      return this->bottom_rightx ;
    }
    
    float Glyph::bottomRightY() const
    {
      return this->bottom_righty ;
    }

    float Glyph::xpos() const
    {
      return this->x_pos ;
    }
    
    float Glyph::ypos() const
    {
      return this->y_pos ;
    }

    unsigned Glyph::value() const
    {
      return this->val ;
    }

    Font::Font()
    {
      this->loader_data = new FontData() ;
    }
    
    Font::~Font()
    {
      delete this->loader_data ;
    }
    
    void Font::reset()
    {
      data().pixels.clear() ;
    }
    
    const Glyph* Font::glyphs( const char* text, float x, float y ) const
    {
      const std::string  txt = text ;
      
      std::vector<Glyph> glyphs ;
      stbtt_aligned_quad quad   ;
      Glyph              glyph  ;
      
      for( auto ch : txt )
      {
        if(ch >= 32 && ch < 128) 
        {
          stbtt_GetBakedQuad( data().char_buff.data(), data().width, data().height, ch - 32, &x, &y, &quad, 1 ) ;

          glyph.bottom_leftx  = quad.s0 ;
          glyph.bottom_lefty  = quad.t1 ;
          glyph.bottom_rightx = quad.s1 ;
          glyph.bottom_righty = quad.t1 ;
          glyph.top_leftx     = quad.s0 ;
          glyph.top_lefty     = quad.t0 ;
          glyph.top_rightx    = quad.s1 ;
          glyph.top_righty    = quad.t0 ;
          glyph.v0            = quad.x0 ;
          glyph.v1            = quad.y0 ;
          glyph.v2            = quad.x1 ;
          glyph.v3            = quad.y1 ;
          glyph.x_pos         = x       ;
          glyph.y_pos         = y       ;
        }

        glyphs.push_back( glyph ) ;
      }
      
      return glyphs.data() ;
    }
    
    void Font::load( const char* path, unsigned resx, unsigned resy, unsigned sz )
    {
      std::ifstream               stream ;
      Glyph                       glyph  ;
      std::vector<unsigned char> file   ;
      
      stream.open( path, std::ios::binary  ) ;
      data().char_buff.resize( 300         ) ;
      data().pixels   .resize( resx * resy ) ;
      if( stream )
      {
        stream.seekg  ( 0, std::ios::end  ) ;
        file  .reserve( stream.tellg()    ) ;
        stream.seekg  ( 0, std::ios::beg  ) ;
        file.assign ( ( std::istreambuf_iterator<char>( stream ) ), std::istreambuf_iterator<char>() ) ;
        stream.close() ;
      }
      
      data().width      = resx ;
      data().height     = resy ;
      data().pix_height = sz   ;
      stbtt_BakeFontBitmap( file.data(), 0, sz, data().pixels.data(), data().width, data().height, 32, data().char_buff.size() - 1, data().char_buff.data() ) ;
    }
    
    unsigned Font::width() const
    {
      return data().width ;
    }
    
    unsigned Font::height() const
    {
      return data().height ;
    }

    const unsigned char* Font::pixels() const
    {
      return data().pixels.data() ;
    }
    
    FontData& Font::data()
    {
      return *this->loader_data ;
    }
    
    const FontData& Font::data() const
    {
      return *this->loader_data ;
    }
  }
}
