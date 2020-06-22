#ifndef CASPER_VK_COMMAND_BUFFER
#define CASPER_VK_COMMAND_BUFFER

typedef struct VkBuffer_T *VkBuffer ;
typedef struct VkCommandBuffer_T*  VkCommandBuffer ;
typedef struct VkPresentInfoKHR  VkPresentInfoKHR ; 

namespace casper
{
  namespace vulkan
  {
    template<typename T>
    class Buffer ;

    class Device    ;
    class SwapChain ;
    class Pipeline  ;
    class Semaphore ;
    
    class CommandBuffer
    {
      public:
        CommandBuffer() ;
        ~CommandBuffer() ;

        const VkCommandBuffer& buffer( unsigned i ) const ;

        void draw( unsigned count, const VkBuffer& buffer, const Pipeline& pipeline ) ;
        void record() ;
        void stop() ;
        void initialize( const Device& device, const Pipeline& pipeline, const SwapChain& chain ) ;
        const VkPresentInfoKHR& submit( const SwapChain& chain ) ;
      private:
        struct CommandBufferData* buffer_data ;
        CommandBufferData& data() ;
        const CommandBufferData& data() const ;
    } ;
  }
}
#endif