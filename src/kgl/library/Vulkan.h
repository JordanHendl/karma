#ifndef KGL_VULKAN_H
#define KGL_VULKAN_H

  #ifdef KGL_VULKAN_FOUND

  namespace vk
  {
    class Buffer         ;
    class DeviceMemory   ;
    class Device         ;
    class PhysicalDevice ;
  }
  
  namespace kgl
  {
    template<typename IMPL>
    class Buffer ;
  
    namespace vk
    {
      class Vulkan
      {
        public:
          using Buffer = ::vk::Buffer       ;
          using Memory = ::vk::DeviceMemory ;
          using Device = ::vk::Device       ;
  
          static void initialize() ;
  
          static unsigned deviceCount() ;
  
          static bool deviceValid( unsigned gpu ) ;
  
          static unsigned convertError( unsigned error ) ;

        private:
          
          class MemoryFlags
          {
            public:
              MemoryFlags() ;
              unsigned val() ;
              MemoryFlags& operator=( unsigned flags ) ;
            private:
              unsigned flag ;
          };

          class BufferFlags
          {
            public:
              BufferFlags() ;
              unsigned val() ;
              BufferFlags& operator=( unsigned flags ) ;
            private:
              unsigned flag ;
          };

          typedef void* Data ;
          friend class ::kgl::Buffer<Vulkan> ;

          Vulkan() ;

          ~Vulkan() ;

          void copyTo( Buffer* src, Buffer* dst, unsigned gpu, unsigned amt ) ;

          void copyTo( Data src, Memory* dst, unsigned gpu, unsigned amt ) ;

          void copyTo( Memory* src, Data dst, unsigned gpu, unsigned amt ) ;

          void free( Buffer* buff, Memory* mem, unsigned gpu ) ;

          Buffer* createBuffer( unsigned size, unsigned gpu, BufferFlags buff_flags ) ;

          Buffer* createBuffer( unsigned size, unsigned gpu ) ;

          Memory* createMemory( Buffer* buffer, unsigned gpu, MemoryFlags mem_flags ) ;
          
          Memory* createMemory( Buffer* buffer, unsigned gpu ) ;
          
          struct VulkanData* vulkan_data ;
          
          VulkanData& data() ;

          const VulkanData& data() const ;
      };
    }
  }
  
  #endif

#endif

