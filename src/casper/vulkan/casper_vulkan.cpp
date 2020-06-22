#include "casper_vulkan.h"
#include "casper_vulkan_validation_layer.h"
#include <Pool.h>
#include <PoolData.h>
#include <Bus.h>
#include <Signal.h>
#include <vulkan/vulkan.h>
#include <vector>
#include <cstring>
#include <iostream>

namespace casper
{
    typedef std::vector<const char*>           ExtensionList ;
    typedef std::vector<VkExtensionProperties> PropertyList  ;

    VkResult CreateDebugUtilsMessengerEXT( VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger ) 
    {
      auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr( instance, "vkCreateDebugUtilsMessengerEXT" ) ;
      
      if (func != nullptr)
      {
          return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
      }
      else 
      {
          return VK_ERROR_EXTENSION_NOT_PRESENT;
      }
    }
    
    void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) 
    {
        auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT") ;
        if (func != nullptr) 
        {
            func(instance, debugMessenger, pAllocator);
        }
    }
  
    static  VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
            VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
            VkDebugUtilsMessageTypeFlagsEXT messageType,
            const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
            void* pUserData )
    {
      std::cerr << "VULKAN::Validation::" << pCallbackData->pMessage << "\n" <<std::endl;
      return VK_FALSE;
    }

    VkApplicationInfo makeApplicationInfo()
    {
        VkApplicationInfo info ;

        info.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO ;
        info.pApplicationName   = "Casper_Test"                      ;
        info.applicationVersion = VK_MAKE_VERSION( 1, 0, 0 )         ;
        info.pEngineName        = "CASPER"                           ;
        info.engineVersion      = VK_MAKE_VERSION( 1, 0, 0 )         ;
        info.apiVersion         = VK_API_VERSION_1_2                 ;
        info.pNext              = NULL                               ;
        
        return info ;
    }
    ExtensionList makeExtensionList()
    {
        PropertyList  plist ;
        ExtensionList list  ;
        uint32_t      count ;

        vkEnumerateInstanceExtensionProperties( NULL, &count, NULL ) ;

        list.resize( count ) ;
        plist.resize( count ) ;

        vkEnumerateInstanceExtensionProperties( NULL, &count, plist.data() ) ;

        for( unsigned i = 0; i < plist.size(); i++ )
        {
          VkExtensionProperties prop = plist[ i ] ;
          char* stage = new char[ VK_MAX_EXTENSION_NAME_SIZE ] ;
          std::memcpy( stage, prop.extensionName, VK_MAX_EXTENSION_NAME_SIZE ) ;
          list[ i ] = stage ;
        }

        return list ;
    }

    VkDebugUtilsMessengerCreateInfoEXT makeDebugInfo()
    {
      VkDebugUtilsMessengerCreateInfoEXT info ;
      
      info = {};
      info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
      info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
      info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
      info.pfnUserCallback = debugCallback;
      
      return info ;
    }

    VkInstance makeInstance()
    {
      VkInstance                         instance   ;
      VkApplicationInfo                  app_info   ;
      ExtensionList                      list       ;
      VkDebugUtilsMessengerCreateInfoEXT debug_info ;
      VkInstanceCreateInfo               info       ;
      casper::vulkan::ValidationLayer    layer      ;

      app_info   = makeApplicationInfo() ;
      list       = makeExtensionList()   ;
      debug_info = makeDebugInfo()       ;
      info       = {}                    ;

      if( layer.supported() )
      {
        info.enabledLayerCount   = ( layer.count() ) ;
        info.ppEnabledLayerNames = ( layer.names() ) ;
        list.push_back( VK_EXT_DEBUG_UTILS_EXTENSION_NAME ) ;
        
        info.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debug_info ;
      }
      else
      {
        info.enabledLayerCount = ( 0 )   ;
        info.pNext             = nullptr ;
      }
        
      info.sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO ;
      info.pApplicationInfo        = &app_info                              ;
      info.enabledExtensionCount   = list.size()                            ;
      info.ppEnabledExtensionNames = list.data()                            ;

     if( vkCreateInstance( &info, nullptr, &instance ) != VK_SUCCESS )
     {
       throw std::runtime_error( "Failed to create vulkan instance!" ) ; 
     }

      return instance ;
    }

  struct VulkanData
  {
    VkInstance               instance ;
    ExtensionList            list     ;
    VkDebugUtilsMessengerEXT debug    ;
    bool                     init     ;
    data::module::Pool       pool     ;

    VulkanData()
    {
      this->init = false ;
    }
  };

  Vulkan::Vulkan()
  {
    this->vk_data = new VulkanData() ;
  }

  Vulkan::~Vulkan()
  {
    delete( this->vk_data ) ;
  }

  void Vulkan::initialize()
  {
     VkInstanceCreateInfo                info       ;
     vulkan::ValidationLayer             layer      ;
     VkDebugUtilsMessengerCreateInfoEXT  debug_info ;
     data::module::Bus                   bus        ;
     data::module::Pool                  pool       ;
     data().instance = makeInstance() ;

     debug_info = makeDebugInfo()  ;


      if( layer.supported() )
      {
        if ( CreateDebugUtilsMessengerEXT( data().instance, &debug_info, nullptr, &data().debug ) != VK_SUCCESS )
        {
          throw std::runtime_error( "Failed to set up vulkan debug messenger!" ) ;
        }
      }
      
     pool[ "CASPER_VULKAN_INSTANCE" ].set ( data().instance ) ; 
     bus [ "CASPER_VULKAN_INSTANCE" ].emit( data().instance ) ;
     
     data().init = true ;
  }

  bool Vulkan::initialized()
  {
    return data().init ;
  }

  void Vulkan::shutDown()
  {
    
  }

  VulkanData& Vulkan::data()
  {
    return *this->vk_data ;
  }

  const VulkanData& Vulkan::data() const
  {
    return *this->vk_data ;
  }
}