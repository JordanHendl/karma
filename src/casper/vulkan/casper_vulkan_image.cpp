#include "casper_vulkan_image.h"
#include "casper_vulkan_error_handler.h"
#include "casper_vulkan_device_pool.h"
#include "casper_vulkan_device.h"
#include "casper_vulkan_command_buffer.h"
#include "stb_image.h"
#include <vulkan/vulkan.h>
#include <cstring>

namespace casper
{
  namespace vulkan
  {
    /**
     * @param filter
     * @param props
     * @param gpu
     */
    static inline unsigned memoryType( unsigned filter, VkMemoryPropertyFlags props, unsigned gpu ) ;

    struct ImageData
    {
      VkImage        img        ; ///< TODO
      VkImageView    image_view ; ///< TODO
      VkSampler      sampler    ; ///< TODO
      VkDeviceMemory memory     ; ///< TODO
      CommandBuffer  cmd_buffer ; ///< TODO
      unsigned       gpu        ; ///< TODO
      unsigned       width      ; ///< TODO
      unsigned       height     ; ///< TODO
      
      /**
       */
      ImageData() ;
      
      /**
       */
      void generateVulkanImage() ;
      
      /**
       */
      void allocateVulkanImage() ;
      
      /**
       * @param size
       * @param usage
       * @param properties
       * @param buffer
       */
      void createBuffer( VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& mem ) ;
      
      /**
       * @param format
       * @param old_layout
       * @param new_layout
       */
      void transitionLayout( VkFormat format, VkImageLayout old_layout, VkImageLayout new_layout ) ;
      
      /**
       * @param buffer
       */
      void createSampler() ;

      /**
       * @param buffer
       */
      void createImageView() ;

      /**
       * 
       * @param buffer
       */
      void copyBufferToImage( VkBuffer buffer ) ;
    };
    
    unsigned memoryType( unsigned filter, VkMemoryPropertyFlags props, unsigned gpu )
    {
      DevicePool                       pool     ;
      VkPhysicalDeviceMemoryProperties mem_prop ;
      
      vkGetPhysicalDeviceMemoryProperties( pool.device( gpu ).physicalDevice(), &mem_prop ) ;
      
      for( unsigned index = 0; index < mem_prop.memoryTypeCount; index++ )
      {
        if( ( filter & ( 1 << index ) ) && ( mem_prop.memoryTypes[ index ].propertyFlags & props ) == props )
        {
          return index ;
        }
      }
      
      return 0 ;
    }

    ImageData::ImageData()
    {
      this->gpu    = 0    ;
      this->width  = 1280 ;
      this->height = 1024 ;
    }
    
    void ImageData::createImageView()
    {
      VkImageViewCreateInfo info ;
      DevicePool            pool ;
      
      info = {} ;
      info.sType                           = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO ;
      info.image                           = this->img                                ;
      info.viewType                        = VK_IMAGE_VIEW_TYPE_2D                    ;
      info.format                          = VK_FORMAT_R8G8B8A8_SRGB                  ; //TODO make this configurable.
      info.subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT                ;
      info.subresourceRange.baseMipLevel   = 0                                        ;
      info.subresourceRange.levelCount     = 1                                        ;
      info.subresourceRange.baseArrayLayer = 0                                        ;
      info.subresourceRange.layerCount     = 1                                        ;
      info.pNext                           = nullptr                                  ;

      HANDLE_ERROR( vkCreateImageView( pool.device( this->gpu ).device(), &info, nullptr, &this->image_view) ) ;
    }
    
    void ImageData::createSampler()
    {
      VkSamplerCreateInfo info ;
      DevicePool          pool ;

      info.sType                   = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO ;
      info.magFilter               = VK_FILTER_LINEAR                      ;
      info.minFilter               = VK_FILTER_LINEAR                      ;
      info.addressModeU            = VK_SAMPLER_ADDRESS_MODE_REPEAT        ;
      info.addressModeV            = VK_SAMPLER_ADDRESS_MODE_REPEAT        ;
      info.addressModeW            = VK_SAMPLER_ADDRESS_MODE_REPEAT        ;
      info.anisotropyEnable        = VK_TRUE                               ;
      info.maxAnisotropy           = 16.0f                                 ;
      info.borderColor             = VK_BORDER_COLOR_INT_OPAQUE_BLACK      ;
      info.unnormalizedCoordinates = VK_FALSE                              ; 
      info.compareEnable           = VK_FALSE                              ;
      info.compareOp               = VK_COMPARE_OP_ALWAYS                  ;
      info.mipmapMode              = VK_SAMPLER_MIPMAP_MODE_LINEAR         ;
      info.mipLodBias              = 0.0f                                  ;
      info.minLod                  = 0.0f                                  ;
      info.maxLod                  = 0.0f                                  ;
      
      HANDLE_ERROR( vkCreateSampler( pool.device( this->gpu ).device(), &info, nullptr, &this->sampler ) ) ;
    }

    void ImageData::transitionLayout( VkFormat format, VkImageLayout old_layout, VkImageLayout new_layout )
    {
      VkImageMemoryBarrier barrier ;
      VkPipelineStageFlags src     ;
      VkPipelineStageFlags dest    ;

      barrier                                 = {}                                     ;
      barrier.sType                           = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER ;
      barrier.oldLayout                       = old_layout                             ;
      barrier.newLayout                       = new_layout                             ;
      barrier.srcQueueFamilyIndex             = VK_QUEUE_FAMILY_IGNORED                ;
      barrier.dstQueueFamilyIndex             = VK_QUEUE_FAMILY_IGNORED                ;
      barrier.image                           = this->img                              ;
      barrier.subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT              ;
      barrier.subresourceRange.baseMipLevel   = 0                                      ;
      barrier.subresourceRange.levelCount     = 1                                      ;
      barrier.subresourceRange.baseArrayLayer = 0                                      ;
      barrier.subresourceRange.layerCount     = 1                                      ;
      
      this->cmd_buffer.record() ;

      if( old_layout == VK_IMAGE_LAYOUT_UNDEFINED && new_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL )
      {
        barrier.srcAccessMask = 0                                 ;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT      ;
        src                   = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT ;
        dest                  = VK_PIPELINE_STAGE_TRANSFER_BIT    ;
      }
      else if( old_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && new_layout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL )
      {
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT          ;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT             ;
        src                   = VK_PIPELINE_STAGE_TRANSFER_BIT        ;
        dest                  = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT ;
      }
      else
      {
        // unsupported layout transition.
      }
      
      vkCmdPipelineBarrier( this->cmd_buffer.buffer( 0 ), src, dest, 0, 0, nullptr, 0, nullptr, 1, &barrier ) ;
      
      this->cmd_buffer.stop() ;
      this->cmd_buffer.submit() ;
    }

    void ImageData::copyBufferToImage( VkBuffer buffer )
    {
      VkBufferImageCopy region ;
      
      region = {} ;
      region.bufferImageHeight               = 0                         ;
      region.bufferOffset                    = 0                         ;
      region.bufferRowLength                 = 0                         ;
      region.imageSubresource.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT ;
      region.imageSubresource.baseArrayLayer = 0                         ;
      region.imageSubresource.layerCount     = 1                         ;
      region.imageSubresource.mipLevel       = 0                         ;
      region.imageOffset.x                   = 0                         ;
      region.imageOffset.y                   = 0                         ;
      region.imageOffset.z                   = 0                         ;
      region.imageExtent.width               = this->width               ;
      region.imageExtent.height              = this->height              ;
      region.imageExtent.depth               = 1                         ;
      
      this->cmd_buffer.record() ;
      vkCmdCopyBufferToImage( this->cmd_buffer.buffer( 0 ), buffer, this->img, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region ) ;
      this->cmd_buffer.stop() ;
      this->cmd_buffer.submit() ;
    }

    void ImageData::createBuffer( VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& mem )
    {
      VkBufferCreateInfo   info       ;
      VkMemoryRequirements req        ;
      VkMemoryAllocateInfo alloc_info ;
      DevicePool           pool       ;

      info       = {} ;
      alloc_info = {} ;
      info.sType       = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO ;
      info.size        = size                                 ;
      info.usage       = usage                                ;
      info.sharingMode = VK_SHARING_MODE_EXCLUSIVE            ;
      info.pNext       = nullptr                              ;
      
      HANDLE_ERROR( vkCreateBuffer( pool.device( this->gpu ).device(), &info, nullptr, &buffer ) ) ;
      
      vkGetBufferMemoryRequirements( pool.device( this->gpu ).device(), buffer, &req ) ;
      alloc_info.sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO                  ;
      alloc_info.allocationSize  = req.size                                                ;
      alloc_info.memoryTypeIndex = memoryType( req.memoryTypeBits, properties, this->gpu ) ;
      alloc_info.pNext           = nullptr                                                 ;
      
      HANDLE_ERROR( vkAllocateMemory( pool.device( this->gpu ).device(), &alloc_info, nullptr, &mem ) ) ;
      HANDLE_ERROR( vkBindBufferMemory( pool.device( this->gpu ).device(), buffer, mem, 0          ) ) ;
    }
    
    void ImageData::allocateVulkanImage()
    {
      DevicePool           pool    ;
      VkMemoryRequirements mem_req ;
      VkMemoryAllocateInfo info    ;

      vkGetImageMemoryRequirements( pool.device( this->gpu ).device(), this->img, &mem_req ) ;
      info.sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO                                                                  ;
      info.allocationSize  = mem_req.size                                                                                            ;
      info.memoryTypeIndex = ::casper::vulkan::memoryType( mem_req.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, this->gpu  ) ;
      info.pNext           = nullptr                                                                                                 ;
      
      HANDLE_ERROR( vkAllocateMemory ( pool.device( this->gpu ).device(), &info, nullptr, &this->memory ) ) ;
      HANDLE_ERROR( vkBindImageMemory( pool.device( this->gpu ).device(), this->img, this->memory, 0    ) ) ;      
    }

    void ImageData::generateVulkanImage()
    {
      auto usage =  VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT ;
      DevicePool        pool ;
      VkImageCreateInfo info ;
      

      info = {} ;
      info.sType         = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO ;
      info.imageType     = VK_IMAGE_TYPE_2D                    ;
      info.extent.width  = this->width                         ;
      info.extent.height = this->height                        ;
      info.extent.depth  = 1                                   ;
      info.mipLevels     = 1                                   ;
      info.arrayLayers   = 1                                   ;
      info.format        = VK_FORMAT_R8G8B8A8_SRGB             ;
      info.tiling        = VK_IMAGE_TILING_OPTIMAL             ;
      info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED           ;
      info.usage         = usage                               ;
      info.sharingMode   = VK_SHARING_MODE_EXCLUSIVE           ;
      info.samples       = VK_SAMPLE_COUNT_1_BIT               ;
      info.flags         = 0                                   ;
      info.pNext         = nullptr                             ;
      
      HANDLE_ERROR( vkCreateImage( pool.device( this->gpu ).device(), &info, nullptr, &img ) ) ;
    }

    Image::Image()
    {
      this->img_data = new ImageData() ;
    }
    
    Image::~Image()
    {
      delete this->img_data ;
    }
    
    const VkImageView& Image::view() const
    {
      return data().image_view ;
    }
    
    const VkSampler& Image::sampler() const
    {
      return data().sampler ;
    }

    void Image::copy( const unsigned char* pixels )
    {
      const auto prop  = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT ;
      const auto usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT                                           ;
      DevicePool     pool               ;
      void*          stage_data         ;
      VkBuffer       staging_buffer     ;
      VkDeviceMemory staging_buffer_mem ;
      VkDeviceSize   size               ;
      VkSubmitInfo   info               ;

      size = data().width * data().height * 4 ;
      
      data().createBuffer( size, usage, prop, staging_buffer, staging_buffer_mem ) ;
      vkMapMemory( pool.device( data().gpu ).device(), staging_buffer_mem, 0, size, 0, &stage_data ) ;
      std::memcpy( stage_data, pixels, static_cast<size_t>( size ) ) ;
      vkUnmapMemory( pool.device( data().gpu ).device(), staging_buffer_mem ) ;
      
      data().transitionLayout( VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED           , VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL     ) ;
      data().copyBufferToImage( staging_buffer ) ;
      data().transitionLayout( VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL ) ;
      
      
      vkDestroyBuffer( pool.device( data().gpu ).device(), staging_buffer    , nullptr ) ;
      vkFreeMemory   ( pool.device( data().gpu ).device(), staging_buffer_mem, nullptr ) ;
    }
    
    void Image::copy( const Image& img )
    {
      // TODO implement lmao
    }

    void Image::initialize( unsigned gpu, const char* img_path ) 
    {
      int          width  ;
      int          height ;
      int          chan   ;
      VkDeviceSize size   ;

      stbi_set_flip_vertically_on_load( true ) ;

      const unsigned char* pixels = stbi_load( img_path, &width, &height, &chan, STBI_rgb_alpha ) ;
      
      size = width * height * 4 ;
      
      if( pixels )
      {
        data().gpu    = gpu    ;
        data().width  = width  ;
        data().height = height ;
        
        data().cmd_buffer.initCompute( gpu, 1 ) ;

        data().generateVulkanImage() ;
        data().allocateVulkanImage() ;
        this->copy( pixels )         ;
        data().createImageView()     ;
        data().createSampler()       ;
      }
      else
      {
        // TODO Report error.
      }
    }
    
    void Image::initialize( unsigned gpu, unsigned w, unsigned h ) 
    {
      data().gpu    = gpu ;
      data().width  = w   ;
      data().height = h   ;
      
      data().cmd_buffer.initCompute( gpu, 1 ) ;

      data().generateVulkanImage() ;
      data().allocateVulkanImage() ;
      data().createImageView()     ;
      data().createSampler()       ;
    }
    
    ImageData& Image::data()
    {
      return *this->img_data ;
    }

    const ImageData& Image::data() const
    {
      return *this->img_data ;
    }
  }
}