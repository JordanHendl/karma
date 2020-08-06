#include "ImageCommand.h"
#include <vulkan/vulkan.hpp>

namespace kgl
{
  namespace vk
  {
    namespace gpu
    {
      struct ImageCommandData
      {
        
      };

      ImageCommand::ImageCommand()
      {
        this->cmd_data = new ImageCommandData() ;
      }

      ImageCommand::~ImageCommand()
      {
        delete this->cmd_data ;
      }

      void ImageCommand::push( const ::vk::CommandBuffer buffer ) const
      {
//        buffer.draw()
      }

      void ImageCommand::setImage( const char* name )
      {
        
      }

      void ImageCommand::setPosition( float x, float y, float z = 0 )
      {
      
      }

      void ImageCommand::setRotation( float x, float y, float z = 1 )
      {
      
      }

      ImageCommandData& ImageCommand::data()
      {
        return *this->cmd_data ;
      }

      const ImageCommandData& ImageCommand::data() const
      {
        return *this->cmd_data ;
      }
    }
  }
}

