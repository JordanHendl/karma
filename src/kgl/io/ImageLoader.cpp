#include "ImageLoader.h"
#include "stb_image.h"
#include <vector>
namespace kgl
{
  namespace io
  {
    struct ImageLoaderData
    {
      typedef std::vector<unsigned char> Pixels ;
      
      Pixels   pixels   ;
      unsigned width    ;
      unsigned height   ;
      unsigned channels ;
    };

    ImageLoader::ImageLoader()
    {
      this->loader_data = new ImageLoaderData() ;
    }
    
    ImageLoader::~ImageLoader()
    {
      delete this->loader_data ;
    }
    
    void ImageLoader::reset()
    {
      data().pixels.clear() ;
    }

    void ImageLoader::load( const char* path )
    {
      int width  ;
      int height ;
      int chan   ;
      const unsigned char* pixels = stbi_load( path, &width, &height, &chan, 4 ) ;
      
//      stbi_set_flip_vertically_on_load( true ) ;  
      data().width    = static_cast<unsigned>( width  ) ;
      data().height   = static_cast<unsigned>( height ) ;
      data().channels = static_cast<unsigned>( 4      ) ;
      data().pixels.assign( pixels, pixels + ( width * height * 4 ) ) ;
    }
    
    unsigned ImageLoader::width() const
    {
      return data().width ;
    }
    
    unsigned ImageLoader::height() const
    {
      return data().height ;
    }

    unsigned ImageLoader::channels() const
    {
      return data().channels ;
    }
    
    const unsigned char* ImageLoader::pixels() const
    {
      return data().pixels.data() ;
    }
    
    ImageLoaderData& ImageLoader::data()
    {
      return *this->loader_data ;
    }
    
    const ImageLoaderData& ImageLoader::data() const
    {
      return *this->loader_data ;
    }
  }
}
