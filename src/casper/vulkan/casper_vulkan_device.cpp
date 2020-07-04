#include "casper_vulkan_device.h"
#include "casper_vulkan_validation_layer.h"
#include "casper_vulkan_surface.h"
#include "casper_vulkan_error_handler.h"
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
    #pragma region // Static Function Declarations

    static const std::vector<const char*> required_extensions = 
    {
      VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    static std::vector<VkPhysicalDevice> physical_devices ;

    static void createDevice( VkInstance instance ) ;

    #pragma endregion // Static Function Declarations

    #pragma region // Data structure Declarations

    struct DeviceSignup
    {
      DeviceSignup()
      {
        data::module::Bus bus ;

        bus[ "CASPER_VULKAN_INSTANCE" ].attach(&createDevice) ;
      }
    };
    static DeviceSignup signup ;

    
    struct QueueFamily
    {
      uint32_t              graphics     ;
      uint32_t              compute      ;
      uint32_t              present      ;
      std::vector<uint32_t> families     ;
      unsigned              num_families ;

      QueueFamily()
      {
        this->compute  = UINT32_MAX ;
        this->graphics = UINT32_MAX ;
        this->present  = UINT32_MAX ;
      }
      bool isComplete() ;
      unsigned count() ;
    };

    struct DeviceData
    {
      QueueFamily      family   ;
      VkDevice         v_device ;
      VkPhysicalDevice p_device ;
      std::string      name     ;
      VkQueue          g_queue  ;
      VkQueue          p_queue  ;
      VkQueue          c_queue  ;

      DeviceData() = default ;
      void operator=( const DeviceData data ) ;
      void findQueueFamilies() ;
      void findQueueFamilies( const Surface& surface ) ;
      void genVirtualDevice() ;
    };

    #pragma endregion // Data structure Declarations
    
    #pragma region // Static Function Definitions

    void createDevice( VkInstance instance )
    {
      VkPhysicalDevice device ;
      unsigned         count  ;

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

    #pragma endregion // Static Function Definitions

    #pragma region // DeviceData Function Definitions

    void DeviceData::operator=( const DeviceData data )
    {
      this->family   = data.family   ;
      this->g_queue  = data.g_queue  ;
      this->name     = data.name     ;
      this->p_device = data.p_device ;
      this->v_device = data.v_device ;
    }

    void DeviceData::findQueueFamilies( const Surface& surface ) 
    {
      typedef std::vector<VkQueueFamilyProperties> FamilyPropList ;

      FamilyPropList list    ;
      uint32_t       count   ;
      uint32_t       id      ;
      VkBool32       support ;

      vkGetPhysicalDeviceQueueFamilyProperties( this->p_device, &count, nullptr ) ;
      list.resize( count ) ;
      vkGetPhysicalDeviceQueueFamilyProperties( this->p_device, &count, list.data() ) ;
      
      id      = 0     ;
      support = false ;
      for (const auto& queueFamily : list ) 
      {
        HANDLE_ERROR( vkGetPhysicalDeviceSurfaceSupportKHR( this->p_device, id, surface.surface(), &support ) ) ;

        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) 
        {
          this->family.graphics = id ;
        }

        if (queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT) 
        {
          this->family.compute = id ;
        }

        if( support )
        {
          this->family.present = id ;
        }

        this->family.families.push_back( id ) ;

        if( this->family.isComplete() )
        {
          return ;
        }

        id++ ;
      }

      this->family.num_families = id ; 
    }

    void casper::vulkan::DeviceData::findQueueFamilies()
    {
      typedef std::vector<VkQueueFamilyProperties> FamilyPropList ;

      FamilyPropList list    ;
      uint32_t       count   ;
      uint32_t       id      ;
      VkBool32       support ;

      vkGetPhysicalDeviceQueueFamilyProperties( this->p_device, &count, nullptr ) ;
      list.resize( count ) ;
      vkGetPhysicalDeviceQueueFamilyProperties( this->p_device, &count, list.data() ) ;
      
      id = 0 ;
      for (const auto& queueFamily : list ) 
      {
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) 
        {
          this->family.graphics = id ;
        }

        if (queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT) 
        {
          this->family.compute = id ;
        }

        this->family.families.push_back( id ) ;

        if( this->family.isComplete() )
        {
          return ;
        }

        id++ ;
      }

      this->family.num_families = id ; 
    }

    void casper::vulkan::DeviceData::genVirtualDevice()
    {
      typedef std::vector<VkDeviceQueueCreateInfo> CreateInfos ;

      const float priority[1] = {0.0f} ; //TODO REMOVE and make configurable.
      VkDeviceCreateInfo       dev_info ;
      CreateInfos              infos    ;
      VkPhysicalDeviceFeatures features ;
      ValidationLayer          layers   ;

      features = {} ;
      dev_info = {} ;
      
      infos.resize( this->family.families.size() ) ;

      for( unsigned i = 0; i < this->family.families.size(); i++ )
      {
        infos[ i ].sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO ;
        infos[ i ].queueFamilyIndex = this->family.families[ i ]                 ;
        infos[ i ].queueCount       = 1                                          ;
        infos[ i ].pQueuePriorities = priority                                   ;
        infos[ i ].pNext            = NULL                                       ;
      }
      
      features.samplerAnisotropy = true ;
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

      if( vkCreateDevice( this->p_device, &dev_info, nullptr, &this->v_device ) != VK_SUCCESS )
      {
        throw std::runtime_error( "Failed to create Logical Device.") ;
      }
        
      vkGetDeviceQueue( this->v_device, this->family.graphics, 0, &this->g_queue ) ;
      vkGetDeviceQueue( this->v_device, this->family.present , 0, &this->p_queue ) ;
      vkGetDeviceQueue( this->v_device, this->family.compute , 0, &this->c_queue ) ;
    }

    #pragma endregion // DeviceData Function Definitions

    #pragma region // QueueFamily Function Definitions

    bool QueueFamily::isComplete()
    {
      return graphics != UINT32_MAX && compute != UINT32_MAX && present != UINT32_MAX ;
    } ;

    unsigned QueueFamily::count()
    {
      return this->num_families ;
    } ;

    #pragma endregion // QueueFamily Function Definitions

    #pragma region // Device Function Definitions

    Device::Device()
    {
      this->device_data = new DeviceData() ;
    }

    Device::~Device()
    {
      delete this->device_data ;
    }

    Device::Device( const Device& device )
    {
      this->device_data = new DeviceData() ;
      
      *this->device_data = *device.device_data ;
    }

    void Device::operator=( const Device& device )
    {
      *this->device_data = *device.device_data ;
    }

    void Device::initialize( unsigned device )
    {
      VkPhysicalDevice           p_device ;
      VkPhysicalDeviceProperties prop     ;
      VkPhysicalDeviceFeatures   feat     ;

      p_device = physical_devices.at( device ) ;

      vkGetPhysicalDeviceProperties( p_device, &prop ) ;
      vkGetPhysicalDeviceFeatures  ( p_device, &feat ) ;

      data().p_device = p_device ;
      data().name     = prop.deviceName ;

      data().findQueueFamilies() ;
      data().genVirtualDevice () ;
    }

    void Device::initialize( const casper::vulkan::Surface& surface, const VkInstance& instance, unsigned device )
    {
      VkPhysicalDevice           p_device ;
      VkPhysicalDeviceProperties prop     ;
      VkPhysicalDeviceFeatures   feat     ;

      p_device = physical_devices.at( device ) ;

      vkGetPhysicalDeviceProperties( p_device, &prop ) ;
      vkGetPhysicalDeviceFeatures  ( p_device, &feat ) ;

      data().p_device = p_device ;
      data().name     = prop.deviceName ;

      data().findQueueFamilies( surface ) ;
      data().genVirtualDevice (         ) ;
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
      return data().family.graphics ;
    }

    unsigned Device::computeFamily() const
    {
      return data().family.compute ;
    }

    unsigned Device::presentFamily() const
    {
      return data().family.present ;
    }

    VkQueue Device::graphics() const
    {
      return data().g_queue ;
    }

    VkQueue Device::present() const
    {
      return data().p_queue ;
    }

    VkQueue Device::compute() const
    {
      return data().c_queue ;
    }

    DeviceData& Device::data()
    {
      return *this->device_data ;
    }
    
    const DeviceData& Device::data() const
    {
      return *this->device_data ;
    }

    #pragma endregion // Device Function Definitions
  }
}