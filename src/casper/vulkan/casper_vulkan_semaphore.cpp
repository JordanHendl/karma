#include "casper_vulkan_semaphore.h"
#include "casper_vulkan_device.h"
#include "casper_vulkan_swapchain.h"
#include <vulkan/vulkan.h>
#include <cerrno>
#include <memory>

namespace casper
{
  namespace vulkan
  {
    struct SemaphoreData
    {
      VkSemaphore semaphore ;
      VkDevice    device    ;
    } ;
  
    Semaphore::Semaphore()
    {
      this->sem_data = new SemaphoreData() ;
    }
  
    Semaphore::~Semaphore()
    {
      delete this->sem_data ;
    }

    Semaphore::Semaphore( const Semaphore& sem )
    {
      this->sem_data = new SemaphoreData() ;
      *this->sem_data = *sem.sem_data ;
    }
  
    void Semaphore::initialize( const Device& device )
    {
      VkSemaphoreCreateInfo info ;
  
      info          = {}                                      ;
      info.sType    = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO ;
      info.flags    = 0                                       ;
      info.pNext    = NULL                                    ;
      data().device = device.device()                         ;
  
      if( vkCreateSemaphore( device.device(), &info, NULL, &data().semaphore ) ) throw std::runtime_error( "Failed to create semaphore.") ;
    }
  
    void Semaphore::lock()
    {
      
    }

    void Semaphore::unlock()
    {
      
    }

    const VkSemaphore* Semaphore::semaphore() const
    {
      return &data().semaphore ;
    }

    unsigned Semaphore::acquire( const SwapChain& chain ) const 
    {
      uint32_t index ;
      vkAcquireNextImageKHR( data().device, *chain.chain(), 10000, data().semaphore, VK_NULL_HANDLE, &index ) ;

      return index ;
    }

    void Semaphore::reset()
    {
      vkDestroySemaphore( data().device, data().semaphore, NULL ) ;
    }
  
    SemaphoreData& Semaphore::data()
    {
      return *this->sem_data ;
    }
  
    const SemaphoreData& Semaphore::data() const
    {
      return *this->sem_data ;
    }
  }
}