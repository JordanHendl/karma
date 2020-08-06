#ifndef KGL_DRAWCOMMAND_H
#define KGL_DRAWCOMMAND_H

namespace kgl
{
  class ImageCommand
  {
    public:
      ImageCommand() ;
      ImageCommand( const ImageCommand& cmd ) ;
      ImageCommand& operator=( const ImageCommand& cmd ) ;
      ~ImageCommand() ;
      void setImage( const char* name ) ;
      void setPosX( float pos ) ;
      void setPosY( float pos ) ;
      void setWidth( float width ) ;
      void setHeight( float height ) ;
      void setRotation( float rotation ) ;
      float posX() const ;
      float posY() const ;
      float width() const ;
      float height() const ;
      float rotation() const ;
      const char* image() const ;
    private:
      struct ImageCommandData* img_cmd_data ;
      ImageCommandData& data() ;
      const ImageCommandData& data() const ;
  };

  class SheetCommand
  {
    public:
      SheetCommand() ;
      ~SheetCommand() ;
      void setImage( const char* name ) ;
      void setPosX( float pos ) ;
      void setPosY( float pos ) ;
      void setWidth( float width ) ;
      void setHeight( float height ) ;
      void setRotation( float rotation ) ;
      void setIndex( unsigned index ) ;
      float posX() const ;
      float posY() const ;
      float width() const ;
      float height() const ;
      float rotation() const ;
      const char* image() const ;
      unsigned index() const ;
    private:
      struct SheetCommandData* img_cmd_data ;
      SheetCommandData& data() ;
      const SheetCommandData& data() const ;
  };
}
#endif

