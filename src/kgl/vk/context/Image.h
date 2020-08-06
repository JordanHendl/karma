#ifndef KGL_VK_IMAGE_H
#define KGL_VK_IMAGE_H

namespace vk
{
  class Sampler   ;
  class ImageView ;
  enum class ImageLayout ;
}

namespace kgl
{
  namespace vk
  {
    class Synchronization ;
    class Image
    {
      public:
        Image() ;
        ~Image() ;
        void initialize( unsigned gpu, unsigned w, unsigned h ) ;
        const ::vk::Sampler sampler() const ;
        const ::vk::ImageView view() const ;
        void setLayout( const ::vk::ImageLayout& layout ) ;
        unsigned width() const ;
        unsigned height() const ;
        void copy( const unsigned char* data, unsigned channels ) ;
        void copy( const Image& image ) ;
        void copy( const Image& image, Synchronization& sync ) ;
        void reset() ;
      private:
        struct ImageData *img_data ;
        ImageData& data() ;
        const ImageData& data() const ;
    };
  }
}

#endif /* IMAGE_H */

