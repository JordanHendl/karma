#ifndef KAL_INTERFACE_H
#define KAL_INTERFACE_H

/** Object to interface and interact with the KAL Library.
 */
class KAL_Interface
{
  public:
    
    /** Default Constructor.
     */
    KAL_Interface() ;
    
    /** Deconstructor.
     */
    ~KAL_Interface() ;
    
    /** Method to initialize the KAL Library.
     * @note The base path is used as a basis for all Asset & JSON lookup.
     */
    void initialize() ;
    
    /** Method to shutdown the KAL Library's operations & free all allocated data..
     */
    void shutdown() ;
    
    /** Method to load asset pack ( in .kaudio format ).
     * @param path The asset pack to load.
     */
    void loadPack( const char* path ) ;
    
    /** Method to unload the currently loaded asset pack.
     */
    void unloadPack() ;

  private:
    
    /** Forward Declared structure to contain this object's data.
     */
    struct KAL_InterfaceData* kal_data ;
    
    /** Method to retrieve a reference to this object's internal data structure.
     * @return Reference to this object's internal data structure.
     */
    KAL_InterfaceData& data() ;
    
      /** Method to retrieve a const reference to this object's underlying data structure.
       * @return A const reference to this object's underlying data structure.
       */
    const KAL_InterfaceData& data() const ;
};

#endif

