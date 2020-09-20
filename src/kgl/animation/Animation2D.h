#ifndef ANIMATION2D_H
#define ANIMATION2D_H

namespace kgl
{
  /** Forward declared structures for drawing commands.
   */
  class ImageCommand ;
  class SheetCommand ;

  namespace anim
  {
    
    /** Object for managing sprite animations.
     */
    class Sprite2D
    {
      public:
        
        /** Default constructor.
         */
        Sprite2D() ;
        
        /** Copy constructor. Copies data from input animation.
         * @param anim The animation to copy data from.
         */
        Sprite2D( const Sprite2D& anim ) ;
        
        /** Default deconstructor.
         */
        ~Sprite2D() ;
        
        /** Equals operator. Copies data from input animation.
         * @param anim The animation to copy data from.
         * @return Reference to this object.
         */
        Sprite2D& operator=( const Sprite2D& anim ) ;
        
        /** Method to retrieve the kgl draw command for the current frame of animation this animation is on.
         * @return The draw command of the current frame this animation is on.
         */
        const SheetCommand& current( float posx, float posy, float posz, float rotation ) ;
        
        /** Method to retrieve the current index of animation to draw given the current frame time.
         * @return The index of frame ( 0 - number of frames ) to draw.
         */
        unsigned current() const ;
        
        /**
         * @param sprite_sheet_name
         * @param frame_time
         */
        void initialize( const char* sprite_sheet_name, float sizex, float sizey, float frame_time ) ;
        
        /**
         * @param anim_index
         */
        void insertAnimation( unsigned anim_index ) ;
        
        /** Method to reset this animation to it's base state.
         */
        void reset() ;
        
        /** Method to start the timing of this animation.
         */
        void start() ;
        
        /** Method to stop the timing of this animation.
         */
        void pause() ;
        
        /** Method to clear all frames of this animation.
         */
        void clear() ;
        
      private:
        
        /** Forward Declared container for this object's internal data.
         */
        struct Sprite2DData *anim2d_data ;
        
        /** Method to retrieve a reference to this object's internal data.
         * @return Reference to this object's internal data.
         */
        Sprite2DData& data() ;
        
        /** Method to retrieve a const reference to this object's internal data.
         * @return Const reference to this object's internal data.
         */
        const Sprite2DData& data() const ;
    };
    
    /** Object for managing image animations.
     */
    class Image2D
    {
      public:
        
        /** Default constructor.
         */
        Image2D() ;
        
        /** Copy constructor. Copies data from input animation.
         * @param anim The animation to copy data from.
         */
        Image2D( const Image2D& anim ) ;
        
        /** Default deconstructor.
         */
        ~Image2D() ;
        
        /** Equals operator. Copies data from input animation.
         * @param anim The animation to copy data from.
         * @return Reference to this object.
         */
        Image2D& operator=( const Image2D& anim ) ;
        
        /** Method to retrieve the kgl draw command for the current frame of animation this animation is on.
         * @return The draw command of the current frame this animation is on.
         */
        const ImageCommand& current( float posx, float posy, float posz, float rotation ) ;
        
        /**
         * @param sprite_sheet_name
         * @param frame_time
         */
        void initialize( float sizex, float sizey, float frame_time ) ;
        
        /**
         * @param anim_index
         */
        void insertAnimation( const char* image_name ) ;
        
        /** Method to reset this animation to it's base state.
         */
        void reset() ;
        
        /** Method to start the timing of this animation.
         */
        void start() ;
        
        /** Method to stop the timing of this animation.
         */
        void pause() ;
        
        /** Method to clear all frames of this animation.
         */
        void clear() ;
        
      private:
        
        /** Forward Declared container for this object's internal data.
         */
        struct Image2DData *anim2d_data ;
        
        /** Method to retrieve a reference to this object's internal data.
         * @return Reference to this object's internal data.
         */
        Image2DData& data() ;
        
        /** Method to retrieve a const reference to this object's internal data.
         * @return Const reference to this object's internal data.
         */
        const Image2DData& data() const ;
    };
  }
}

#endif /* ANIMATION2D_H */

