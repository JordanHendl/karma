
#ifndef KGL_VK_SYNCHRONIZATION_H
#define KGL_VK_SYNCHRONIZATION_H

namespace vk
{
  class Semaphore ;
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
         * @param gpu
         */
        void initialize( unsigned gpu ) ;
        
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
         */
        void wait() const ;
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

