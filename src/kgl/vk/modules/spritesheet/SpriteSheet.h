
#ifndef KGL_SPRITESHEET_MODULE_H
#define KGL_SPRITESHEET_MODULE_H

#include <Module.h>

namespace kgl
{
  namespace vk
  {
    class SpriteSheet : public ::kgl::vk::Module
    {
      public:
        SpriteSheet() ;
        ~SpriteSheet() ;
        void initialize() final ;
        void shutdown() final ;
        void subscribe( const char* pipeline, unsigned id ) final ;
        void execute() final ;
        void resize() final ;
      private:
        struct SpriteSheetData *data_2d ;
        SpriteSheetData& data() ;
        const SpriteSheetData& data() const ;
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

