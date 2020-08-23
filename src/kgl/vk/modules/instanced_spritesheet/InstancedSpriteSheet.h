#ifndef KGL_INSTANCED_SPRITESHEET_MODULE_H
#define KGL_INSTANCED_SPRITESHEET_MODULE_H

#include <Module.h>

namespace kgl
{
  namespace vk
  {
    
    class Synchronization ;
    
    /** Module for rendering a constant list of sprites in a spritesheet/atlas.
     */
    class InstancedSpriteSheet : public ::kgl::vk::Module
    {
      public:
        
        /** Default constructor. Allocates object data.
         */
        InstancedSpriteSheet() ;
        
        /** Deconstructor. Releases Object data.
         */
        ~InstancedSpriteSheet() ;
        
        /** Method to initialize this module's internal data.
         */
        void initialize() final ;
        
        /** Method to shutdown operations of this module.
         */
        void shutdown() final ;
        
        /** Method to set the name of this object's synchronization input.
         */
        void setInputName( const char* name ) ;
        
        /** Method to recieve a synchronization object for rendering synchronization.
         * @param sync The object to wait on for operations.
         */
        void input( const ::kgl::vk::Synchronization& sync ) ;
        
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
        struct InstancedSpriteSheetData *isprite_data ;
        
        /** Method to retrieve a reference to this object's underlying data.
         * @return Reference to this object's underlying data.
         */
        InstancedSpriteSheetData& data() ;
        
        /** Method to retrieve a const reference to this object's underlying data.
         * @return Const reference to this object's underlying data.
         */
        const InstancedSpriteSheetData& data() const ;
    };
  }
}
#endif /* RENDER_H */

