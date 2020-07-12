#ifndef KGL_VK_MODULE
#define KGL_VK_MODULE

namespace kgl
{
  namespace vk
  {
    /** Class for describing a GPU Vulkan Render/Compute Pass.
     */
    class Module
    { 
      public:
        /** Default Constructor. Initializes this object's data.
         */
        Module() ;
        
        /** Virtual deconstructor. Needed for inheritance.
         */
        virtual ~Module() ;
        
        /** Method to initialize this module after being configured.
         */
        virtual void initialize() = 0 ;
        
        /**
         * @param bus_id The channel of bus to subscribe inputs on.
         * @param gpu The gpu to use for all GPU operations.
         */
        virtual void subscribe( unsigned bus_id ) ;

        /** Method to shut down this object's operation.
         */
        virtual void shutdown() = 0 ;
        
        /** Method to execute a single instance of this module's operation.
         */
        virtual void execute() = 0 ;

        /** Method to kick off operation of this module.
         */
        void kick() ;
        
        /** Method to start operations of this module.
         * @return 
         */
        void start() ;
        
        /** Method to stop operation of this module.
         * @return Whether the module is stopped or not.
         */
        bool stop() ;
        
        /** Method to clear all commands allocated to this module.
         */
        virtual void clear() = 0 ;
        
        /** Method to set the name of this module.
         * @param name
         */
        void setName( const char* name ) ;
        
        /** Method to retrieve the name of this module.
         * @return The name of this module.
         */
        const char* name() const ;

      private:
        struct ModuleData* module_data ;
        
        /** Method to retrieve a reference to this object's internal data structure.
         * @return Reference to this object's internal data structure.
         */
        ModuleData& data() ;

        /** Method to retrieve a const-reference to this object's internal data structure.
         * @return Const-reference to this object's internal data structure.
         */
        const ModuleData& data() const ;
    };
  }
}
#endif