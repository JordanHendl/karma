#include "casper_vulkan_device.h"
#include "casper_vulkan_validation_layer.h"
#include "casper_vulkan_surface.h"
#include <Bus.h>
#include <Signal.h>
#include <vulkan/vulkan.h>
#include <map>
#include <vector>
#include <memory>

namespace casper
{
  namespace vulkan
  {
    const std::vector<const char*> required_extensions = 
    {
      VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    static std::vector<VkPhysicalDevice> physical_devices ;

    void createDevice( const VkInstance& instance ) ;

    struct DeviceSignup
    {
      DeviceSignup()
      {
        data::module::Bus bus ;

        bus[ "CASPER_VULKAN_INSTANCE"].attach( &createDevice ) ;
      }
    };

    
    struct QueueFamily
    {
      uint32_t graphics ;
      uint32_t compute  ;
      uint32_t present  ;
      std::vector<uint32_t> families     ;
      unsigned              num_families ;

      QueueFamily() ;
      ~QueueFamily() ;
      bool isComplete() ;
      unsigned count() ;
    };

    struct DeviceData
    {
      typedef std::unique_ptr<QueueFamily> pQueueFamily ;

      pQueueFamily                   family   ;
      VkDevice                       v_device ;
      VkPhysicalDevice               p_device ;
      std::string                    name     ;
      VkQueue                        g_queue  ;
      VkQueue                        p_queue  ;
      DeviceData()
      {
        family = std::make_unique<QueueFamily>() ;
      }
    };

    typedef std::map<std::string, std::unique_ptr<casper::vulkan::Device>> DeviceMap ;
    static DeviceSignup dummy        ;
    static DeviceMap    device_map   ;


    void findDevices( const VkInstance& instance )
    {
      VkPhysicalDevice              device       ;
      unsigned                      count        ;

      if( physical_devices.size() == 0 )
      {
        vkEnumeratePhysicalDevices( instance, &count, nullptr ) ;
        
        if( count == 0 )
        {
          throw std::runtime_error( "Casper::Failed to find GPU with VULKAN support.") ;
        }
        
        physical_devices.resize( count ) ;
         
        vkEnumeratePhysicalDevices( instance, &count, physical_devices.data() ) ;
      }
    }

    void createDevice( const VkInstance& instance )
    {
      findDevices( instance ) ;
    }

    QueueFamily::QueueFamily()
    {
      this->graphics = UINT32_MAX ;
      this->compute  = UINT32_MAX ;
      this->present  = UINT32_MAX ;
    }

    QueueFamily::~QueueFamily()
    {
    }

    bool QueueFamily::isComplete()
    {
      return graphics != UINT32_MAX && compute != UINT32_MAX && present != UINT32_MAX ;
    } ;

    unsigned QueueFamily::count()
    {
      return this->num_families ;
    } ;

    Device::Device()
    {
      this->device_data = new DeviceData() ;
    }

    Device::~Device()
    {
      delete this->device_data ;
    }

    VkDevice Device::Device::device() const
    {
      return data().v_device ;
    }

    VkPhysicalDevice Device::physicalDevice() const
    {
      return data().p_device ;
    }
    
    const char* Device::name() const
    {
      return data().name.c_str() ;
    }

    unsigned Device::graphicsFamily() const 
    {
      return data().family->graphics ;
    }

    unsigned Device::computeFamily() const
    {
      return data().family->compute ;
    }

    unsigned Device::presentFamily() const
    {
      return data().family->present ;
    }

    void Device::findQueueFamilies( const Surface& surface )
    {
      typedef std::vector<VkQueueFamilyProperties> FamilyPropList ;

      FamilyPropList list    ;
      uint32_t       count   ;
      uint32_t       id      ;
      VkBool32       support ;

      vkGetPhysicalDeviceQueueFamilyProperties( data().p_device, &count, nullptr ) ;
      list.resize( count ) ;
      vkGetPhysicalDeviceQueueFamilyProperties( data().p_device, &count, list.data() ) ;
      
      id = 0 ;
      for (const auto& queueFamily : list ) 
      {
        vkGetPhysicalDeviceSurfaceSupportKHR( data().p_device, id, surface.surface(), &support ) ;

        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) 
        {
          data().family->graphics = id ;
        }

        if (queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT) 
        {
          data().family->compute = id ;
        }

        if( support )
        {
          data().family->present = id ;
        }

        data().family->families.push_back( id ) ;

        if( data().family->isComplete() )
        {
          return ;
        }

        id++ ;
      }

      data().family->num_families = id ; 
    }

    void Device::genVirtualDevice()
    {
      typedef std::vector<VkDeviceQueueCreateInfo> CreateInfos ;

      const float priority[1] = {0.0f} ; //TODO REMOVE and make configurable.
      VkDeviceCreateInfo       dev_info ;
      CreateInfos              infos     ;
      VkPhysicalDeviceFeatures features ;
      ValidationLayer          layers   ;

      features = {} ;
      dev_info = {} ;
      
      infos.resize( data().family->families.size() ) ;

      for( unsigned i = 0; i < data().family->families.size(); i++ )
      {
        infos[ i ].sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO ;
        infos[ i ].queueFamilyIndex = data().family->families[ i ]               ;
        infos[ i ].queueCount       = 1                                          ;
        infos[ i ].pQueuePriorities = priority                                   ;
        infos[ i ].pNext            = NULL                                       ;
      }

      dev_info.sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO ;
      dev_info.pQueueCreateInfos       = infos.data()                         ;
      dev_info.queueCreateInfoCount    = 1                                    ;
      dev_info.pEnabledFeatures        = &features                            ;
      dev_info.enabledExtensionCount   = required_extensions.size()           ;
			dev_info.ppEnabledExtensionNames = required_extensions.data()           ;
      dev_info.pNext                   = NULL                                 ;
      dev_info.ppEnabledLayerNames     = NULL                                 ;

      if( layers.supported() )
      {
        dev_info.enabledLayerCount   = layers.count() ;
        dev_info.ppEnabledLayerNames = layers.names() ;
      }

      if( vkCreateDevice( data().p_device, &dev_info, nullptr, &data().v_device ) != VK_SUCCESS )
      {
        throw std::runtime_error( "Failed to create Logical Device.") ;
      }
        
      vkGetDeviceQueue( data().v_device, data().family->graphics, 0, &data().g_queue ) ;
      vkGetDeviceQueue( data().v_device, data().family->present , 0, &data().p_queue ) ;
    }

    VkQueue Device::graphics() const
    {
      return data().g_queue ;
    }

    VkQueue Device::present() const
    {
      return data().p_queue ;
    }

    DeviceData& Device::data()
    {
      return *this->device_data ;
    }
    
    const DeviceData& Device::data() const
    {
      return *this->device_data ;
    }

    DeviceManager::DeviceManager()
    {

    }

    DeviceManager::~DeviceManager()
    {
      
    }

    const Device& DeviceManager::device( const char* surface_name )
    {
      return *device_map.at( std::string( surface_name ) ) ;
    }

    void DeviceManager::create( const Surface& surface, unsigned physical_device )
    {
      VkPhysicalDevice           p_device     ;
      VkPhysicalDeviceProperties prop         ;
      VkPhysicalDeviceFeatures   feat         ;

      p_device = physical_devices.at( physical_device ) ;

      vkGetPhysicalDeviceProperties( p_device, &prop ) ;
      vkGetPhysicalDeviceFeatures  ( p_device, &feat ) ;

      device_map.insert( { surface.name(), std::make_unique<Device>() } )                     ;
      device_map.at      ( surface.name() )->data().p_device = p_device                       ;
      device_map.at      ( surface.name() )->data().name     = std::string( prop.deviceName ) ;
      device_map.at      ( surface.name() )->findQueueFamilies( surface )                     ;
      device_map.at      ( surface.name() )->genVirtualDevice()                               ;
    }
  }
}