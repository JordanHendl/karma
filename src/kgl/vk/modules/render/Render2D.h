
#ifndef KGL_RENDER_MODULE_H
#define KGL_RENDER_MODULE_H

#ifdef _WIN32
  #include <windows.h>
  #define  exported_function extern "C" __declspec(dllexport)
#else 
  #define exported_function extern "C"
#endif

#include <Module.h>

namespace kgl
{
  namespace vk
  {
    class Synchronization ;
    class Render2D : public ::kgl::vk::Module
    {
      public:
        Render2D() ;
        ~Render2D() ;
        void initialize() final ;
        void shutdown() final ;
        void subscribe( const char* pipeline, unsigned id ) final ;

        /** Method to set the name of this object's synchronization input.
         */
        void setInputName( const char* name ) ;
        
        /** Method to recieve a synchronization object for rendering synchronization.
         * @param sync The object to wait on for operations.
         */
        void input ( const ::kgl::vk::Synchronization& sync ) ;

        void execute() final ;
        void resize() final ;
      private:
        struct Render2DData *data_2d ;
        Render2DData& data() ;
        const Render2DData& data() const ;
    };
  }
}

/**
 * @return 
 */
exported_function const char* name()
{
  return "Render2D" ;
}

/**
 * @return 
 */
exported_function unsigned version()
{
  return 1 ;
}

/**
 * @return 
 */
exported_function ::kgl::vk::Module* make( unsigned version )
{
  return new ::kgl::vk::Render2D() ;
}

/**
 * @param module
 */
exported_function void destroy( ::kgl::vk::Module* module )
{
  ::kgl::vk::Render2D* mod ;
  
  mod = dynamic_cast<::kgl::vk::Render2D*>( module ) ;
  delete mod ;
}

#endif /* RENDER_H */

