
#ifndef KGL_RENDER_MODULE_H
#define KGL_RENDER_MODULE_H

#include <Module.h>

namespace kgl
{
  namespace vk
  {
    class Render2D : public ::kgl::vk::Module
    {
      public:
        Render2D() ;
        ~Render2D() ;
        void initialize() final ;
        void shutdown() final ;
        void subscribe( const char* pipeline, unsigned id ) final ;
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
extern "C" ::kgl::vk::Module* make( unsigned version )
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

