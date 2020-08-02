#ifndef KGL_VK_COMMANDBUFFER_H
#define KGL_VK_COMMANDBUFFER_H

namespace vk
{
  class CommandBuffer ;
  class Buffer        ;
  class Semaphore     ;
  class Fence         ;
}

namespace kgl
{
  namespace vk
  {
    class Window     ;
    class RenderPass ;
    namespace gpu
    {
      class Command ;
    }

    enum BufferLevel
    {
      Primary,
      Secondary
    };
    
    namespace render
    {
      class CommandBuffer
      {
        public:
          
          /**
           */
          CommandBuffer() ;
          
          /**
           */
          ~CommandBuffer() ;
          
          /**
           * @param window_name
           * @param gpu
           */
          void initialize( const char* window_name, unsigned gpu, unsigned count, BufferLevel level = BufferLevel::Primary ) ;
          
          /** Method to return the name of the window this command buffer is associated with.
           * @return The name of the window this command buffer is associated with.
           */
          const char* window() const ;
          
          /**
           * @param id
           * @return 
           */
          ::vk::CommandBuffer buffer( unsigned id ) const ;
          
          /**
           * @return 
           */
          unsigned count() const ;
          
          /**
           */
          void clearSynchronization() ;
          
          /**
           */
          void record( ::kgl::vk::RenderPass& pass ) const ;
          
          /**
           * @param command
           */
          void push( const ::kgl::vk::gpu::Command& command ) ;
          
          /**
           * @param vertices
           */
          void draw( const ::vk::Buffer vertices, unsigned element_size ) ;
          
          /** Method to draw an object with vertices & indices.
           * @param num_indices Number of indices provided.
           * @param indices The vulkan buffer associated with the indices.
           * @param vertices The vulkan buffer associated with the vertices.
           */
          void drawIndexed( unsigned num_indices, const ::vk::Buffer indices, const ::vk::Buffer vertices ) ;
          
          /** Method to stop recording of this command buffer.
           */
          void stop() const ;
          
          /** Submits this command buffer to the Window to be pushed to the graphics queue.
           * @note This is to be done once per frame. Synchronizes with the swapchain's framebuffers.
           */
          void submit( int buffer = -1 ) ;

          /** Submits this command buffer to the Window to be pushed to the graphics queue, and then immidiately syncs the device.
           * @note This does no smart synchronization. Pushes buffers to queue and then immidiately syncs the device.
           */
          void submitSync() ;
          
          /**
           * @param sem
           */
          void waitOn( ::vk::Semaphore sem ) ;
          
          /**
           * @param sem
           */
          void onFinish( ::vk::Semaphore sem ) ;
          
          /**
           * @param fence
           */
          void attach( ::vk::Fence fence ) ;

          /** Submits the input secondary command buffer into this one.
           * @param buffer The buffer to submit as a part of this one.
           */
          void submit( const CommandBuffer& buffer ) const ;

        private:
          
          /**
           */
          struct CmdBuffData *cmd_data ;
          
          /**
           * @return 
           */
          CmdBuffData& data() ;
          
          /**
           * @return 
           */
          const CmdBuffData& data() const ;
          
          friend class Window ;
      };
    }
    
    namespace compute
    {
      /**
       */
      class CommandBuffer
      {
        public:

          /**
           */
          CommandBuffer() ;
        
          /**
           * @param gpu
           * @param count
           */
          ~CommandBuffer() ;
        
          /**
           * @param command
           */
          void push( const ::kgl::vk::gpu::Command& command ) ;

          /**
           * @param gpu
           * @param count
           */
          void initialize( unsigned gpu, unsigned count, BufferLevel level = BufferLevel::Primary ) ;
        
          /**
           */
          void record() const ;

          /**
           */
          void clearSynchronization() ;

          /**
           * @param id
           * @return 
           */
          ::vk::CommandBuffer buffer( unsigned id ) const ;
          
          /**
           * @return 
           */
          unsigned count() const ;

          /**
           * @param sem
           */
          void waitOn( ::vk::Semaphore sem ) ;
          
          /**
           * @param sem
           */
          void onFinish( ::vk::Semaphore sem ) ;
          
          /**
           * @param fence
           */
          void attach( ::vk::Fence fence ) ;

          /**
           */
          void stop() const ;
        
          /**
           */
          void submit() const ;
          
          /**
           * @param buffer
           */
          void submit( const CommandBuffer& buffer ) const ;
          
        private:
          
          /**
           */
          struct CmdBuffData *cmd_data ;
          
          /**
           * @return 
           */
          CmdBuffData& data() ;
          
          /**
           * @return 
           */
          const CmdBuffData& data() const ;
      };
    } 
  }
}
#endif /* COMMANDBUFFER_H */

