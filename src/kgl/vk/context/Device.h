#ifndef CASPER_VULKAN_DEVICE_H
#define CASPER_VULKAN_DEVICE_H

namespace vk
{
  class Device ;
  class Instance ;
  class PhysicalDevice ;
  class SurfaceKHR ;
  class Queue ;
};

namespace kgl
{
  namespace vk
  {
    class Device ;
    class Surface ;

    class Device
    {
      public:
        void initialize( const ::vk::SurfaceKHR surface, unsigned device ) ;
        void initialize( unsigned device ) ;
        const ::vk::Device device() const ;
        const ::vk::PhysicalDevice physicalDevice() const ;
        unsigned graphicsFamily() const ;
        unsigned presentFamily()  const ;
        unsigned computeFamily()  const ;
        const ::vk::Queue present() const ;
        const ::vk::Queue graphics() const ;
        const ::vk::Queue compute() const ;
        const char* name() const ;
        Device() ;
        Device( const Device& device ) ;
        void operator=( const Device& device ) ;
        ~Device() ;

      private:
        struct DeviceData* device_data ;
        DeviceData& data() ;
        const DeviceData& data() const ;
    };
  }
}

#endif