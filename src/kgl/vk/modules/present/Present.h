#ifndef KGL_PRESENT_MODULE_H
#define KGL_PRESENT_MODULE_H

#include <Module.h>

const unsigned VERSION = 1 ;

namespace kgl
{
  namespace vk
  {
    class Synchronization ;
    class Image ;
    /** Module for copying a framebuffer to the window & signalling window to present.
     */
    class Present : public ::kgl::vk::Module
    {
      public:
        
        /** Default constructor. Allocates object data.
         */
        Present() ;
        
        /** Deconstructor. Releases Object data.
         */
        ~Present() ;
        
        /** Method to initialize this module's internal data.
         */
        void initialize() final ;
        
        /** Method to shutdown operations of this module.
         */
        void shutdown() final ;
        
        /** Method to subscribe this module's inputs to the data bus.
         * @param pipeline The name of pipeline this object is a part of.
         * @param id The ID of bus to subscribe to.
         */
        void subscribe( const char* pipeline, unsigned id ) final ;
        
        /** Method to perform a single operation of this module's function.
         */
        void execute() final ;

        /** Method to set the name of this object's synchronization input.
         */
        void setInputName( const char* name ) ;
        
        void setInputImageName( const char* name ) ;
        void setImage( const ::kgl::vk::Image& image ) ;
        /** Method to recieve a synchronization object for rendering synchronization.
         * @param sync The object to wait on for operations.
         */
        void input ( const ::kgl::vk::Synchronization& sync ) ;
        
        /** Method to resize this object's parameters on window resizing.
         */
        void resize() final ;
 
      private:
        
        /** Forward declared structure containing this object's data.
         */
        struct PresentData *present_data ;
        
        /** Method to retrieve a reference to this object's underlying data.
         * @return Reference to this object's underlying data.
         */
        PresentData& data() ;
        
        /** Method to retrieve a const reference to this object's underlying data.
         * @return Const reference to this object's underlying data.
         */
        const PresentData& data() const ;
    };
  }
}

#endif
