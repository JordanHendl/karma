#include "Synchronization.h"
#include <vk/context/Context.h>
#include <vulkan/vulkan.hpp>
#include <vector>
#include <iostream>
namespace kgl
{
  namespace vk
  {
    struct SynchronizationData
    {
      typedef std::vector<::vk::Semaphore> Semaphores ;
      
      Semaphores   wait_sems   ;
//      Semaphores   signal_sems ;
//      ::vk::Semaphore      wait_sem   ;
      ::vk::Device         device     ;
      ::vk::Semaphore      signal_sem ;
      mutable ::vk::Fence  fence      ;
      bool has_wait = false ;
      
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

    void Synchronization::operator=( const Synchronization& sync )
    {
      
      *this->sync_data = *sync.sync_data ;
    }
    
    Synchronization::Synchronization( const Synchronization& sync )
    {
      this->sync_data = new SynchronizationData() ;
      
      *this->sync_data = *sync.sync_data ;
    }

    void Synchronization::initialize( unsigned gpu )
    {
      const ::kgl::vk::render::Context context ;
      const ::vk::Device               device = context.virtualDevice( gpu ) ;
      ::vk::SemaphoreCreateInfo sem_info   ;
      ::vk::FenceCreateInfo     fence_info ;
      
      fence_info.setFlags( ::vk::FenceCreateFlagBits::eSignaled ) ;
      
      data().device     = device                                        ;
      data().signal_sem = device.createSemaphore( sem_info  , nullptr ) ;
      data().fence      = device.createFence    ( fence_info, nullptr ) ;

      device.resetFences( 1, &data().fence ) ;
    }
    
    void Synchronization::flip()
    {
      const ::vk::Semaphore tmp = data().signal_sem ;

      data().signal_sem     = data().wait_sems[ 0 ] ;
      data().wait_sems[ 0 ] = tmp                   ;
    }

    void Synchronization::initialize( ::vk::Device device )
    {
      ::vk::SemaphoreCreateInfo sem_info   ;
      ::vk::FenceCreateInfo     fence_info ;
      
      fence_info.setFlags( ::vk::FenceCreateFlagBits::eSignaled ) ;

      data().device     = device                                        ;
      data().signal_sem = device.createSemaphore( sem_info  , nullptr ) ;
      data().fence      = device.createFence    ( fence_info, nullptr ) ;
      
      device.resetFences( 1, &data().fence ) ;
    }

    void Synchronization::copy( const Synchronization& sync )
    {
      this->data().wait_sems.clear() ;
      this->data().wait_sems.push_back( sync.data().signal_sem ) ;
    }
    
    void Synchronization::copyWaits( const Synchronization& sync )
    {
      this->data().wait_sems = sync.data().wait_sems ;
      data().has_wait = true ;
    }

    void Synchronization::resetFence() 
    {
      data().device.resetFences( 1, &data().fence ) ;
    }

    unsigned Synchronization::numWaitSems() const
    {
      return data().wait_sems.size() ;
    }

    unsigned Synchronization::numSignalSems() const
    {
      return 1 ;
    }

    const ::vk::Semaphore Synchronization::waitSem( unsigned id ) const
    {
      return id < data().wait_sems.size() ? data().wait_sems.at( id ) : ::vk::Semaphore() ;
    }

    const ::vk::Semaphore Synchronization::signalSem( unsigned id ) const
    {
      return data().signal_sem ;
    }

    const ::vk::Semaphore* Synchronization::waitSems() const
    {
      return data().wait_sems.data() ;
    }

    const ::vk::Semaphore* Synchronization::signalSems() const
    {
      return &data().signal_sem ;
    }
    
    const ::vk::Fence Synchronization::fence() const
    {
      return data().fence ;
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