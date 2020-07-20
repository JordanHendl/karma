#ifndef KGL_PRESENT_MODULE_H
#define KGL_PRESENT_MODULE_H

#include <Module.h>

const unsigned VERSION = 1 ;

namespace kgl
{
  namespace vk
  {
    class Present : public ::kgl::vk::Module
    {
      public:
        Present() ;
        ~Present() ;
        void initialize() final ;
        void shutdown() final ;
        void subscribe( const char* pipeline, unsigned id ) final ;
        void execute() final ;
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
extern "C" const char* name()
{
  return "Present" ;
}

/**
 * @return 
 */
extern "C" unsigned version()
{
  return VERSION ;
}

/**
 * @return 
 */
extern "C" ::kgl::vk::Module* make()
{
  return new ::kgl::vk::Present() ;
}

/**
 * @param module
 */
extern "C" void destroy( ::kgl::vk::Module* module )
{
  ::kgl::vk::Present* mod ;
  
  mod = dynamic_cast<::kgl::vk::Present*>( module ) ;
  delete mod ;
}

#endif
