#ifndef KGL_VK_IMAGECOMMAND_H
#define KGL_VK_IMAGECOMMAND_H
#include "Command.h"

namespace kgl
{
  namespace vk
  {
    namespace gpu
    {
      class ImageCommand : public Command 
      {
        public:
          ImageCommand() ;
          ~ImageCommand() ;
          void push( const ::vk::CommandBuffer buffer ) const ;
          void setImage( const char* name ) ;
          void setPosition( float x, float y, float z = 0 ) ;
          void setRotation( float x, float y, float z = 1 ) ;
        private:
          struct ImageCommandData *cmd_data ;
          ImageCommandData& data() ;
          const ImageCommandData& data() const ;
      };
    }
  }
}

#endif /* IMAGECOMMAND_H */

