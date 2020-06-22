#include "casper_vulkan_2d_vertex.h"
#include <vulkan/vulkan.h>

namespace casper
{
  namespace vulkan
  {
    static VkVertexInputAttributeDescription attribute   ;
    static VkVertexInputBindingDescription   description ;

    Vertex2D::Vertex2D()
    {
    
    }

    void  Vertex2D::setX( float y )
    {
      this->info[ 1 ] = y ;
    }

    void  Vertex2D::setY( float x )
    {
      this->info[ 0 ] = x ;
    }

    void  Vertex2D::setUVX( float x )
    {
      this->info[ 2 ] = x ;
    }

    void  Vertex2D::setUVY( float y )
    {
      this->info[ 3 ] = y ;
    }

    float  Vertex2D::x()
    {
      return this->info[ 1 ] ;
    }

    float  Vertex2D::y()
    {
      return this->info[ 0 ] ;
    }

    float  Vertex2D::UVX()
    {
      return this->info[ 2 ] ;
    }

    float  Vertex2D::UVY()
    {
      return this->info[ 3 ] ;
    }

    const float* Vertex2D::data()
    {
      return this->info ;
    }

    VertexType2D::VertexType2D()
    {

    }

    VertexType2D::~VertexType2D()
    {

    }

    VertexType2D Vertex2D::type()
    {
      return VertexType2D() ;
    }

    const VkVertexInputBindingDescription* VertexType2D::desc()  const
    {
      description.binding   = 0                           ;
      description.stride    = sizeof( Vertex2D )          ;
      description.inputRate = VK_VERTEX_INPUT_RATE_VERTEX ;

      return &description ;
    }

    const VkVertexInputAttributeDescription* VertexType2D::attr() const
    {

      attribute.binding  = 0 ;
      attribute.location = 0 ;
      attribute.format = VK_FORMAT_R32G32B32A32_SFLOAT ;
      attribute.offset = offsetof( Vertex2D, info ) ;
      return &attribute ;
    }

    unsigned VertexType2D::numAttr() const 
    {
      return 1 ; 
    }

    unsigned VertexType2D::numDesc() const
    {
      return 1 ; 
    }
  }
}