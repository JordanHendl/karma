#include "Animation2D.h"
#include "../cmd/DrawCommand.h"
#include "../../profiling/Timer.h"
#include <vector>
#include <string>

namespace kgl
{
  namespace anim
  {
    const float DEFAULT_FRAME_TIME = 83.333 ;
    
    typedef struct std::vector<kgl::SheetCommand> SpriteCommands ;
    typedef struct std::vector<kgl::ImageCommand> ImageCommands  ;
    
    struct Sprite2DData
    {
      SpriteCommands   cmds       ;
      karma::ms::Timer timer      ;
      float            frame_time ;
      std::string      sprite     ;
      float            sizex      ;
      float            sizey      ;
      
      Sprite2DData() ;
    };
    
    struct Image2DData
    {
      ImageCommands    cmds       ;
      karma::ms::Timer timer      ;
      float            frame_time ;
      float            sizex      ;
      float            sizey      ;
      
      Image2DData() ;
    };
    
    Sprite2DData::Sprite2DData()
    {
      this->frame_time = DEFAULT_FRAME_TIME ;
      this->sprite     = ""                 ;
      this->sizex      = 0.0f               ;
      this->sizey      = 0.0f               ;
      
      this->timer.initialize( "" ) ;
    }
    
    Image2DData::Image2DData()
    {
      this->frame_time = DEFAULT_FRAME_TIME ;
      this->sizex      = 0.0f               ;
      this->sizey      = 0.0f               ;
      
      this->timer.initialize( "" ) ;
    }
    
    Sprite2D::Sprite2D()
    {
      this->anim2d_data = new Sprite2DData() ;
    }

    Sprite2D::Sprite2D( const Sprite2D& anim )
    {
      this->anim2d_data = new Sprite2DData() ;
      
      *this->anim2d_data = *anim.anim2d_data ;
    }

    Sprite2D::~Sprite2D()
    {
      delete this->anim2d_data ;
    }

    Image2D::Image2D()
    {
      this->anim2d_data = new Image2DData() ;
    }

    Image2D::Image2D( const Image2D& anim )
    {
      this->anim2d_data = new Image2DData() ;
      
      *this->anim2d_data = *anim.anim2d_data ;
    }

    Image2D::~Image2D()
    {
      delete this->anim2d_data ;
    }

    Sprite2D& Sprite2D::operator=( const Sprite2D& anim )
    {
      *this->anim2d_data = *anim.anim2d_data ;
      
      return *this ;
    }

    Image2D& Image2D::operator=( const Image2D& anim )
    {
      *this->anim2d_data = *anim.anim2d_data ;
      
      return *this ;
    }

    const SheetCommand& Sprite2D::current( float posx, float posy, float posz, float rotation )
    {
      const float    current_time = data().timer.time()                                                            ;
      const unsigned index        = static_cast<unsigned>( current_time / data().frame_time ) % data().cmds.size() ;
      
      data().cmds[ index ].setPosX    ( posx     ) ;
      data().cmds[ index ].setPosY    ( posy     ) ;
      data().cmds[ index ].setRotation( rotation ) ;
      
      return data().cmds[ index ] ;
    }

    const ImageCommand& Image2D::current( float posx, float posy, float posz, float rotation )
    {
      const float    current_time = data().timer.time()                                                            ;
      const unsigned index        = static_cast<unsigned>( current_time / data().frame_time ) % data().cmds.size() ;
      
      data().cmds[ index ].setPosX    ( posx     ) ;
      data().cmds[ index ].setPosY    ( posy     ) ;
      data().cmds[ index ].setRotation( rotation ) ;
      
      return data().cmds[ index ] ;
    }

    void Sprite2D::initialize( const char* sprite_sheet_name, float sizex, float sizey, float frame_time )
    {
      data().sprite     = sprite_sheet_name ;
      data().sizex      = sizex             ;
      data().sizey      = sizey             ;
      data().frame_time = frame_time        ;
    }

    void Image2D::initialize( float sizex, float sizey, float frame_time )
    {
      data().sizex      = sizex             ;
      data().sizey      = sizey             ;
      data().frame_time = frame_time        ;
    }

    void Sprite2D::insertAnimation( unsigned anim_index )
    {
      kgl::SheetCommand cmd ;
      
      cmd.setSheet ( data().sprite.c_str() ) ;
      cmd.setIndex ( anim_index            ) ;
      cmd.setWidth ( data().sizex          ) ;
      cmd.setHeight( data().sizey          ) ;
      
      data().cmds.push_back( cmd ) ;
    }

    void Image2D::insertAnimation( const char* image_name )
    {
      kgl::ImageCommand cmd ;
      
      cmd.setImage ( image_name   ) ;
      cmd.setWidth ( data().sizex ) ;
      cmd.setHeight( data().sizey ) ;
      
      data().cmds.push_back( cmd ) ;
    }

    void Sprite2D::reset()
    {
      data().timer.stop() ;
    }

    void Image2D::reset()
    {
      data().timer.stop() ;
    }

    void Sprite2D::start()
    {
      data().timer.start() ;
    }

    void Image2D::start()
    {
      data().timer.start() ;
    }

    void Sprite2D::pause()
    {
      data().timer.pause() ;
    }

    void Image2D::pause()
    {
      data().timer.pause() ;
    }

    void Sprite2D::clear()
    {
      data().cmds.clear() ;
    }

    void Image2D::clear()
    {
      data().cmds.clear() ;
    }

    Sprite2DData& Sprite2D::data()
    {
      return *this->anim2d_data ;
    }

    Image2DData& Image2D::data()
    {
      return *this->anim2d_data ;
    }

    const Sprite2DData& Sprite2D::data() const
    {
      return *this->anim2d_data ;
    }

    const Image2DData& Image2D::data() const
    {
      return *this->anim2d_data ;
    }
  }
}

