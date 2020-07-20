#ifndef KGL_VULKAN_CONTEXT
#define KGL_VULKAN_CONTEXT

/** Forward declared vulkan structures.
 */
namespace vk
{
  class Queue          ;
  class SurfaceKHR     ;
  class Instance       ;
  class SwapchainKHR   ;
  class CommandBuffer  ;
  class Device         ;
  class PhysicalDevice ;
  class ImageView      ;
  class Framebuffer    ;
}

namespace kgl
{
  namespace vk
  {
    /** Function to initialize the rendering library
     */ 
    void initialize() ;
    
    /** Function to retrieve whether or not the rendering library is initialized.
     * @return Whether or not the rendering library is initialized.
     */
    bool isInitialized() ;
    
    /** Function to shut down rendering library.
     */
    void shutdown() ;
    
    /** Function to set the base path of installation of software.
     * @param base_path The base path of the program.
     */
    void setBasePath( const char* base_path ) ;

    /** Function to recieve the base path of the program.
     * @return The base path of the program as set by the initialized function interface.
     */
    const char* basePath() ;

    /** Function to retrieve the vulkan instance for this program.
     */
    const ::vk::Instance instance() ;
    
    namespace render
    {
      class CommandBuffer ;

      class Context
      {
        public:
          
          /**
           */
          Context()  = default ;
          ~Context() = default ;
          
          /**
           * @param name
           * @param gpu
           * @param width
           * @param height
           */
          void addWindow( const char* name, unsigned gpu, unsigned width, unsigned height ) ;
          
          /**
           * @param gpu
           * @return 
           */
          const ::vk::Device virtualDevice( unsigned gpu ) const ;
          
          /**
           * @param gpu
           * @return 
           */
          const ::vk::PhysicalDevice physicalDevice( unsigned gpu ) const ;
          
          /**
           * @param gpu
           * @return 
           */
          const ::vk::Queue graphicsQueue( unsigned gpu ) const ;
          
          /**
           * @param count
           * @param buffers
           */
          void submit( const CommandBuffer& buffer ) ;
          
          /**
           * @param gpu
           * @return 
           */
          unsigned graphicsFamily( unsigned gpu ) ;
          
          /**
           * @param gpu
           * @return 
           */
          unsigned presentFamily( unsigned gpu ) ;
          
          /**
           * @param gpu
           * @return 
           */
          const ::vk::Queue presentQueue( unsigned gpu ) const ;
          
          /**
           * @param window_name
           * @return 
           */
          const ::vk::SurfaceKHR surface( const char* window_name ) const ;
          
          /**
           * @param window_name
           * @return 
           */
          const ::vk::SwapchainKHR swapchain( const char* window_name ) const ;
          
          /**
           * @param window_name
           * @return 
           */
          unsigned width( const char* window_name ) const ;
          
          /**
           * @param window_name
           * @return 
           */
          unsigned height( const char* window_name ) const ;
          
          /**
           * @param window_name
           * @return 
           */
          unsigned currentSwap( const char* window_name ) const ;

          /**
           * @param window_name
           * @return 
           */
          unsigned numFrameBuffers( const char* window_name ) const ;
          
          /**
           * @param window_name
           * @param id
           * @return 
           */
          const ::vk::Framebuffer framebuffer( const char* window_name, unsigned id ) const ;
      };
    }
    
    namespace compute
    {
      class CommandBuffer ;

      class Context
      {
        public:
          Context()  = default ;
          ~Context() = default  ;
          const  unsigned computeFamily( unsigned gpu ) const ;
          const ::vk::Device virtualDevice( unsigned gpu ) const ;
          const ::vk::PhysicalDevice physicalDevice( unsigned gpu ) const ;
          const ::vk::Queue computeQueue( unsigned gpu ) const ;
          const void submit( const CommandBuffer& buffer ) ;
      };
    }
  }
}
#endif
