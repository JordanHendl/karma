#include "Semaphore.h"
#include "Device.h"
#include "Swapchain.h"
#include <vulkan/vulkan.hpp>
#include <cerrno>
#include <memory>

namespace kgl
{
  namespace vk
  {
    struct SemaphoreData
    {
      ::vk::Semaphore semaphore ;
      ::vk::Device    device    ;
    };
  
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
  
    void Semaphore::initialize( const ::vk::Device device )
    {
      ::vk::SemaphoreCreateInfo info ;

      data().device    = device                                  ;
      data().semaphore = device.createSemaphore( info, nullptr ) ;
    }
  
    void Semaphore::lock()
    {
      
    }

    void Semaphore::unlock()
    {
      
    }

    const ::vk::Semaphore Semaphore::semaphore() const
    {
      return data().semaphore ;
    }

    unsigned Semaphore::acquire( const ::vk::SwapchainKHR chain ) const 
    {
      ::vk::AcquireNextImageInfoKHR info ;
      ::vk::Fence fence ;
      info.setSemaphore( data().semaphore ) ;
      info.setSwapchain( chain            ) ;
      info.setTimeout  ( UINT64_MAX       ) ;
      
      auto result = data().device.acquireNextImageKHR( chain, UINT64_MAX, data().semaphore, fence ) ;
      
      return result.value ;
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