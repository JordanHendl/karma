#ifndef KGL_COMBINER_MODULE_H
#define KGL_COMBINER_MODULE_H

#include <Module.h>

namespace kgl
{
  namespace vk
  {
    
    class Synchronization ;
    class Image           ;
    /** Module for rendering sprites in a spritesheet/atlas.
     */
    class Combiner : public ::kgl::vk::Module
    {
      public:
        
        /** Default constructor. Allocates object data.
         */
        Combiner() ;
        
        /** Deconstructor. Releases Object data.
         */
        ~Combiner() ;
        
        /** Method to initialize this module's internal data.
         */
        void initialize() final ;
        
        /** Method to shutdown operations of this module.
         */
        void shutdown() final ;
        
        /** Method to set the name of this object's synchronization input.
         * @param name The name of the sync object to use for this slot.
         */
        void setInputNameOne   ( const char* name ) ;
        void setInputNameTwo   ( const char* name ) ;
        void setInputNameThree ( const char* name ) ;
        void setInputNameFour  ( const char* name ) ;
        void setInputNameFive  ( const char* name ) ;
        void setInputNameSix   ( const char* name ) ;
        void setInputNameSeven ( const char* name ) ;
        void setInputNameEight ( const char* name ) ;
        void setInputNameNine  ( const char* name ) ;
        void setInputNameTen   ( const char* name ) ;
        
        /** Methods to set the input image names.
         * @param name The name if the image to input for this slot.
         */
        void setInputImageNameOne   ( const char* name ) ;
        void setInputImageNameTwo   ( const char* name ) ;
        void setInputImageNameThree ( const char* name ) ;
        void setInputImageNameFour  ( const char* name ) ;
        void setInputImageNameFive  ( const char* name ) ;
        void setInputImageNameSix   ( const char* name ) ;
        void setInputImageNameSeven ( const char* name ) ;
        void setInputImageNameEight ( const char* name ) ;
        void setInputImageNameNine  ( const char* name ) ;
        void setInputImageNameTen   ( const char* name ) ;
        
        /** Method to recieve a synchronization object for rendering synchronization.
         * @param sync The object to wait on for operations.
         */
        void inputOne   ( const ::kgl::vk::Synchronization& sync ) ;
        void inputTwo   ( const ::kgl::vk::Synchronization& sync ) ;
        void inputThree ( const ::kgl::vk::Synchronization& sync ) ;
        void inputFour  ( const ::kgl::vk::Synchronization& sync ) ;
        void inputFive  ( const ::kgl::vk::Synchronization& sync ) ;
        void inputSix   ( const ::kgl::vk::Synchronization& sync ) ;
        void inputSeven ( const ::kgl::vk::Synchronization& sync ) ;
        void inputEight ( const ::kgl::vk::Synchronization& sync ) ;
        void inputNine  ( const ::kgl::vk::Synchronization& sync ) ;
        void inputTen   ( const ::kgl::vk::Synchronization& sync ) ;
        
        void inputImageOne   ( const ::kgl::vk::Image& image ) ;
        void inputImageTwo   ( const ::kgl::vk::Image& image ) ;
        void inputImageThree ( const ::kgl::vk::Image& image ) ;
        void inputImageFour  ( const ::kgl::vk::Image& image ) ;
        void inputImageFive  ( const ::kgl::vk::Image& image ) ;
        void inputImageSix   ( const ::kgl::vk::Image& image ) ;
        void inputImageSeven ( const ::kgl::vk::Image& image ) ;
        void inputImageEight ( const ::kgl::vk::Image& image ) ;
        void inputImageNine  ( const ::kgl::vk::Image& image ) ;
        void inputImageTen   ( const ::kgl::vk::Image& image ) ;
        
        /** Method to subscribe this module's inputs to the data bus.
         * @param pipeline The name of pipeline this object is a part of.
         * @param id The ID of bus to subscribe to.
         */
        void subscribe( const char* pipeline, unsigned id ) final ;
        
        /** Method to perform a single operation of this module's function.
         */
        void execute() final ;
        
        /** Method to resize this object's parameters on window resizing.
         */
        void resize() final ;
      private:
        
        /** Forward declared structure containing this object's data.
         */
        struct CombinerData *data_2d ;
        
        /** Method to retrieve a reference to this object's underlying data.
         * @return Reference to this object's underlying data.
         */
        CombinerData& data() ;
        
        /** Method to retrieve a const reference to this object's underlying data.
         * @return Const reference to this object's underlying data.
         */
        const CombinerData& data() const ;
    };
  }
}
#endif /* RENDER_H */

