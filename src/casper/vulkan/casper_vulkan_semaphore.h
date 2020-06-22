#ifndef CASPER_SEMAPHORE_H
#define CASPER_SEMAPHORE_H

typedef struct VkSemaphore_T* VkSemaphore ;

namespace casper
{
  namespace vulkan
  {
    class Device ;
    class SwapChain ;

    class Semaphore
    {
      public:
        Semaphore() ;
        Semaphore( const Semaphore& sem ) ;
        ~Semaphore() ;
        const VkSemaphore* semaphore() const ;
        void initialize( const Device& device ) ;
        unsigned acquire( const SwapChain& chain ) const ;
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