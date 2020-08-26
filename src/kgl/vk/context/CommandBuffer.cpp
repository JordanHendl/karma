#include "CommandBuffer.h"
#include "Semaphore.h"
#include "Context.h"
#include "Command.h"
#include "RenderPass.h"
#include "Synchronization.h"
#include <vulkan/vulkan.hpp>
#include <algorithm>
#include <string>
#include <map>
#include <array>
#include <limits.h>
#include <condition_variable>

namespace kgl
{
  namespace vk
  {
    /** Helper Typedefs
     */
    typedef unsigned                                 GPU          ;
    typedef unsigned                                 QueueFamily  ;
    typedef std::map<QueueFamily, ::vk::CommandPool> QueuePoolMap ;
    typedef std::map<GPU, QueuePoolMap             > PoolMap      ;
    
    static std::mutex mutex ;
    
    std::mutex& queueMutex()
    {
      return mutex ;
    }

    namespace render
    {
      /** Function to create a command pool on the Render Queue if it is missing.
       * @param device The device to generate the pool on.
       * @param family The queue family to associate with this pool.
       * @param flags The flags to use for command buffer creation.
       * @return A command buffer initialized with the input data.
       */
      static ::vk::CommandPool createPool( GPU device, QueueFamily family, unsigned flags ) ;

      /** Structure to contain all of the render command buffer's data.
       */
      struct CmdBuffData
      {
        typedef std::vector<::vk::RenderPassBeginInfo> RenderPassInfos ;
        typedef std::vector<::vk::CommandBuffer      > CommandBuffers  ;
        typedef std::vector<::vk::Fence>               FlightFences    ;
        const unsigned max_frames_in_flight = 5 ;
        mutable ::kgl::vk::Synchronization sync ;
        ::vk::CommandPool pool          ;
        RenderPassInfos   render_infos  ; ///< The render info array to use for drawing.
        CommandBuffers    buffers       ; ///< The buffers that make up this object.
        unsigned          gpu           ; ///< The gpu to use for all operations.
        ::vk::ClearValue  clear_color   ; ///< The color to use for clearing.
        std::string       window_name   ; ///< The name of the window this object is attached to.
        BufferLevel       level         ; ///< The level of buffer.

        /** Default Constructor.
         */
        CmdBuffData() ;
        
        CmdBuffData( const CmdBuffData& data )
        {
          this->buffers      .assign( data.buffers.begin()    , data.buffers.end()      ) ;
          this->render_infos.assign( data.render_infos.begin(), data.render_infos.end() ) ;
          this->sync    = data.sync    ;
          this->gpu     = data.gpu     ;
          this->level   = data.level   ;
        }
        
        CmdBuffData& operator=( const CmdBuffData& data )
          {
            this->buffers      .assign( data.buffers.begin()    , data.buffers.end()      ) ;
            this->render_infos.assign( data.render_infos.begin(), data.render_infos.end() ) ;
            this->sync    = data.sync    ;
            this->gpu     = data.gpu     ;
            this->level   = data.level   ;

            return *this ;
          }
      };
    }
    

    namespace compute
    {
      
      /** Function to create a command pool on the Compute Queue if it is missing.
       * @param device The device to generate the pool on.
       * @param family The queue family to associate with this pool.
       * @param flags The flags to use for command buffer creation.
       * @return A command buffer initialized with the input data.
       */
      static ::vk::CommandPool createPool( GPU device, QueueFamily family, unsigned flags ) ;
      
      /** Structure to contain all of the compute command buffer's data.
       */
      struct CmdBuffData
      {
        typedef std::vector<::vk::CommandBuffer> CommandBuffers ;
        
        mutable ::kgl::vk::Synchronization sync ;
        ::vk::Fence       fence       ;
        ::vk::CommandPool pool        ;
        CommandBuffers    buffers     ; ///< The buffers that make up this object.
        unsigned          gpu         ; ///< The gpu to use for all operations.
        BufferLevel       level       ; ///< The level of buffer.

        /** Default constructor.
         */
        CmdBuffData() ;
        ~CmdBuffData()
        {
          this->buffers.clear() ;
        }
        
        CmdBuffData( const CmdBuffData& data )
        {
          this->buffers.assign( data.buffers.begin(), data.buffers.end() ) ;
          this->sync    = data.sync    ;
          this->gpu     = data.gpu     ;
          this->level   = data.level   ;
        }

        CmdBuffData& operator=( const CmdBuffData& data )
        {
          this->buffers.assign( data.buffers.begin(), data.buffers.end() ) ;
          this->sync    = data.sync    ;
          this->gpu     = data.gpu     ;
          this->level   = data.level   ;
          
          return *this ;
        }
      };
    }

    namespace render
    {
      
      ::vk::CommandPool createPool( GPU device, QueueFamily family, unsigned flags )
      {
        ::vk::CommandPoolCreateInfo info     ;
        ::vk::CommandPool           cmd_pool ;
        ::kgl::vk::render::Context  context  ;
        
        info.setQueueFamilyIndex( family                                             ) ;
        info.setFlags           ( static_cast<::vk::CommandPoolCreateFlags>( flags ) ) ;
        
        context.virtualDevice( device ).createCommandPool( &info, nullptr, &cmd_pool ) ;
        
        return cmd_pool ;
      }
      
      CmdBuffData::CmdBuffData()
      {
        this->gpu = UINT_MAX ;
      }

      CommandBuffer::CommandBuffer()
      {
        this->cmd_data = new CmdBuffData() ;
      }
      
      CommandBuffer::CommandBuffer( const CommandBuffer& buff )
      {
        this->cmd_data = new CmdBuffData() ;
        
        *this->cmd_data = *buff.cmd_data ;
      }
      
      CommandBuffer& CommandBuffer::operator=(const CommandBuffer& buff)
      {
        *this->cmd_data = *buff.cmd_data ;
        return *this ;
      }

      CommandBuffer::~CommandBuffer()
      {
        delete this->cmd_data ;
      }
      
      void CommandBuffer::initialize( const char* window_name, unsigned gpu, unsigned count, BufferLevel level )
      {
        const auto buff_level = level == BufferLevel::Primary ? ::vk::CommandBufferLevel::ePrimary : ::vk::CommandBufferLevel::eSecondary ;
        const auto pool_flags = ::vk::CommandPoolCreateFlagBits::eResetCommandBuffer                                                      ;
        ::kgl::vk::render::Context      context          ;
        ::vk::CommandBufferAllocateInfo info             ;
        ::vk::FenceCreateInfo           fence_info       ;
        ::vk::CommandPool               pool             ;
        ::vk::Device                    device           ;
        unsigned                        qFamily          ;
        
        data().gpu         = gpu                                    ;
        data().window_name = window_name                            ;
        data().level       = level                                  ;
        qFamily            = context.graphicsFamily( gpu )          ;
        device             = context.virtualDevice( gpu )           ;
        
        data().sync.initialize( gpu ) ;
        data().buffers.resize( count ) ;
        data().pool = ::kgl::vk::render::createPool( gpu, qFamily, static_cast<unsigned>( pool_flags ) ) ;
        
        pool = data().pool ;

        info.setCommandPool       ( pool       ) ;
        info.setLevel             ( buff_level ) ;
        info.setCommandBufferCount( count      ) ;
        
        device.allocateCommandBuffers( &info, data().buffers.data() ) ;
      }

      void CommandBuffer::record() const
      {
        ::vk::CommandBufferBeginInfo info ;
        
        for( unsigned index = 0; index < data().buffers.size(); index++ )
        {
          data().buffers[ index ].begin( &info ) ;
        }
      }

      void CommandBuffer::record(::kgl::vk::RenderPass& pass ) const
      {
        const std::array<float, 4> clear_colors = { 0.0f, 0.0f, 0.0f, 0.0f } ;
        const auto                render_flags = ::vk::SubpassContents::eInline ;

        ::vk::ClearValue             clear_val        ;
        ::vk::RenderPassBeginInfo    render_pass_info ;
        ::vk::CommandBufferBeginInfo info             ;
        
        clear_val.setColor( ::vk::ClearColorValue( clear_colors ) ) ;

        render_pass_info.setRenderPass       ( pass.pass()    ) ;
        render_pass_info.setRenderArea       ( pass.area()    ) ;
        render_pass_info.setClearValueCount  ( 1              ) ;
        render_pass_info.setPClearValues     ( &clear_val     ) ;
        for( unsigned index = 0; index < data().buffers.size(); index++ )
        {
          render_pass_info.setFramebuffer( pass.buffer( index ) ) ;
          data().buffers[ index ].begin( &info ) ;
          data().buffers[ index ].beginRenderPass( &render_pass_info, render_flags ) ;
        }
      }

      ::vk::CommandBuffer CommandBuffer::buffer( unsigned id ) const
      {
        return id < data().buffers.size() ? data().buffers[ id ] : data().buffers[ 0 ] ;
      }

      unsigned CommandBuffer::count() const
      {
        return data().buffers.size() ;
      }

      void CommandBuffer::pushConstantBase( void* val, ::vk::PipelineLayout layout, unsigned stage_flags, unsigned count, unsigned element_size )
      {
        for( auto buff : data().buffers )
        {
          buff.pushConstants( layout, static_cast<::vk::ShaderStageFlags>( stage_flags ), 0, count * element_size, val ) ;
        }
      }

      void CommandBuffer::draw( const ::vk::Buffer vertices, unsigned element_size ) // TODO::: write instanced drawing.
      {
        ::vk::DeviceSize offset ;
        
        offset = 0 ;
        for( auto buff : data().buffers )
        {
          buff.bindVertexBuffers( 0, 1, &vertices, &offset ) ;
          buff.draw( element_size, 1, 0, 0 ) ;
        }
      }
      
      void CommandBuffer::drawInstanced( const ::vk::Buffer vertices, unsigned element_size, unsigned instance_count ) // TODO::: write instanced drawing.
      {
        ::vk::DeviceSize offset ;
        
        offset = 0 ;
        for( auto buff : data().buffers )
        {
          buff.bindVertexBuffers( 0, 1, &vertices, &offset ) ;
          buff.draw( element_size, instance_count, 0, 0 ) ;
        }
      }

      void CommandBuffer::drawIndexed( unsigned num_indices, const ::vk::Buffer indices, const ::vk::Buffer vertices ) // TODO:: write instanced drawing.
      {
        const auto index_type = ::vk::IndexType::eUint32 ;
        ::vk::DeviceSize offset ;
        
        offset = 0 ;
        for( auto buff : data().buffers )
        {
          buff.bindVertexBuffers( 0      , 1, &vertices, &offset                   ) ;
          buff.bindIndexBuffer  ( indices, 0, index_type                           ) ;
          buff.drawIndexed      ( static_cast<uint32_t>( num_indices ), 1, 0, 0, 0 ) ;
        }
      }
      
      void CommandBuffer::push( const ::kgl::vk::gpu::Command& command )
      {
        for( auto buff : data().buffers )
        {
          command.push( buff ) ;
        }
      }
      
      void CommandBuffer::stop( bool pass ) const
      {
        for( auto buff : data().buffers )
        {
          if( pass ) buff.endRenderPass() ;
          buff.end()           ;
        }
      }
      
      void CommandBuffer::reset()
      {
        const ::kgl::vk::render::Context context ;
        const ::vk::Device device = context.virtualDevice( data().gpu ) ;
        
        device.freeCommandBuffers( data().pool, data().buffers.size(), data().buffers.data() ) ;
        
        device.destroy( data().pool ) ;
        data().buffers.clear() ;
      }
      
      void CommandBuffer::submitSync()
      {
        const ::kgl::vk::render::Context context ;
        const ::vk::Queue queue = context.graphicsQueue( data().gpu ) ;
        
        ::vk::SubmitInfo info ;
        
        info.setCommandBufferCount( data().buffers.size() ) ;
        info.setPCommandBuffers   ( data().buffers.data() ) ;
        
        mutex.lock() ;
        queue.submit( 1, &info, ::vk::Fence() ) ;
        queue.waitIdle() ;
        mutex.unlock() ;
      }

      void CommandBuffer::waitOn( ::vk::Semaphore sem )
      {
//        data().wait_sems.push_back( sem ) ;
      }

      void CommandBuffer::onFinish( ::vk::Semaphore sem )
      {
//        data().signal_sems.push_back( sem ) ;
      }

      void CommandBuffer::attach( ::vk::Fence fence )
      {
//        data().fence = fence ;
      }

      void CommandBuffer::submit( const ::vk::SubmitInfo& info, const Synchronization& sync ) const 
      {
        const ::kgl::vk::render::Context context                        ;
        const ::vk::Device device = context.virtualDevice( data().gpu ) ;
        const ::vk::Queue queue   = context.graphicsQueue( data().gpu ) ;
        ::vk::Fence fence = sync.fence() ;
        
        mutex.lock() ;
        device.resetFences( 1, &fence ) ;
        queue.submit( 1, &info, fence ) ;
        mutex.unlock() ;
      }
      
      void CommandBuffer::submit() const
      {
        const ::kgl::vk::render::Context context                        ;
        const ::vk::Queue queue   = context.graphicsQueue( data().gpu ) ;

        ::vk::SubmitInfo info  ;
        
        data().sync.resetFence() ;
        if( data().level == BufferLevel::Primary )
        { 
//          info.setWaitSemaphoreCount  ( data().wait_sems.size()   ) ;
//          info.setPWaitSemaphores     ( data().wait_sems.data()   ) ;
          info.setCommandBufferCount  ( data().buffers.size()     ) ;
          info.setPCommandBuffers     ( data().buffers.data()     ) ;
//          info.setSignalSemaphoreCount( data().signal_sems.size() ) ;
//          info.setPSignalSemaphores   ( data().signal_sems.data() ) ;
           
          mutex.lock() ;
          queue.submit( 1, &info, data().sync.fence() ) ;
          mutex.unlock() ;
        }
      }

      void CommandBuffer::submit( int buffer )
      {
//        const ::kgl::vk::render::Context context ;
////        const              ::vk::Queue   queue  = context.graphicsQueue( data().gpu )                 ;
//
//        ::vk::SubmitInfo info  ;
//        
//        if( data().level == BufferLevel::Primary )
//        { 
////          info.setWaitSemaphoreCount  ( data().wait_sems.size()                                          ) ;
////          info.setPWaitSemaphores     ( data().wait_sems.data()                                          ) ;
////          info.setCommandBufferCount  ( buffer == -1 ? data().buffers.size() : 1                         ) ;
////          info.setPCommandBuffers     ( buffer == -1 ? data().buffers.data() : &data().buffers[ buffer ] ) ;
////          info.setSignalSemaphoreCount( data().signal_sems.size()                                        ) ;
////          info.setPSignalSemaphores   ( data().signal_sems.data()                                        ) ;
////           
////          queue.submit( 1, &info, data().fence ) ;
//        }
      }
      
      void CommandBuffer::clearSynchronization()
      {
//        data().finish_sem .clear()   ;
//        data().signal_sems.clear()   ;
//        data().wait_sems  .clear()   ;
//        data().fence = ::vk::Fence() ;
      }

      void CommandBuffer::submit( const CommandBuffer& buffer ) const
      {
        const unsigned buff_size  = data().buffers.size()           ;
        const unsigned input_size = buffer.count()                  ;
        const unsigned num_buff = std::min( buff_size, input_size ) ;
        ::vk::CommandBuffer temp_buff ;

        for( unsigned index = 0; index < num_buff; index++ )
        {
          temp_buff = buffer.buffer( index ) ;
          data().buffers[ index ].executeCommands( 1, &temp_buff ) ;
        }
      }

      CmdBuffData& CommandBuffer::data()
      {
        return *this->cmd_data ;
      }

      const CmdBuffData& CommandBuffer::data() const
      {
        return *this->cmd_data ;
      }
    }
    
    namespace compute
    {
      ::vk::CommandPool createPool( GPU device, QueueFamily family, unsigned flags )
      {
        ::vk::CommandPoolCreateInfo info     ;
        ::vk::CommandPool           cmd_pool ;
        ::kgl::vk::compute::Context context  ;
        
        info.setQueueFamilyIndex( family                                             ) ;
        info.setFlags           ( static_cast<::vk::CommandPoolCreateFlags>( flags ) ) ;
        
        context.virtualDevice( device ).createCommandPool( &info, nullptr, &cmd_pool ) ;
        
        return cmd_pool ;
      }

      CmdBuffData::CmdBuffData()
      {
        this->gpu = UINT_MAX ;
      }

      CommandBuffer::CommandBuffer()
      {
        this->cmd_data = new CmdBuffData() ;
      }

      CommandBuffer::CommandBuffer( const CommandBuffer& buff )
      {
        this->cmd_data = new CmdBuffData() ;
        
        *this->cmd_data = *buff.cmd_data ;
      }
      
      CommandBuffer& CommandBuffer::operator=(const CommandBuffer& buff)
      {
        *this->cmd_data = *buff.cmd_data ;
        return *this ;
      }

      CommandBuffer::~CommandBuffer()
      {
        delete this->cmd_data ;
      }
      
      void CommandBuffer::reset()
      {
        const ::kgl::vk::render::Context context ;
        const ::vk::Device device = context.virtualDevice( data().gpu ) ;
        
        device.freeCommandBuffers( data().pool, data().buffers.size(), data().buffers.data() ) ;
        
        device.destroy( data().pool ) ;
        data().buffers.clear() ;
      }
      
      void CommandBuffer::initialize( unsigned gpu, unsigned count, BufferLevel level )
      {
        const auto buff_level = level == BufferLevel::Primary ? ::vk::CommandBufferLevel::ePrimary : ::vk::CommandBufferLevel::eSecondary ;
        const auto pool_flags = ::vk::CommandPoolCreateFlagBits::eResetCommandBuffer                                                      ;
        ::kgl::vk::compute::Context     context          ;
        ::vk::CommandBufferAllocateInfo info             ;
        ::vk::CommandPool               pool             ;
        ::vk::Device                    device           ;
        unsigned                        qFamily          ;

        data().gpu   = gpu                          ;
        data().level = level                        ;
        qFamily      = context.computeFamily( gpu ) ;
        device       = context.virtualDevice( gpu ) ;
        
        data().sync.initialize( gpu ) ;
        data().buffers.resize( count ) ;
        
        data().pool = ::kgl::vk::compute::createPool( gpu, qFamily, static_cast<unsigned>( pool_flags ) ) ;
        
        pool = data().pool ;

        info.setCommandPool       ( pool       ) ;
        info.setLevel             ( buff_level ) ;
        info.setCommandBufferCount( count      ) ;
        
        device.allocateCommandBuffers( &info, data().buffers.data() ) ;
      }
      
      void CommandBuffer::clearSynchronization()
      {
//        data().finish_sem .clear()   ;
//        data().signal_sems.clear()   ;
//        data().wait_sems  .clear()   ;
        data().fence = ::vk::Fence() ;
      }

      void CommandBuffer::record() const
      {
        ::vk::CommandBufferBeginInfo info ;

        for( unsigned index = 0; index < data().buffers.size(); index++ )
        {
          data().buffers[ index ].begin( &info ) ;
        }
      }

      ::vk::CommandBuffer CommandBuffer::buffer( unsigned id ) const
      {
        return id < data().buffers.size() ? data().buffers[ id ] : data().buffers[ 0 ] ;
      }
      
      void CommandBuffer::pushConstantBase( void* val, const ::vk::PipelineLayout& layout, unsigned stage_flags, unsigned count, unsigned element_size )
      {
        for( auto &buff : data().buffers )
        {
          buff.pushConstants( layout, static_cast<::vk::ShaderStageFlags>( stage_flags ), 0, count * element_size, val ) ;
        }
      }

      unsigned CommandBuffer::count() const
      {
        return data().buffers.size() ;
      }
      
      void CommandBuffer::swap( Synchronization& sync )
      {
        data().sync.copy( sync ) ;
      }

      void CommandBuffer::stop() const
      {
        for( auto buff : data().buffers )
        {
          buff.end() ;
        }
      }

      void CommandBuffer::submit() const
      {
        const ::kgl::vk::compute::Context context                       ;
        const ::vk::Queue queue   = context.computeQueue ( data().gpu ) ;

        ::vk::SubmitInfo info  ;
        
        data().sync.resetFence() ;
        if( data().level == BufferLevel::Primary )
        { 
          info.setWaitSemaphoreCount  ( 0 ) ; //data().sync.numWaitSems()   ) ;
          info.setPWaitSemaphores     ( data().sync.waitSems()      ) ;
          info.setCommandBufferCount  ( data().buffers.size()       ) ;
          info.setPCommandBuffers     ( data().buffers.data()       ) ;
          info.setSignalSemaphoreCount( 0                           ) ;
          info.setPSignalSemaphores   ( data().sync.signalSems()    ) ;
          
          mutex.lock() ;
          queue.submit( 1, &info, data().sync.fence() ) ;
          queue.waitIdle() ;
          mutex.unlock() ;
          
        }
      }
      
      void CommandBuffer::wait() const
      {
        const ::kgl::vk::compute::Context context                        ;
        const ::vk::Device device = context.virtualDevice( data().gpu )  ;
        
        auto fence = data().sync.fence() ;
        device.waitForFences( 1, &fence, true, UINT64_MAX ) ;
      }

      void CommandBuffer::submit( const CommandBuffer& buffer ) const
      {
        const unsigned buff_size  = data().buffers.size()           ;
        const unsigned input_size = buffer.count()                  ;
        const unsigned num_buff = std::min( buff_size, input_size ) ;
        ::vk::CommandBuffer temp_buff ;

        for( unsigned index = 0; index < num_buff; index++ )
        {
          temp_buff = buffer.buffer( index ) ;
          data().buffers[ index ].executeCommands( 1, &temp_buff ) ;
        }
      }

      void CommandBuffer::waitOn( ::vk::Semaphore sem )
      {
//        data().wait_sems.push_back( sem ) ;
      }

      void CommandBuffer::onFinish( ::vk::Semaphore sem )
      {
//        data().signal_sems.push_back( sem ) ;
      }

      void CommandBuffer::attach( ::vk::Fence fence )
      {
        data().fence = fence ;
      }

      CmdBuffData& CommandBuffer::data()
      {
        return *this->cmd_data ;
      }

      const CmdBuffData& CommandBuffer::data() const
      {
        return *this->cmd_data ;
      }
    } 
  }
}