#include "Synchronization.h"
#include <vk/context/Context.h>
#include <vulkan/vulkan.hpp>
#include <vector>

namespace kgl
{
  namespace vk
  {
    struct SynchronizationData
    {
      typedef std::vector<::vk::Semaphore> Semaphores ;
      
//      Semaphores   wait_sems   ;
//      Semaphores   signal_sems ;
      ::vk::Device    device     ;
      ::vk::Semaphore wait_sem   ;
      ::vk::Semaphore signal_sem ;
      ::vk::Fence     fence      ;
      
      /**
       */
      SynchronizationData() ;
    };

    SynchronizationData::SynchronizationData()
    {
      
    }

    Synchronization::Synchronization()
    {
      this->sync_data = new SynchronizationData() ;
    }

    Synchronization::~Synchronization()
    {
      delete this->sync_data ;
    }

    void Synchronization::initialize( unsigned gpu )
    {
      const ::kgl::vk::render::Context context ;
      const ::vk::Device               device = context.virtualDevice( gpu ) ;
      ::vk::SemaphoreCreateInfo sem_info   ;
      ::vk::FenceCreateInfo     fence_info ;
      
      fence_info.setFlags( ::vk::FenceCreateFlagBits::eSignaled ) ;

      data().wait_sem   = device.createSemaphore( sem_info  , nullptr ) ;
      data().signal_sem = device.createSemaphore( sem_info  , nullptr ) ;
      data().fence      = device.createFence    ( fence_info, nullptr ) ;
    }

    unsigned Synchronization::numWaitSems() const
    {
      return 1 ;
    }

    unsigned Synchronization::numSignalSems() const
    {
      return 1 ;
    }

    const ::vk::Semaphore Synchronization::waitSem( unsigned id ) const
    {
      return data().wait_sem ;
    }

    const ::vk::Semaphore Synchronization::signalSem( unsigned id ) const
    {
      return data().signal_sem ;
    }

    const ::vk::Semaphore* Synchronization::waitSems() const
    {
      return &data().wait_sem ;
    }

    const ::vk::Semaphore* Synchronization::signalSems() const
    {
      return &data().signal_sem ;
    }

    void Synchronization::wait() const
    {
      data().device.waitForFences( 1, &data().fence, true, UINT64_MAX ) ;
    }

    SynchronizationData& Synchronization::data()
    {
      return *this->sync_data ;
    }

    const SynchronizationData& Synchronization::data() const
    {
      return *this->sync_data ;
    }
  }
}