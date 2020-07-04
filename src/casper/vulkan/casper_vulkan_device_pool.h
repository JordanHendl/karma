#ifndef CASPER_VULKAN_DEVICE_POOL_H
#define CASPER_VULKAN_DEVICE_POOL_H

namespace casper
{
  namespace vulkan
  {
    class Device ;
    class Window ;
    
    /**
     */
    class DevicePool
    {
      public:
        
        /**
         */
        DevicePool() = default ;
        
        /**
         */
        ~DevicePool() = default ;
        
        /**
         * @param gpu
         * @return 
         */
        const Device& device( unsigned gpu ) const ;

      private:
        
        /**
         * @param gpu
         * @param device
         */
        void setDevice( unsigned gpu, const Device& device ) const ;
        
        /**
         */
        friend class Window ;
    };
  }
}

#endif /* CASPER_VULKAN_DEVICE_POOL_H */

