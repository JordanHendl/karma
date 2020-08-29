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

    float Glyph::topLeftX()
    {
      return this->top_leftx ;
    }
    
    float Glyph::topLeftY()
    {
      return this->top_lefty ;
    }

    float Glyph::topRightX()
    {
      return this->top_rightx ;
    }
    
    float Glyph::topRightY()
    {
      return this->top_righty ;
    }

    float Glyph::bottomLeftX()
    {
      return this->bottom_leftx ;
    }
    
    float Glyph::bottomLeftY()
    {
      return this->bottom_lefty ;
    }

    float Glyph::bottomRightX()
    {
      return this->bottom_rightx ;
    }
    
    float Glyph::bottomRightY()
    {
      return this->bottom_righty ;
    }

    unsigned Glyph::value()
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
    
    const Glyph* Font::glyphs( const char* text, float x, float y )
    {
      const std::string  txt = text ;
      
      std::vector<Glyph> glyphs ;
      stbtt_aligned_quad quad   ;
      Glyph              glyph  ;
      
      for( auto ch : txt )
      {
        stbtt_GetBakedQuad( data().char_buff.data(), data().width, data().height, ch, &x, &y, &quad, 1 ) ;
        
        glyph.bottom_leftx  = quad.x0 ;
        glyph.bottom_lefty  = quad.y1 ;
        glyph.bottom_rightx = quad.x1 ;
        glyph.bottom_righty = quad.y1 ;
        glyph.top_leftx     = quad.x0 ;
        glyph.top_lefty     = quad.y0 ;
        glyph.top_rightx    = quad.x1 ;
        glyph.top_righty    = quad.y0 ;
        
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
