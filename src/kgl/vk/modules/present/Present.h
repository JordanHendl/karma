#ifndef KGL_PRESENT_MODULE_H
#define KGL_PRESENT_MODULE_H

#include <Module.h>

#ifdef _WIN32
  #include <windows.h>
  #define  exported_function extern "C" __declspec(dllexport)
#else 
  #define exported_function extern "C"
#endif

const unsigned VERSION = 1 ;

namespace kgl
{
  namespace vk
  {
    class Synchronization ;

    class Present : public ::kgl::vk::Module
    {
      public:
        Present() ;
        ~Present() ;
        void initialize() final ;
        void shutdown() final ;
        void subscribe( const char* pipeline, unsigned id ) final ;
        void execute() final ;

        /** Method to set the name of this object's synchronization input.
         */
        void setInputName( const char* name ) ;
        
        /** Method to recieve a synchronization object for rendering synchronization.
         * @param sync The object to wait on for operations.
         */
        void input ( const ::kgl::vk::Synchronization& sync ) ;

        void resize() final ;
      private:
        struct PresentData *data_2d ;
        PresentData& data() ;
        const PresentData& data() const ;
    };
  }
}

/**
 * @return 
 */
exported_function const char* name()
{
  return "Present" ;
}

/**
 * @return 
 */
exported_function unsigned version()
{
  return VERSION ;
}

/**
 * @return 
 */
exported_function ::kgl::vk::Module* make()
{
  return new ::kgl::vk::Present() ;
}

/**
 * @param module
 */
exported_function void destroy( ::kgl::vk::Module* module )
{
  ::kgl::vk::Present* mod ;
  
  mod = dynamic_cast<::kgl::vk::Present*>( module ) ;
  delete mod ;
}

#endif
