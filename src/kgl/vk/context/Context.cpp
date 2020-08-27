#include "Context.h"
#include "Window.h"
#include "Device.h"
#include "Surface.h"
#include "Instance.h"
#include "Swapchain.h"
#include "RenderPass.h"
#include "CommandBuffer.h"
#include <vulkan/vulkan.hpp>
#include <string>
#include <map>

namespace kgl
{
  namespace vk
  {
    typedef uint32_t                                 GPU        ;
    typedef std::string                              WindowName ;
    typedef std::map<WindowName, ::kgl::vk::Window*> WindowMap  ;
    typedef std::map<GPU       , ::kgl::vk::Device>  DeviceMap  ;
    
    struct VulkanContextData
    {
      WindowMap                        window_map    ;
      DeviceMap                        device_map    ;
      ::kgl::vk::Instance              instance      ;
      bool                             initialized   ;
      ::kgl::vk::render::CommandBuffer render_buffer ;
      std::string                      base_path     ;

      /**
       */
      VulkanContextData() ;
      
      /**
       */
      void initialize() ;
      
      /**
       */
      void shutdown() ;
    };
    
    static VulkanContextData vk_context ;

    const ::vk::Instance instance()
    {
      return ::kgl::vk::vk_context.instance.instance() ;
    }

    void initialize()
    {
      ::kgl::vk::vk_context.initialize() ;
    }
    
    bool isInitialized()
    {
      return ::kgl::vk::vk_context.initialized ;
    }

    void shutdown()
    {
      ::kgl::vk::vk_context.shutdown() ;
    }
    
    void setBasePath( const char* base_path )
    {
      ::kgl::vk::vk_context.base_path = base_path ;
    }

    const char* basePath()
    {
      return ::kgl::vk::vk_context.base_path.c_str() ;
    }

    VulkanContextData::VulkanContextData()
    {
      this->initialized = false ;
    }

    void VulkanContextData::initialize()
    {
      if( !this->initialized ) 
      {
        this->instance.initialize() ;

        this->initialized = true ;
      }
    }

    void VulkanContextData::shutdown()
    {
      if( this->initialized )
      {
        this->instance.reset() ;

        this->initialized = false ;
      }
    }

    namespace render
    {
      void Context::addWindow( const char* name, const char* display_name, unsigned gpu, unsigned width, unsigned height, bool resizable, bool borderless, unsigned num_sems )
      {
        const auto iter = ::kgl::vk::vk_context.window_map.find( name ) ;
        
        if( iter == ::kgl::vk::vk_context.window_map.end() )
        {
          ::kgl::vk::vk_context.window_map.insert( { std::string( name ), nullptr } )                ;
          ::kgl::vk::vk_context.window_map[ name ] = new ::kgl::vk::Window()                         ;
          ::kgl::vk::vk_context.window_map[ name ]->initialize( name, display_name, gpu, width, height, resizable, borderless, num_sems ) ;
          
          if( ::kgl::vk::vk_context.device_map.find( gpu ) == ::kgl::vk::vk_context.device_map.end() )
          {
            ::kgl::vk::vk_context.device_map.insert( { gpu, ::kgl::vk::vk_context.window_map[ name ]->device() } ) ;
          }
        }
      }
      
      const ::vk::Device Context::virtualDevice( unsigned gpu ) const
      {
        static ::vk::Device dummy ;
  
        auto iter = ::kgl::vk::vk_context.device_map.find( gpu ) ;
        
        if( iter != ::kgl::vk::vk_context.device_map.end() ) return iter->second.device() ;
        
        return dummy ;
      }
  
      const ::vk::PhysicalDevice Context::physicalDevice( unsigned gpu ) const
      {
        static ::vk::PhysicalDevice dummy ;
  
        auto iter = ::kgl::vk::vk_context.device_map.find( gpu ) ;
        
        if( iter != ::kgl::vk::vk_context.device_map.end() ) return iter->second.physicalDevice() ;
        
        return dummy ;
      }
  
      const ::vk::Queue Context::graphicsQueue( unsigned gpu ) const
      {
        static ::vk::Queue dummy ;
        
        auto iter = ::kgl::vk::vk_context.device_map.find( gpu ) ;
        
        if( iter != ::kgl::vk::vk_context.device_map.end() ) return iter->second.graphics() ;
        
        return dummy ;
      }
  
      unsigned Context::graphicsFamily( unsigned gpu ) const
      {
        static const unsigned dummy = 0;
        
        auto iter = ::kgl::vk::vk_context.device_map.find( gpu ) ;
        
        if( iter != ::kgl::vk::vk_context.device_map.end() ) return iter->second.graphicsFamily() ;
        
        return dummy ;
      }

      unsigned Context::presentFamily( unsigned gpu )
      {
        static const unsigned dummy = 0 ;
        
        auto iter = ::kgl::vk::vk_context.device_map.find( gpu ) ;
        
        if( iter != ::kgl::vk::vk_context.device_map.end() ) return iter->second.presentFamily() ;
        
        return dummy ;
      }
      
      unsigned Context::width( const char* window_name ) const
      {
        static const unsigned dummy = 0 ;
        auto iter = ::kgl::vk::vk_context.window_map.find( window_name ) ;
        
        if( iter != ::kgl::vk::vk_context.window_map.end() ) return iter->second->chain().width() ;
        
        return dummy ;
      }
      
      ::kgl::vk::Window& Context::window( const char* window_name )
      {
        static ::kgl::vk::Window dummy ;
        auto iter = ::kgl::vk::vk_context.window_map.find( window_name ) ;
        
        if( iter != ::kgl::vk::vk_context.window_map.end() ) return *iter->second ;
        
        return dummy ;
      }
      
      void Context::start( const char* window_name ) const
      {
        auto iter = ::kgl::vk::vk_context.window_map.find( window_name ) ;
        
        if( iter != ::kgl::vk::vk_context.window_map.end() ) return iter->second->start() ;
      }

      unsigned Context::height( const char* window_name ) const
      {
        static const unsigned dummy = 0 ;
        auto iter = ::kgl::vk::vk_context.window_map.find( window_name ) ;
        
        if( iter != ::kgl::vk::vk_context.window_map.end() ) return iter->second->chain().height() ;
        
        return dummy ;
      }
      
      unsigned Context::currentSwap( const char* window_name ) const
      {
        static const unsigned dummy = 0 ;
        auto iter = ::kgl::vk::vk_context.window_map.find( window_name ) ;
        
        if( iter != ::kgl::vk::vk_context.window_map.end() ) return iter->second->currentSwap() ;
        
        return dummy ;
      }
      
      const ::vk::Queue Context::presentQueue( unsigned gpu ) const
      {
        static const ::vk::Queue dummy ;
  
        auto iter = ::kgl::vk::vk_context.device_map.find( gpu ) ;
        
        if( iter != ::kgl::vk::vk_context.device_map.end() ) return iter->second.present() ;
        
        return dummy ;
      }

      const ::vk::SurfaceKHR Context::surface( const char* window_name ) const
      {
        static const ::vk::SurfaceKHR dummy ;
  
        auto iter = ::kgl::vk::vk_context.window_map.find( window_name ) ;
        
        if( iter != ::kgl::vk::vk_context.window_map.end() ) return iter->second->surface() ;
        
        return dummy ;
      }
  
      const ::vk::SwapchainKHR Context::swapchain( const char* window_name ) const
      {
        static const ::vk::SwapchainKHR dummy ;
        auto iter = ::kgl::vk::vk_context.window_map.find( window_name ) ;
        
        if( iter != ::kgl::vk::vk_context.window_map.end() ) return iter->second->chain().chain() ;
        
        return dummy ;
      }
  
      unsigned Context::numFrameBuffers( const char* window_name ) const
      {
        static unsigned dummy = 0 ;
  
        auto iter = ::kgl::vk::vk_context.window_map.find( window_name ) ;
        
        if( iter != ::kgl::vk::vk_context.window_map.end() ) return iter->second->chain().numBuffers() ;
        
        return dummy ;
      }
  
      const ::vk::Framebuffer Context::framebuffer( const char* window_name, unsigned id ) const
      {
        static ::vk::Framebuffer dummy ;
  
        auto iter = ::kgl::vk::vk_context.window_map.find( window_name ) ;
        
        if( iter != ::kgl::vk::vk_context.window_map.end() ) return iter->second->chain().buffer( id ) ;
        
        return dummy ;
      }
    }
    
    namespace compute
    {
      const  unsigned Context::computeFamily( unsigned gpu ) const
      {
        static const unsigned dummy = 0 ;
  
        auto iter = ::kgl::vk::vk_context.device_map.find( gpu ) ;
        
        if( iter != ::kgl::vk::vk_context.device_map.end() ) return iter->second.computeFamily() ;
        
        return dummy ;
      }

      const ::vk::Device Context::virtualDevice( unsigned gpu ) const
      {
        static const ::vk::Device dummy ;
  
        auto iter = ::kgl::vk::vk_context.device_map.find( gpu ) ;
        
        if( iter != ::kgl::vk::vk_context.device_map.end() ) return iter->second.device() ;
        
        return dummy ;
      }

      const ::vk::PhysicalDevice Context::physicalDevice( unsigned gpu ) const
      {
        static const ::vk::PhysicalDevice dummy ;
  
        auto iter = ::kgl::vk::vk_context.device_map.find( gpu ) ;
        
        if( iter != ::kgl::vk::vk_context.device_map.end() ) return iter->second.physicalDevice() ;
        
        return dummy ;
      }

      const ::vk::Queue Context::computeQueue( unsigned gpu ) const
      {
        static const ::vk::Queue dummy ;
  
        auto iter = ::kgl::vk::vk_context.device_map.find( gpu ) ;
        
        if( iter != ::kgl::vk::vk_context.device_map.end() ) return iter->second.compute() ;
        
        return dummy ;
      }
      
      const unsigned Context::deviceCount() const
      {
        return 1 ;
      }
      
      const bool Context::validDevice( unsigned gpu )
      {
        return true ;
      }

      const void Context::submit( const CommandBuffer& buffer )
      {
      
      }
    }
  }
}