#ifndef CASPER_VK_COMMAND_BUFFER
#define CASPER_VK_COMMAND_BUFFER

typedef struct VkBuffer_T *VkBuffer ;
typedef struct VkCommandBuffer_T*  VkCommandBuffer ;
typedef struct VkPresentInfoKHR  VkPresentInfoKHR ; 

namespace casper
{
  namespace vulkan
  {
    class Buffer ;

    class Pipeline  ;
    class Window    ;
    
    class CommandBuffer
    {
      public:
        
        enum SubmitionType
        {
          GRAPHICS,
          COMPUTE,
          PRESENT
        };

        CommandBuffer() ;
        ~CommandBuffer() ;

        const VkCommandBuffer& buffer( unsigned i ) const ;
        const VkCommandBuffer* bufferPtr( unsigned i ) const ;
        void drawIndexed( const Buffer& index, const Buffer& vert ) ;
        void submitCompute( unsigned x, unsigned y, unsigned z = 1 ) ;
        void draw( const Buffer& buffer ) ;
        unsigned count() const ;
        void record() ;
        void stop() ;
        void initCompute( unsigned gpu, unsigned num_buffers ) ;
        void submit( SubmitionType type = SubmitionType::GRAPHICS ) ;
        void initRender( const Window& window ) ;
      private:
        struct CommandBufferData* buffer_data ;
        CommandBufferData& data() ;
        const CommandBufferData& data() const ;
    } ;
  }
}
#endif