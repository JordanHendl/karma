#ifndef KGL_VK_IMAGE_H
#define KGL_VK_IMAGE_H

namespace vk
{
  class Sampler   ;
  class ImageView ;
}

namespace kgl
{
  namespace vk
  {
    class Image
    {
      public:
        Image() ;
        ~Image() ;
        void initialize( unsigned gpu, unsigned w, unsigned h ) ;
        const ::vk::Sampler sampler() const ;
        const ::vk::ImageView view() const ;
        void copy( const unsigned char* data ) ;
        void copy( const Image& image ) ;
      private:
        struct ImageData *img_data ;
        ImageData& data() ;
        const ImageData& data() const ;
    };
  }
}

#endif /* IMAGE_H */

