#include "DrawCommand.h"
#include <string>

namespace kgl
{
  struct ImageCommandData
  {
    std::string image  ;
    float       width  ;
    float       height ;
    float       x      ;
    float       y      ;
    float       rot    ;
    
    /**
     */
    ImageCommandData() ;
  };
  
  struct SheetCommandData
  {
    std::string image  ;
    unsigned    index  ;
    float       width  ;
    float       height ;
    float       x      ;
    float       y      ;
    float       rot    ;
    
    SheetCommandData() ;
  };
  
  struct CameraData
  {
    float posx   ;
    float posy   ;
    float posz   ;
    float frontx ;
    float fronty ;
    float frontz ;
    float upx    ;
    float upy    ;
    float upz    ;
    
    CameraData() ;
  };
  
  ImageCommandData::ImageCommandData()
  {
    this->x      = 0  ;
    this->y      = 0  ;
    this->image  = "" ;
    this->height = 0  ;
    this->width  = 0  ;
    this->rot    = 0  ;
  }
  
  SheetCommandData::SheetCommandData()
  {
    this->x      = 0  ;
    this->y      = 0  ;
    this->image  = "" ;
    this->height = 0  ;
    this->width  = 0  ;
    this->rot    = 0  ;
    this->index  = 0  ;
  }
  
  CameraData::CameraData()
  {
    this->posx   =  0.0f ;
    this->posy   =  0.0f ;
    this->posz   =  0.0f ;
    this->frontx =  0.0f ;
    this->fronty =  0.0f ;
    this->frontz = -1.0f ;
    this->upx    =  0.0f ;
    this->upy    =  1.0f ;
    this->upz    =  0.0f ;
  }

  Camera::Camera()
  {
    this->cam_data = new CameraData() ;
  }

  Camera::Camera( const Camera& camera )
  {
    this->cam_data = new CameraData() ;
  }

  Camera::~Camera()
  {
    delete this->cam_data ;
  }

  Camera& Camera::operator=( const Camera& camera )
  {
    *this->cam_data = *camera.cam_data ;
    
    return *this ;
  }

  void Camera::setPosX( float pos )
  {
    data().posx = pos ;
  }

  void Camera::setPosY( float pos )
  {
    data().posy = pos ;
  }

  void Camera::setPosZ( float pos )
  {
    data().posz = pos ;
  }
  
  float Camera::posX() const
  {
    return data().posx ;
  }
  
  float Camera::posY() const
  {
    return data().posy ;
  }
  
  float Camera::posZ() const
  {
    return data().posz ;
  }

  void Camera::setPos( float x, float y, float z )
  {
    this->setPosX( x ) ;
    this->setPosY( y ) ;
    this->setPosZ( z ) ;
  }

  void Camera::setFrontX( float x )
  {
    data().frontx = x ;
  }

  void Camera::setFrontY( float y )
  {
    data().fronty = y ;
  }

  void Camera::setFrontZ( float z )
  {
    data().frontz = z ;
  }

  void Camera::setFront( float x, float y, float z )
  {
    this->setFrontX( x ) ;
    this->setFrontY( y ) ;
    this->setFrontZ( z ) ;
  }
  
  float Camera::frontX() const
  {
    return data().frontx ;
  }
  
  float Camera::frontY() const
  {
    return data().fronty ;
  }
  
  float Camera::frontZ() const
  {
    return data().frontz ;
  }

  void Camera::setUpX( float x )
  {
    data().upx = x ;
  }

  void Camera::setUpY( float y )
  {
    data().upy = y ;
  }

  void Camera::setUpZ( float z )
  {
    data().upz = z ;
  }

  void Camera::setUp( float x, float y, float z )
  {
    this->setUpX( x ) ;
    this->setUpY( y ) ;
    this->setUpZ( z ) ;
  }

  float Camera::upX() const
  {
    return data().upx ;
  }

  float Camera::upY() const
  {
    return data().upy ;
  }

  float Camera::upZ() const
  {
    return data().upz ;
  }
  
  CameraData& Camera::data()
  {
    return *this->cam_data ;
  }

  const CameraData& Camera::data() const
  {
    return *this->cam_data ;
  }

  ImageCommand::ImageCommand()
  {
    this->img_cmd_data = new ImageCommandData() ;
  }
  
  ImageCommand::ImageCommand( const ImageCommand& cmd )
  {
    this->img_cmd_data = new ImageCommandData() ;
    
    *this->img_cmd_data = *cmd.img_cmd_data ;
  }

  ImageCommand::~ImageCommand()
  {
    delete this->img_cmd_data ;
  }
  
  ImageCommand& ImageCommand::operator =( const ImageCommand& cmd )
  {
    *this->img_cmd_data = *cmd.img_cmd_data ;
    
    return *this ;
  }
  
  void ImageCommand::setImage( const char* name )
  {
    data().image = name ;
  }

  void ImageCommand::setPosX( float pos )
  {
    data().x = pos ;
  }

  void ImageCommand::setPosY( float pos )
  {
    data().y = pos ;
  }

  void ImageCommand::setWidth( float width )
  {
    data().width = width ;
  }

  void ImageCommand::setHeight( float height )
  {
    data().height = height ;
  }

  void ImageCommand::setRotation( float rotation )
  {
    data().rot = rotation ;
  }

  float ImageCommand::posX() const
  {
    return data().x ;
  }

  float ImageCommand::posY() const
  {
    return data().y ;
  }

  float ImageCommand::width() const
  {
    return data().width ;
  }

  float ImageCommand::height() const
  {
    return data().height ;
  }

  float ImageCommand::rotation() const
  {
    return data().rot ;
  }

  const char* ImageCommand::image() const
  {
    return data().image.c_str() ;
  }

  ImageCommandData& ImageCommand::data()
  {
    return *this->img_cmd_data ;
  }

  const ImageCommandData& ImageCommand::data() const
  {
    return *this->img_cmd_data ;
  }

  SheetCommand::SheetCommand()
  {
    this->img_cmd_data = new SheetCommandData() ;
  }
  
  SheetCommand::SheetCommand( const SheetCommand& cmd )
  {
    this->img_cmd_data = new SheetCommandData() ;
    
    *this->img_cmd_data = *cmd.img_cmd_data ;
  }
  
  SheetCommand& SheetCommand::operator=( const SheetCommand& cmd )
  {
    *this->img_cmd_data = *cmd.img_cmd_data ;
    
    return *this ;
  }

  SheetCommand::~SheetCommand()
  {
    delete this->img_cmd_data ;
  }

  void SheetCommand::setSheet( const char* name )
  {
    data().image = name ;
  }

  void SheetCommand::setPosX( float pos )
  {
    data().x = pos ;
  }

  void SheetCommand::setPosY( float pos )
  {
    data().y = pos ;
  }

  void SheetCommand::setWidth( float width )
  {
    data().width = width ;
  }

  void SheetCommand::setHeight( float height )
  {
    data().height = height ;
  }

  void SheetCommand::setRotation( float rotation )
  {
    data().rot = rotation ;
  }

  void SheetCommand::setIndex( unsigned index )
  {
    data().index = index ;
  }

  float SheetCommand::posX() const
  {
    return data().x ;
  }

  float SheetCommand::posY() const
  {
    return data().y ;
  }

  float SheetCommand::width() const
  {
    return data().width ;
  }

  float SheetCommand::height() const
  {
    return data().height ;
  }

  float SheetCommand::rotation() const
  {
    return data().rot ;
  }

  const char* SheetCommand::sheet() const
  {
    return data().image.c_str() ;
  }

  unsigned SheetCommand::index() const
  {
    return data().index ;
  }

  SheetCommandData& SheetCommand::data()
  {
    return *this->img_cmd_data ;
  }

  const SheetCommandData& SheetCommand::data() const
  {
    return *this->img_cmd_data ;
  }
}