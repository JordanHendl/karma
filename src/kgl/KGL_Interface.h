#ifndef KGL_INTERFACE_H
#define KGL_INTERFACE_H

class ModelSubmition
{
  public:
    ModelSubmition() ;
    ~ModelSubmition() ;
    void setModelName( const char* name ) ;
    void setSubmitionName( const char* name ) ;
    void setPosition( float x, float y, float z ) ;
    void setSize( float x, float y, float z ) ;
    void setRotationEuler( float x, float y, float z ) ;
    void setAnimation( const char* animation_name ) ;
  private:
    struct ModelSubmitionData *sub_data ;
    ModelSubmitionData& data() ;
    const ModelSubmitionData& data() const ;
};

class ImageSubmition
{
  public:
    ImageSubmition() ;
    ~ImageSubmition() ;
    void setImageName( const char* name ) ;
    void addGraph( const char* graph ) ;
    void setSubmitionName( const char* name ) ;
    void setPosition( float x, float y, float z ) ;
    void setSize( float x, float y, float z ) ;
    void setRotationEuler( float x, float y, float z ) ;
  private:
    struct ImageSubmitionData *sub_data ;
    ImageSubmitionData& data() ;
    const ImageSubmitionData& data() const ;
};

class KGL_Interface
{
  public:
    KGL_Interface() ;
    ~KGL_Interface() ;
    void initialize( const char* base_path ) ;
    void setCurrentWindow( const char* name ) ;
    void loadPack( const char* path ) ;
    void unlockPack() ;
    void drawModel( const ModelSubmition& submit ) ;
    void drawImage( const ImageSubmition& submit ) ;
    void drawParticle( const char* name, float cx, float cy, unsigned amount ) ;
    void clear() ;
    void present() ;
  private:
    struct KGL_InterfaceData* kgl_data ;
    KGL_InterfaceData& data() ;
    const KGL_InterfaceData& data() const ;
};

#endif /* KGL_INTERFACE_H */

