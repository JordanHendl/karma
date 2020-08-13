#ifndef KGL_RENDER_MODULE_H
#define KGL_RENDER_MODULE_H

#include <Module.h>

namespace kgl
{
  namespace vk
  {
    class Synchronization ;
    
    /** Module for rendering 2D Images.
     */
    class Render2D : public ::kgl::vk::Module
    {
      public:
        
        /** Default constructor. Allocates object data.
         */
        Render2D() ;
        
        /** Deconstructor. Releases Object data.
         */
        ~Render2D() ;
        
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

        /** Method to set the name of this object's synchronization input.
         */
        void setInputName( const char* name ) ;
        
        /** Method to recieve a synchronization object for rendering synchronization.
         * @param sync The object to wait on for operations.
         */
        void input ( const ::kgl::vk::Synchronization& sync ) ;
        
        /** Method to perform a single operation of this module's function.
         */
        void execute() final ;
        
        /** Method to resize this object's parameters on window resizing.
         */
        void resize() final ;

      private:
        
        /** Forward declared structure containing this object's data.
         */
        struct Render2DData *data_2d ;
        
        /** Method to retrieve a reference to this object's underlying data.
         * @return Reference to this object's underlying data.
         */
        Render2DData& data() ;
        
        /** Method to retrieve a const reference to this object's underlying data.
         * @return Const reference to this object's underlying data.
         */
        const Render2DData& data() const ;
    };
  }
}

/** Exported function to retrive the name of this module type.
 * @return The name of this object's type.
 */
exported_function const char* name()
{
  return "Render2D" ;
}

/** Exported function to retrieve the version of this module.
 * @return The version of this module.
 */
exported_function unsigned version()
{
  return 1 ;
}

/** Exported function to make one instance of this module.
 * @return A single instance of this module.
 */
exported_function ::kgl::vk::Module* make( unsigned version )
{
  return new ::kgl::vk::Render2D() ;
}

/** Exported function to destroy an instance of this module.
 * @param module A Pointer to a Module object that is of this type.
 */
exported_function void destroy( ::kgl::vk::Module* module )
{
  ::kgl::vk::Render2D* mod ;
  
  mod = dynamic_cast<::kgl::vk::Render2D*>( module ) ;
  delete mod ;
}

#endif 