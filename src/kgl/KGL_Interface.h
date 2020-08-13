#ifndef KGL_INTERFACE_H
#define KGL_INTERFACE_H

/** Object to interface and interact with the KGL Library.
 */
class KGL_Interface
{
  public:
    
    /** Default Constructor.
     */
    KGL_Interface() ;
    
    /** Deconstructor.
     */
    ~KGL_Interface() ;
    
    /** Method to initialize the KGL Library.
     * @param base_path The base path of the project. 
     * @note The base path is used as a basis for all Asset & JSON lookup.
     */
    void initialize( const char* base_path ) ;
    
    /** Method to shutdown the KGL Library's operations & free all allocated data..
     */
    void shutdown() ;
    
    /** Method to set the current window KGL will listen to for interaction.
     */
    void setCurrentWindow( const char* name ) ;
    
    /** Method to load asset pack ( in .krender format ).
     * @param path The asset pack to load.
     */
    void loadPack( const char* path ) ;
    
    /** Method to unload the currently loaded asset pack.
     */
    void unloadPack() ;
    
    /** Method to kick operation of the KGL Graphics library.
     */
    void start() ;
    
    /** Method to poll events from the currently selected window.
     * @note To listen for events, attach to kgl::input using the @Bus object.
     */
    void pollEvents() ;

  private:
    
    /** Forward Declared structure to contain this object's data.
     */
    struct KGL_InterfaceData* kgl_data ;
    
    /** Method to retrieve a reference to this object's internal data structure.
     * @return Reference to this object's internal data structure.
     */
    KGL_InterfaceData& data() ;
    
      /** Method to retrieve a const reference to this object's underlying data structure.
       * @return A const reference to this object's underlying data structure.
       */
    const KGL_InterfaceData& data() const ;
};

#endif

