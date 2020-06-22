#ifndef CASPER_VULKAN_DEVICE_H
#define CASPER_VULKAN_DEVICE_H

typedef struct VkInstance_T*       VkInstance       ;
typedef struct VkPhysicalDevice_T* VkPhysicalDevice ;
typedef struct VkSurfaceKHR_T*     VkSurfaceKHR     ;
typedef struct VkDevice_T*         VkDevice         ;
typedef struct VkQueue_T*          VkQueue          ;

namespace casper
{
  namespace vulkan
  {
    void findDevices( const VkInstance& instance ) ;
    class Device ;
    class Surface ;
    class DeviceManager
    {
      public:
        DeviceManager() ;
        ~DeviceManager() ;
        void create( const Surface& surface, unsigned physical_device ) ;
        const Device& device( const char* surface_name ) ;
    };

    class Device
    {
      public:
        VkDevice device() const ;
        VkPhysicalDevice physicalDevice() const ;
        unsigned graphicsFamily() const ;
        unsigned presentFamily()  const ;
        unsigned computeFamily()  const ;
        VkQueue present() const ;
        VkQueue graphics() const ;
        const char* name() const ;
        Device() ;
        ~Device() ;

      private:
        void genVirtualDevice() ;
        void findQueueFamilies( const Surface& surface ) ;

        struct DeviceData* device_data ;
        DeviceData& data() ;
        const DeviceData& data() const ;

        friend class DeviceManager ;
        friend void findDevices( const VkInstance& instance ) ;
    };
  }
}

#endif