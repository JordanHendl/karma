#ifndef CASPER_VULKAN_2D_VERTEX
#define CASPER_VULKAN_2D_VERTEX
#include "casper_vulkan_vertex.h"

namespace casper
{
  namespace vulkan
  {
    class VertexType2D ;
    class Vertex2D
    {
      public:
        static VertexType2D type() ;

        Vertex2D() ;
        void setX( float y ) ;
        void setY( float x ) ;
        void setUVX( float x ) ;
        void setUVY( float y ) ;
        float UVX() ;
        float UVY() ;
        float x() ;
        float y() ;
        const float* data() ;
      private:
        float info[4] ;

        friend class VertexType2D ;
    };

    class VertexType2D : public VertexType
    {
      public:
        VertexType2D() ;
        ~VertexType2D() ;
        const VkVertexInputBindingDescription* desc()  const ;
        const VkVertexInputAttributeDescription* attr() const ;
        unsigned numDesc() const ;
        unsigned numAttr() const ;
    };
  }
}
#endif