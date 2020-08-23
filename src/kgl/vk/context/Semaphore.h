#ifndef CASPER_SEMAPHORE_H
#define CASPER_SEMAPHORE_H

typedef struct VkSemaphore_T* VkSemaphore ;

namespace vk
{
  class Semaphore ;
  class SwapchainKHR ;
  class Device ;
}

namespace kgl
{
  namespace vk
  {
    class Device ;
    class SwapChain ;

    class Semaphore
    {
      public:
        Semaphore() ;
        Semaphore( const Semaphore& sem ) ;
        ~Semaphore() ;
        const ::vk::Semaphore semaphore() const ;
        void initialize( const ::vk::Device device ) ;
        unsigned acquire( const ::vk::SwapchainKHR chain ) const ;
        void lock() ;
        void unlock() ;
        void reset() ;

      private:
      struct SemaphoreData* sem_data ;
      SemaphoreData& data() ;
      const SemaphoreData& data() const ;
    };
  }
}
#endif