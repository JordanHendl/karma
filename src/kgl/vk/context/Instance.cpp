#include "Instance.h"
#include "Validation.h"
#include "Library.h"
#include <Bus.h>
#include <Signal.h>
#include <vulkan/vulkan.hpp>
#include <vector>
#include <iostream>

namespace kgl
{
  namespace vk
  {
    typedef std::vector<const char*>               ExtensionList ;
    typedef std::vector<::vk::ExtensionProperties> PropertyList  ;
    
    static  VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
            VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
            VkDebugUtilsMessageTypeFlagsEXT messageType,
            const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
            void* pUserData )
    {
      std::cerr << "VULKAN::Validation::" << pCallbackData->pMessage << "\n" <<std::endl;
      return VK_FALSE;
    }

    struct InstanceData
    {
      ::vk::Instance instance ;
      
      ::vk::ApplicationInfo makeAppInfo() const ;
      ExtensionList makeExtensionList() const ;
      ::vk::DebugUtilsMessengerCreateInfoEXT makeDebugInfo() const ;
    };

    ::vk::ApplicationInfo InstanceData::makeAppInfo() const
    {
      ::vk::ApplicationInfo info ;
      
      info.setPApplicationName  ( "KGL VULKAN"               ) ;
      info.setPEngineName       ( "Karma"                    ) ;
      info.setApplicationVersion( VK_MAKE_VERSION( 1, 0, 0 ) ) ;
      info.setEngineVersion     ( VK_MAKE_VERSION( 1, 0, 0 ) ) ;
      info.setApiVersion        ( VK_API_VERSION_1_2         ) ;
      
      return info ;
    }

    ExtensionList InstanceData::makeExtensionList() const
    {
        PropertyList  plist ;
        ExtensionList list  ;

        plist = ::vk::enumerateInstanceExtensionProperties() ;

        list.resize( plist.size() ) ;

        for( unsigned i = 0; i < plist.size(); i++ )
        {
          ::vk::ExtensionProperties prop = plist[ i ] ;
          char* stage = new char[ VK_MAX_EXTENSION_NAME_SIZE ] ;
          std::memcpy( stage, prop.extensionName, VK_MAX_EXTENSION_NAME_SIZE ) ;
          list[ i ] = stage ;
        }

        return list ;
    }

    ::vk::DebugUtilsMessengerCreateInfoEXT InstanceData::makeDebugInfo() const
    {
      const auto severity_bits = ::vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose | ::vk::DebugUtilsMessageSeverityFlagBitsEXT::eError  | ::vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning ;
      const auto message_type  = ::vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral     | ::vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation | ::vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance ;
      
      ::vk::DebugUtilsMessengerCreateInfoEXT info ;
      
      info.setMessageSeverity( severity_bits ) ;
      info.setMessageType    ( message_type  ) ;
      info.setPfnUserCallback( debugCallback ) ;
      
      return info ;
    }

    Instance::Instance()
    {
      this->instance_data = new InstanceData() ;
    }

    Instance::~Instance()
    {
      delete this->instance_data ;
    }

    void Instance::initialize( bool validation_layers_enabled )
    {
      ::vk::ApplicationInfo                  app_info   ;
      ExtensionList                          ext_list   ;
      PropertyList                           prop_list  ;
      ::vk::DebugUtilsMessengerCreateInfoEXT debug_info ;
      ::vk::InstanceCreateInfo               info       ;
      ::kgl::vk::Validation                  layer      ;
      ::data::module::Bus                    bus        ;
      

      app_info   = data().makeAppInfo()       ;
      ext_list   = data().makeExtensionList() ;
      debug_info = data().makeDebugInfo()     ;
      
      if( layer.supported() )
      {
        info.setEnabledLayerCount  ( layer.count() ) ;
        info.setPpEnabledLayerNames( layer.names() ) ;
        info.setPNext              ( (VkDebugUtilsMessengerCreateInfoEXT*)&debug_info ) ;
        
        ext_list.push_back( VK_EXT_DEBUG_UTILS_EXTENSION_NAME ) ;
      }
      else
      {
        info.setEnabledLayerCount( 0 ) ;
      }
      
      info.setPApplicationInfo       ( &app_info       ) ;
      info.setEnabledExtensionCount  ( ext_list.size() ) ;
      info.setPpEnabledExtensionNames( ext_list.data() ) ;
      
      
      data().instance = ::vk::createInstance( info, nullptr ) ;
      
      // Emit instance for physical device creation.
      bus( "KGL_VULKAN_INSTANCE" ).emit( data().instance ) ;
    }
    
    void Instance::reset()
    {
     //TODO shutdown
    }

    const ::vk::Instance Instance::instance() const
    {
      return data().instance ;
    }

    InstanceData& Instance::data()
    {
      return *this->instance_data ;
    }

    const InstanceData& Instance::data() const
    {
      return *this->instance_data ;
    }

  }
}
