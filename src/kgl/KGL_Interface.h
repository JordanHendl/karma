#ifndef KGL_INTERFACE_H
#define KGL_INTERFACE_H

class KGL_Interface
{
  public:
    KGL_Interface() ;
    ~KGL_Interface() ;
    void initialize( const char* base_path ) ;
    void shutdown() ;
    void setCurrentWindow( const char* name ) ;
    void loadPack( const char* path ) ;
    void unloadPack() ;
    void start() ;
    void pollEvents() ;
//    void present() ;
  private:
    struct KGL_InterfaceData* kgl_data ;
    KGL_InterfaceData& data() ;
    const KGL_InterfaceData& data() const ;
};

#endif /* KGL_INTERFACE_H */

