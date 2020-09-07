#ifndef KGL_VK_IMAGE_H
#define KGL_VK_IMAGE_H

namespace vk
{
  class Sampler          ;
  class ImageView        ;
  class Semaphore        ;
  class CommandBuffer    ;
  enum class ImageLayout ;
  enum class Format      ;
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
        Image( const Image& image ) ;
        ~Image() ;
        void initialize( unsigned gpu, unsigned w, unsigned h ) ;
        Image& operator=( const Image& image ) ;
        bool operator<( const Image& image ) const ;
        const ::vk::Sampler sampler() const ;
        const ::vk::ImageView view() const ;
        const ::vk::ImageLayout& layout() const ;
        void setLayout( const ::vk::ImageLayout& layout ) const ;
        void setFormat( const ::vk::Format& format ) ;
        unsigned width() const ;
        unsigned height() const ;
        void copy( const unsigned char* data, unsigned channels ) ;
        void copy( const Image& image, Synchronization& sync, ::vk::CommandBuffer& buffer ) ;
        void reset() ;
      private:
        struct ImageData *img_data ;
        ImageData& data() ;
        const ImageData& data() const ;
    };
  }
}

#endif /* IMAGE_H */

