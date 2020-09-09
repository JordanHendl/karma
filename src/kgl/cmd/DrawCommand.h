#ifndef KGL_DRAWCOMMAND_H
#define KGL_DRAWCOMMAND_H

namespace kgl
{
  /** Object to contain a command to draw an image. 
   * Used for interaction with the KGL library.
   */
  class ImageCommand
  {
    public:
      
      /** Default Constructor
       */
      ImageCommand() ;
      
      /** Copy constructor. Copies the parameters from the input.
       * @param cmd The command to copy.
       */
      ImageCommand( const ImageCommand& cmd ) ;
      
      /** Equivalence operator. Copies the parameters from the input command.
       * @param cmd The command to copy parameters from.
       */
      ImageCommand& operator=( const ImageCommand& cmd ) ;
      
      /** Default deconstructor. 
       */
      ~ImageCommand() ;
      
      /** Method to set the name of image to be drawn by this command.
       * @note The image name correlates to the images loaded by the current krender loaded.
       * @param name The name of image to be drawn.
       */
      void setImage( const char* name ) ;
      
      /** Method to set the X position in pixels to draw this image.
       * @note This is assuming the top-left of the image is ( 0, 0 ).
       * @@param pos The X location to draw the image at.
       */
      void setPosX( float pos ) ;
      
      /** Method to set the Y position in pixels to draw this image.
       * @note This is assuming the top-left of the image is ( 0, 0 ).
       * @@param pos The Y location to draw the image at.
       */
      void setPosY( float pos ) ;
      
      /** Method to set the width to draw the image at.
       * @note If left unset, draws the image at it's original width.
       * @param width The width, in pixels, to draw the image at.
       */
      void setWidth( float width ) ;
      
      /** Method to set the height to draw the image at.
       * @note If left unset, draws the image at it's original height.
       * @param height The height, in pixels, to draw the image at.
       */
      void setHeight( float height ) ;
      
      /** Method to set the degree of rotation of the image to be drawn.
       * @note If left unset, draws the image as if there is no rotation, aka, as it is loaded.
       * @param rotation The 0-360 degrees of rotation to use when rendering the image.
       */
      void setRotation( float rotation ) ;
      
      /** Method to retrieve the X-position of this command.
       * @return The X-postion of this command.
       */
      float posX() const ;
      
      /** Method to retrieve the Y-position of this command.
       * @return The Y-postion of this command.
       */
      float posY() const ;
      
      /** Method to retrieve the width of this draw command.
       * @return The width of this draw command. If unset, returns 0.
       */
      float width() const ;
      
      /** Method to retrieve the height of this draw command.
       * @return The height of this draw command. If unset, returns 0.
       */
      float height() const ;
      
      /** Method to retrieve the degrees of rotation of this command.
       * @return The 0-360 rotation of this draw command.
       */
      float rotation() const ;
      
      /** Method to retrieve the name of image this command is set to draw.
       * @return The name of the image this command is set to draw.
       */
      const char* image() const ;

    private:
      
      /** Forward Declared structure to contain this object's data.
       */
      struct ImageCommandData* img_cmd_data ;
      
      /** Method to retrieve a reference to this object's underlying data structure.
       * @return A reference to this object's underlying data structure.
       */
      ImageCommandData& data() ;
      
      /** Method to retrieve a const reference to this object's underlying data structure.
       * @return A const reference to this object's underlying data structure.
       */
      const ImageCommandData& data() const ;
  };
  
  /** Object to contain a command to draw text. 
   * Used for interaction with the KGL library.
   */
  class TextCommand
  {
    public:
      
      /** Default Constructor
       */
      TextCommand() ;
      
      /** Copy constructor. Copies the parameters from the input.
       * @param cmd The command to copy.
       */
      TextCommand( const TextCommand& cmd ) ;
      
      /** Equivalence operator. Copies the parameters from the input command.
       * @param cmd The command to copy parameters from.
       */
      TextCommand& operator=( const TextCommand& cmd ) ;
      
      /** Default deconstructor. 
       */
      ~TextCommand() ;
      
      /** Method to set the name of font to use for this command.
       * @note The image name correlates to the images loaded by the current krender loaded.
       * @param name The name of image to be drawn.
       */
      void setFont( const char* name ) ;
      
      /** Method to set the name of text to be drawn using this command.
       * @note The image name correlates to the images loaded by the current krender loaded.
       * @param name The name of image to be drawn.
       */
      void setText( const char* text ) ;
      
      /** Method to set the red channel color of the text to be drawn by this command.
       * @param red The red channel to use for this command's color.
       */
      void setColorR( float red ) ;
      
      /** Method to set the red channel color of the text to be drawn by this command.
       * @param red The red channel to use for this command's color.
       */
      void setColorG( float green ) ;
      
      /** Method to set the red channel color of the text to be drawn by this command.
       * @param red The red channel to use for this command's color.
       */
      void setColorB( float blue ) ;
      
      /** Method to set the red channel color of the text to be drawn by this command.
       * @param red The red channel to use for this command's color.
       */
      void setColorA( float alpha ) ;
      
      /** Method to set the X position in pixels to draw this image.
       * @note This is assuming the top-left of the image is ( 0, 0 ).
       * @@param pos The X location to draw the image at.
       */
      void setPosX( float pos ) ;
      
      /** Method to set the Y position in pixels to draw this image.
       * @note This is assuming the top-left of the image is ( 0, 0 ).
       * @@param pos The Y location to draw the image at.
       */
      void setPosY( float pos ) ;
      
      /** Method to set the width in pixels to draw this image.
       * @note This is assuming the top-left of the image is ( 0, 0 ).
       * @@param pos The X location to draw the image at.
       */
      void setWidth( float pos ) ;
      
      /** Method to set the height in pixels to draw this image.
       * @note This is assuming the top-left of the image is ( 0, 0 ).
       * @@param pos The Y location to draw the image at.
       */
      void setHeight( float pos ) ;

      /** Method to set the degree of rotation of the image to be drawn.
       * @note If left unset, draws the image as if there is no rotation, aka, as it is loaded.
       * @param rotation The 0-360 degrees of rotation to use when rendering the image.
       */
      void setRotation( float rotation ) ;
      
      /** Method to retrieve the X-position of this command.
       * @return The X-postion of this command.
       */
      float posX() const ;
      
      /** Method to retrieve the Y-position of this command.
       * @return The Y-postion of this command.
       */
      float posY() const ;
      
      /** Method to retrieve the X-position of this command.
       * @return The X-postion of this command.
       */
      float width() const ;
      
      /** Method to retrieve the Y-position of this command.
       * @return The Y-postion of this command.
       */
      float height() const ;
      
      /** Method to retrieve the degrees of rotation of this command.
       * @return The 0-360 rotation of this draw command.
       */
      float rotation() const ;
      
      /** Method to retrieve the red color of this command.
       * @return The red color channel of this command.
       */
      float red() const ;
      
      /** Method to retrieve the red color of this command.
       * @return The red color channel of this command.
       */
      float green() const ;
      
      /** Method to retrieve the red color of this command.
       * @return The red color channel of this command.
       */
      float blue() const ;
      
      /** Method to retrieve the red color of this command.
       * @return The red color channel of this command.
       */
      float alpha() const ;
      
      /** Method to retrieve the name of image this command is set to draw.
       * @return The name of the image this command is set to draw.
       */
      const char* font() const ;
      
      /** Method to retrieve the contents of this command.
       * @return The message stored by this command.
       */
      const char* text() const ;

    private:
      
      /** Forward Declared structure to contain this object's data.
       */
      struct TextCommandData* txt_cmd_data ;
      
      /** Method to retrieve a reference to this object's underlying data structure.
       * @return A reference to this object's underlying data structure.
       */
      TextCommandData& data() ;
      
      /** Method to retrieve a const reference to this object's underlying data structure.
       * @return A const reference to this object's underlying data structure.
       */
      const TextCommandData& data() const ;
  };
  
  /** Object to contain camera information that modules understand.
   */
  class Camera
  {
    public:
      Camera() ;
      Camera( const Camera& camera ) ;
      ~Camera() ;
      Camera& operator=( const Camera& camera ) ;
      void setPosX( float pos ) ;
      void setPosY( float pos ) ;
      void setPosZ( float pos ) ;
      void setPos( float x, float y, float z ) ;
      float posX() const ;
      float posY() const ;
      float posZ() const ;
      void setFrontX( float x ) ;
      void setFrontY( float y ) ;
      void setFrontZ( float z ) ;
      void setFront( float x, float y, float z ) ;
      float frontX() const ;
      float frontY() const ;
      float frontZ() const ;
      void setUpX( float x ) ;
      void setUpY( float y ) ;
      void setUpZ( float z ) ;
      void setUp( float x, float y, float z ) ;
      float upX() const ;
      float upY() const ;
      float upZ() const ;
    private:
      struct CameraData* cam_data ;
      CameraData& data() ;
      const CameraData& data() const ;
  };

  /** Object to contain a command to draw a sprite from a spritesheet. 
   * Used for interaction with the KGL library.
   */
  class SheetCommand
  {
    public:
      
      /** Default Constructor
       */
      SheetCommand() ;
      
      /** Copy constructor. Copies the parameters from the input.
       * @param cmd The command to copy.
       */
      SheetCommand( const SheetCommand& cmd ) ;
      
      /** Equivalence operator. Copies the parameters from the input command.
       * @param cmd The command to copy parameters from.
       */
      SheetCommand& operator=( const SheetCommand& cmd ) ;

      /** Deconstructor.
       */
      ~SheetCommand() ;
      
     /** Method to set the name of sheet to be drawn by this command.
       * @note The image name correlates to the images loaded by the current krender loaded.
       * @param name The name of sheet to reference for the sprite.
       */
      void setSheet( const char* name ) ;
      
      /** Method to set the X position in pixels to draw this image.
       * @note This is assuming the top-left of the image is ( 0, 0 ).
       * @@param pos The X location to draw the image at.
       */
      void setPosX( float pos ) ;
      
      /** Method to set the Y position in pixels to draw this image.
       * @note This is assuming the top-left of the image is ( 0, 0 ).
       * @@param pos The Y location to draw the image at.
       */
      void setPosY( float pos ) ;
      
      /** Method to set the width to draw the image at.
       * @note If left unset, draws the image at it's original width.
       * @param width The width, in pixels, to draw the image at.
       */
      void setWidth( float width ) ;
      
      /** Method to set the height to draw the image at.
       * @note If left unset, draws the image at it's original height.
       * @param height The height, in pixels, to draw the image at.
       */
      void setHeight( float height ) ;
      
      /** Method to set the degree of rotation of the image to be drawn.
       * @note If left unset, draws the image as if there is no rotation, aka, as it is loaded.
       * @param rotation The 0-360 degrees of rotation to use when rendering the image.
       */
      void setRotation( float rotation ) ;
      
      /** Method to set the index of sprite to draw by this command.
       * @note The sprite size is calculated by what is in the database JSON for this spritesheet.
       * @note This index is assumed to be the first sprite in the spritesheet, starting from the top-left, going per row. 
       *        e.g.
       *            ______________
       *          | 0 1 2 3 4 5 6 |
       *          | 7 8 9 10 11 12|
       *          | ...           |
       *          |               |
       *          |               |
       *          |_______________|
       * 
       * @param index The index of sprite to draw in this command.
       */
      void setIndex( unsigned index ) ;
      
      /** Method to retrieve the X-position of this command.
       * @return The X-postion of this command.
       */
      float posX() const ;
      
      /** Method to retrieve the Y-position of this command.
       * @return The Y-postion of this command.
       */
      float posY() const ;
      
      /** Method to retrieve the width of this draw command.
       * @return The width of this draw command. If unset, returns 0.
       */
      float width() const ;
      
      /** Method to retrieve the height of this draw command.
       * @return The height of this draw command. If unset, returns 0.
       */
      float height() const ;
      
      /** Method to retrieve the degrees of rotation of this command.
       * @return The 0-360 rotation of this draw command.
       */
      float rotation() const ;
      
      /** Method to retrieve the name of sheet this command is set to draw.
       * @return The name of the sheet this command is set to draw.
       */
      const char* sheet() const ;
      
      /** Method to retrieve the index of sprite this command is set to draw.
       * @return The index of sprite being drawn by this command.
       */
      unsigned index() const ;

    private:
      
      /** Forward Declared structure to contain this object's data.
       */
      struct SheetCommandData* img_cmd_data ;
      
      /** Method to retrieve a reference to this object's underlying data structure.
       * @return A reference to this object's underlying data structure.
       */
      SheetCommandData& data() ;
      
      /** Method to retrieve a const reference to this object's underlying data structure.
       * @return A const reference to this object's underlying data structure.
       */
      const SheetCommandData& data() const ;
  };
}
#endif

