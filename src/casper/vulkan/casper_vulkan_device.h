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
    static void findDevices( const VkInstance& instance ) ;
    class Device ;
    class Surface ;

    class Device
    {
      public:
        void initialize( const casper::vulkan::Surface& surface, const VkInstance& instance, unsigned device ) ;
        void initialize( unsigned device ) ;
        VkDevice device() const ;
        VkPhysicalDevice physicalDevice() const ;
        unsigned graphicsFamily() const ;
        unsigned presentFamily()  const ;
        unsigned computeFamily()  const ;
        VkQueue present() const ;
        VkQueue graphics() const ;
        VkQueue compute() const ;
        const char* name() const ;
        Device() ;
        Device( const Device& device ) ;
        void operator=( const Device& device ) ;
        ~Device() ;

      private:
        struct DeviceData* device_data ;
        DeviceData& data() ;
        const DeviceData& data() const ;

        friend void findDevices( const VkInstance& instance ) ;
    };
  }
}

#endif