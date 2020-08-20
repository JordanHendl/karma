
#ifndef KGL_VK_SYNCHRONIZATION_H
#define KGL_VK_SYNCHRONIZATION_H

namespace vk
{
  class Semaphore ;
  class Fence     ;
  class Device    ;
}

namespace kgl
{
  namespace vk
  {
    /**
     */
    class Synchronization
    {
      public:
        
        /**
         */
        Synchronization() ;
        
        /**
         * @param gpu
         */
        ~Synchronization() ;
        
        /**
         * @param sync
         */
        void operator=( const Synchronization& sync ) ;
        
        /**
         * @param sync
         */
        Synchronization( const Synchronization& sync ) ;

        /**
         * @param gpu
         */
        void initialize( unsigned gpu ) ;

        /**
         * @param gpu
         */
        void initialize( unsigned gpu, unsigned count ) ;
        
        /**
         * @param gpu
         */
        void initialize( ::vk::Device device, unsigned count ) ;
        
        /** Method to clear all wait sems from this object.
         */
        void clear() ;

        /** Method to flip the semaphores of this object.
         */
        void flip() ;

        /**
         * @param sync
         */
        void copy( const Synchronization& sync ) ;
        
        /**
         * @return 
         */
        void copyWaits( const Synchronization& sync ) ;

        /**
         */
        void resetFence() ;

        /**
         * @return 
         */
        unsigned numWaitSems() const ;
        
        /**
         * @return 
         */
        unsigned numSignalSems() const ;
        
        /**
         * @param id
         * @return 
         */
        const ::vk::Semaphore waitSem( unsigned id ) const ;
        
        /**
         * @param id
         * @return 
         */
        const ::vk::Semaphore signalSem( unsigned id ) const ;
        
        /**
         * @return 
         */
        const ::vk::Semaphore* waitSems() const ;
        
        /**
         * @return 
         */
        const ::vk::Semaphore* signalSems() const ;
        
        /**
         * @return 
         */
        void addWait( ::vk::Semaphore sem ) ;

        /**
         */
        void wait() const ;
        
        /**
         * @return 
         */
        const ::vk::Fence fence() const ;
      private:
        
        /**
         */
        struct SynchronizationData *sync_data ;
        
        /**
         * @return 
         */
        SynchronizationData& data() ;
        
        /**
         * @return 
         */
        const SynchronizationData& data() const ;
    };
  }
}
#endif /* SYNCHRONIZATION_H */

