#include "Device.h"
#include "Context.h"
#include "Validation.h"
#include "Library.h"
#include <Bus.h>
#include <Signal.h>
#include <vulkan/vulkan.hpp>
#include <map>
#include <vector>
#include <memory>

namespace kgl
{
  namespace vk
  {

    static const std::vector<const char*> required_extensions = 
    {
      VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    static std::vector<::vk::PhysicalDevice> physical_devices ;

    static void createDevice( ::vk::Instance instance ) ;

    struct DeviceSignup
    {
      DeviceSignup()
      {
        data::module::Bus bus ;

        bus( "KGL_VULKAN_INSTANCE" ).attach( &createDevice ) ;
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
      QueueFamily          family   ;
      ::vk::Device         v_device ;
      ::vk::PhysicalDevice p_device ;
      std::string          name     ;
      ::vk::Queue          g_queue  ;
      ::vk::Queue          p_queue  ;
      ::vk::Queue          c_queue  ;

      DeviceData() = default ;
      void operator=( const DeviceData data ) ;
      void findQueueFamilies() ;
      void findQueueFamilies( ::vk::SurfaceKHR surface ) ;
      void genVirtualDevice() ;
    };

    void createDevice( ::vk::Instance instance )
    {
      ::vk::PhysicalDevice device ;

      if( physical_devices.size() == 0 )
      {
        physical_devices = instance.enumeratePhysicalDevices() ;
        
        if( physical_devices.size() == 0 ) ; // TODO error, no devices found.
      }
    }

    void DeviceData::operator=( const DeviceData data )
    {
      this->family   = data.family   ;
      this->g_queue  = data.g_queue  ;
      this->c_queue  = data.c_queue  ;
      this->p_queue  = data.p_queue  ;
      this->name     = data.name     ;
      this->p_device = data.p_device ;
      this->v_device = data.v_device ;
    }

    void DeviceData::findQueueFamilies( const ::vk::SurfaceKHR surface ) 
    {
      typedef std::vector<VkQueueFamilyProperties> FamilyPropList ;

      FamilyPropList list    ;
      uint32_t       count   ;
      uint32_t       id      ;
      ::vk::Bool32   support ;

      vkGetPhysicalDeviceQueueFamilyProperties( this->p_device, &count, nullptr ) ;
      list.resize( count ) ;
      vkGetPhysicalDeviceQueueFamilyProperties( this->p_device, &count, list.data() ) ;
      
      id      = 0     ;
      support = false ;
      for (const auto& queueFamily : list ) 
      {
        this->p_device.getSurfaceSupportKHR( id, surface, &support ) ;

        if ( queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT )
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

    void DeviceData::findQueueFamilies()
    {
      typedef std::vector<VkQueueFamilyProperties> FamilyPropList ;

      FamilyPropList list    ;
      uint32_t       count   ;
      uint32_t       id      ;

      vkGetPhysicalDeviceQueueFamilyProperties( this->p_device, &count, nullptr ) ;
      list.resize( count ) ;
      vkGetPhysicalDeviceQueueFamilyProperties( this->p_device, &count, list.data() ) ;
      
      id = 0 ;
      for (const auto& queueFamily : list ) 
      {
        if ( queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT ) 
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

    void DeviceData::genVirtualDevice()
    {
      typedef std::vector<::vk::DeviceQueueCreateInfo> CreateInfos ;

      const float priority[1] = {0.0f} ; //TODO REMOVE and make configurable.
      ::vk::DeviceCreateInfo       dev_info ;
      CreateInfos                  infos    ;
      ::vk::PhysicalDeviceFeatures features ;
      Validation                   layers   ;
      
      infos.resize( this->family.families.size() ) ;

      for( unsigned i = 0; i < this->family.families.size(); i++ )
      {
        infos[ i ].setQueueFamilyIndex( this->family.families[ i ] ) ;
        infos[ i ].setQueueCount      ( 1                          ) ;
        infos[ i ].setPQueuePriorities( priority                   ) ;
      }
      
      features.setSamplerAnisotropy( true ) ;
      
      dev_info.setQueueCreateInfoCount   ( infos.size()               ) ;
      dev_info.setPQueueCreateInfos      ( infos.data()               ) ;
      dev_info.setEnabledExtensionCount  ( required_extensions.size() ) ;
      dev_info.setPpEnabledExtensionNames( required_extensions.data() ) ;
      dev_info.setPEnabledFeatures       ( &features                  ) ;

      if( layers.supported() )
      {
        dev_info.enabledLayerCount   = layers.count() ;
        dev_info.ppEnabledLayerNames = layers.names() ;
      }
      

      this->p_device.createDevice( &dev_info, nullptr, &this->v_device ) ;
      
      this->g_queue = this->v_device.getQueue( this->family.graphics, 0 ) ;
      this->p_queue = this->v_device.getQueue( this->family.present , 0 ) ;
      this->c_queue = this->v_device.getQueue( this->family.compute , 0 ) ;
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

    void Device::initialize( const ::vk::SurfaceKHR surface, unsigned device )
    {
      ::vk::PhysicalDevice       p_device ;
      VkPhysicalDeviceProperties prop     ;
      VkPhysicalDeviceFeatures   feat     ;

      p_device = physical_devices.at( device ) ;

      prop = p_device.getProperties() ;
      feat = p_device.getFeatures()   ;

      data().p_device = p_device ;
      data().name     = prop.deviceName ;

      data().findQueueFamilies( surface ) ;
      data().genVirtualDevice (         ) ;
    }

    const ::vk::Device Device::Device::device() const
    {
      return data().v_device ;
    }

    const ::vk::PhysicalDevice Device::physicalDevice() const
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

    const ::vk::Queue Device::graphics() const
    {
      return data().g_queue ;
    }

    const ::vk::Queue Device::present() const
    {
      return data().p_queue ;
    }

    const ::vk::Queue Device::compute() const
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
  }
}