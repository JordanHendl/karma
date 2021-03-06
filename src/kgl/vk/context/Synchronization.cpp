#include "Synchronization.h"
#include <vk/context/Context.h>
#include <vulkan/vulkan.hpp>
#include <vector>
#include <iostream>
#include <mutex>

namespace kgl
{
  namespace vk
  {
    struct SynchronizationData
    {
      typedef std::vector<::vk::Semaphore> Semaphores ;
      
      Semaphores           wait_sems   ;
      Semaphores           signal_sems ;
      ::vk::Device         device      ;
      mutable ::vk::Fence  fence       ;
      std::mutex           mutex       ;
      
      bool has_wait = false ;
      
      /**
       */
      SynchronizationData() ;
      
      SynchronizationData& operator=( const SynchronizationData& data ) ;
    };

    SynchronizationData::SynchronizationData()
    {
      
    }

    SynchronizationData& SynchronizationData::operator=( const SynchronizationData& data )
    {
      this->mutex.lock() ;
      this->wait_sems   = data.wait_sems   ;
      this->signal_sems = data.signal_sems ;
      this->device      = data.device      ;
      this->fence       = data.fence       ;
      this->has_wait    = data.has_wait    ;
      this->mutex.unlock() ;

      return *this ;
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
    
    void Synchronization::clear()
    {
      data().wait_sems.clear() ;  
    }
    
    void Synchronization::initialize( unsigned gpu, unsigned count )
    {
      const ::kgl::vk::render::Context context ;
      const ::vk::Device               device = context.virtualDevice( gpu ) ;
      ::vk::SemaphoreCreateInfo sem_info   ;
      ::vk::FenceCreateInfo     fence_info ;
      
      fence_info.setFlags( ::vk::FenceCreateFlagBits::eSignaled ) ;
      
      data().signal_sems.resize( count ) ;
      
      for( auto &sem : data().signal_sems ) sem = device.createSemaphore( sem_info  , nullptr ) ;

      data().device     = device                                        ;
      data().fence      = device.createFence    ( fence_info, nullptr ) ;

      device.resetFences( 1, &data().fence ) ;
    }
    
    void Synchronization::initialize( ::vk::Device device, unsigned count )
    {
      ::vk::SemaphoreCreateInfo sem_info   ;
      ::vk::FenceCreateInfo     fence_info ;
      
      fence_info.setFlags( ::vk::FenceCreateFlagBits::eSignaled ) ;
      
      data().signal_sems.resize( count ) ;
      
      for( auto &sem : data().signal_sems ) sem = device.createSemaphore( sem_info  , nullptr ) ;

      data().device     = device                                        ;
      data().fence      = device.createFence    ( fence_info, nullptr ) ;

      device.resetFences( 1, &data().fence ) ;
    }

    void Synchronization::initialize( unsigned gpu )
    {
      const ::kgl::vk::render::Context context ;
      const ::vk::Device               device = context.virtualDevice( gpu ) ;
      const unsigned count = 1 ;

      ::vk::SemaphoreCreateInfo sem_info   ;
      ::vk::FenceCreateInfo     fence_info ;
      
      fence_info.setFlags( ::vk::FenceCreateFlagBits::eSignaled ) ;
      
      data().signal_sems.resize( count ) ;
      
      for( auto &sem : data().signal_sems ) sem = device.createSemaphore( sem_info  , nullptr ) ;

      data().device     = device                                        ;
      data().fence      = device.createFence    ( fence_info, nullptr ) ;

      device.resetFences( 1, &data().fence ) ;
    }
    
    void Synchronization::flip()
    {
      data().signal_sems = data().wait_sems ;
    }

    void Synchronization::copy( const Synchronization& sync )
    {
      this->data().wait_sems.clear() ;
      
      data().mutex.lock() ;
      this->data().wait_sems.push_back( sync.data().signal_sems[ 0 ] ) ;
      data().mutex.unlock() ;
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
      return data().signal_sems.size() ;
    }

    const ::vk::Semaphore Synchronization::waitSem( unsigned id ) const
    {
      return id < data().wait_sems.size() ? data().wait_sems.at( id ) : ::vk::Semaphore() ;
    }

    const ::vk::Semaphore Synchronization::signalSem( unsigned id ) const
    {
      return id < data().signal_sems.size() ? data().signal_sems[ id ] : ::vk::Semaphore() ;
    }

    const ::vk::Semaphore* Synchronization::waitSems() const
    {
      return data().wait_sems.data() ;
    }

    const ::vk::Semaphore* Synchronization::signalSems() const
    {
      return data().signal_sems.data() ;
    }
    
    void Synchronization::addWait( ::vk::Semaphore sem )
    {
      data().wait_sems.push_back( sem ) ;
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