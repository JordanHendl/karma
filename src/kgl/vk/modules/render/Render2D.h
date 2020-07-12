
#ifndef KGL_RENDER_MODULE_H
#define KGL_RENDER_MODULE_H

#include <Module.h>

namespace kgl
{
  namespace vk
  {
    namespace gpu
    {
      class Command ;
    }

    class Render2D : public ::kgl::vk::Module
    {
      public:
        Render2D() ;
        ~Render2D() ;
        void initialize() ;
        void shutdown() ;
        void clear() ;
        void add( const gpu::Command& command ) ;
        void subscribe( unsigned id ) ;
        void execute() ;
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
extern "C" const char* name()
{
  return "Render2D" ;
}

/**
 * @return 
 */
extern "C" unsigned version()
{
  return 1 ;
}

/**
 * @return 
 */
extern "C" ::kgl::vk::Module* make()
{
  return new ::kgl::vk::Render2D() ;
}

/**
 * @param module
 */
extern "C" void destroy( ::kgl::vk::Module* module )
{
  ::kgl::vk::Render2D* mod ;
  
  mod = dynamic_cast<::kgl::vk::Render2D*>( module ) ;
  delete mod ;
}

#endif /* RENDER_H */

