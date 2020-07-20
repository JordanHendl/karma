#include "Image.h"
#include "CommandBuffer.h"
#include "Context.h"
#include <vulkan/vulkan.hpp>

namespace kgl
{
  namespace vk
  {
    /**
     * @param filter
     * @param props
     * @param gpu
     */
    static inline unsigned memoryType( unsigned filter, ::vk::MemoryPropertyFlags props, unsigned gpu ) ;

    struct ImageData
    {
      ::vk::Image                       img        ; ///< TODO
      ::vk::ImageView                   image_view ; ///< TODO
      ::vk::Sampler                     sampler    ; ///< TODO
      ::vk::DeviceMemory                memory     ; ///< TODO
      ::vk::Device                      device     ; ///< TODO
      ::kgl::vk::compute::CommandBuffer cmd_buffer ; ///< TODO
      unsigned                          gpu        ; ///< TODO
      unsigned                          width      ; ///< TODO
      unsigned                          height     ; ///< TODO
      
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
      void createBuffer( ::vk::DeviceSize size, ::vk::BufferUsageFlags usage, ::vk::MemoryPropertyFlags properties, ::vk::Buffer& buffer, ::vk::DeviceMemory& mem ) ;
      
      /**
       * @param format
       * @param old_layout
       * @param new_layout
       */
      void transitionLayout( ::vk::Format format, ::vk::ImageLayout old_layout, ::vk::ImageLayout new_layout ) ;
      
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
      void copyBufferToImage( ::vk::Buffer buffer ) ;
    };
    unsigned memoryType( unsigned filter, ::vk::MemoryPropertyFlags props, unsigned gpu )
    {
      const ::kgl::vk::compute::Context context ;
      const ::vk::PhysicalDevice device = context.physicalDevice( gpu ) ;

      ::vk::PhysicalDeviceMemoryProperties mem_prop ;
      
      mem_prop = device.getMemoryProperties() ;

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
      ::vk::ImageViewCreateInfo   info  ;
      ::vk::ImageSubresourceRange range ;
      
      range.setAspectMask    ( ::vk::ImageAspectFlagBits::eColor ) ;
      range.setBaseMipLevel  ( 0                                 ) ;
      range.setLevelCount    ( 1                                 ) ;
      range.setBaseArrayLayer( 0                                 ) ;
      range.setLayerCount    ( 1                                 ) ;

      info.setImage           ( this->img                   ) ;
      info.setViewType        ( ::vk::ImageViewType::e2D    ) ;
      info.setFormat          ( ::vk::Format::eR8G8B8A8Srgb ) ;
      info.setSubresourceRange( range                       ) ;
      
      this->image_view = this->device.createImageView( info, nullptr ) ;
    }
    
    void ImageData::createSampler()
    {
      const auto max_anisotropy = 16.0f ;
      ::vk::SamplerCreateInfo info ;
      
      info.setMagFilter              ( ::vk::Filter::eLinear              ) ;
      info.setMinFilter              ( ::vk::Filter::eLinear              ) ;
      info.setAddressModeU           ( ::vk::SamplerAddressMode::eRepeat  ) ;
      info.setAddressModeV           ( ::vk::SamplerAddressMode::eRepeat  ) ;
      info.setAddressModeW           ( ::vk::SamplerAddressMode::eRepeat  ) ;
      info.setBorderColor            ( ::vk::BorderColor::eIntOpaqueBlack ) ;
      info.setCompareOp              ( ::vk::CompareOp::eAlways           ) ;
      info.setMipmapMode             ( ::vk::SamplerMipmapMode::eLinear   ) ;
      info.setAnisotropyEnable       ( ::vk::Bool32( true )               ) ;
      info.setUnnormalizedCoordinates( ::vk::Bool32( false )              ) ;
      info.setCompareEnable          ( ::vk::Bool32( false )              ) ;
      info.setMaxAnisotropy          ( max_anisotropy                     ) ;
      info.setMipLodBias             ( 0.0f                               ) ;
      info.setMinLod                 ( 0.0f                               ) ;
      info.setMaxLod                 ( 0.0f                               ) ;
      
      this->sampler = this->device.createSampler( info, nullptr ) ;
    }

    void ImageData::transitionLayout( ::vk::Format format, ::vk::ImageLayout old_layout, ::vk::ImageLayout new_layout )
    {
      ::vk::ImageMemoryBarrier    barrier ;
      ::vk::ImageSubresourceRange range   ;
      ::vk::PipelineStageFlags    src     ;
      ::vk::PipelineStageFlags    dest    ;
      ::vk::DependencyFlags       flags   ;

      range.setBaseArrayLayer( 0                                 ) ;
      range.setBaseMipLevel  ( 0                                 ) ;
      range.setLevelCount    ( 1                                 ) ;
      range.setLayerCount    ( 1                                 ) ;
      range.setAspectMask    ( ::vk::ImageAspectFlagBits::eColor ) ;

      barrier.setOldLayout       ( old_layout ) ;
      barrier.setNewLayout       ( new_layout ) ;
      barrier.setImage           ( this->img  ) ;
      barrier.setSubresourceRange( range      ) ;
      
      this->cmd_buffer.record() ;

      if( old_layout == ::vk::ImageLayout::eUndefined && new_layout == ::vk::ImageLayout::eTransferDstOptimal )
      {
        barrier.setDstAccessMask( ::vk::AccessFlagBits::eTransferWrite ) ;
        
        src  = ::vk::PipelineStageFlagBits::eTopOfPipe ;
        dest = ::vk::PipelineStageFlagBits::eTransfer  ;
      }
      else if( old_layout == ::vk::ImageLayout::eTransferDstOptimal && new_layout == ::vk::ImageLayout::eShaderReadOnlyOptimal )
      {
        barrier.setSrcAccessMask( ::vk::AccessFlagBits::eTransferWrite ) ;
        barrier.setDstAccessMask( ::vk::AccessFlagBits::eShaderRead    ) ;
        
        src  = ::vk::PipelineStageFlagBits::eTransfer ;
        dest = ::vk::PipelineStageFlagBits::eFragmentShader ;
      }
      else
      {
        // unsupported layout transition.
      }

      this->cmd_buffer.buffer( 0 ).pipelineBarrier( src, dest, flags, 0, nullptr, 0, nullptr, 1, &barrier ) ;
      this->cmd_buffer.stop() ;
      this->cmd_buffer.submit() ;
    }

    void ImageData::copyBufferToImage( ::vk::Buffer buffer )
    {
      ::vk::BufferImageCopy        region      ;
      ::vk::ImageSubresourceLayers subresource ;
      ::vk::Extent3D               extent      ;
      ::vk::Offset3D               offset      ;

      subresource.setAspectMask    ( ::vk::ImageAspectFlagBits::eColor ) ;
      subresource.setBaseArrayLayer( 0                                 ) ;
      subresource.setLayerCount    ( 1                                 ) ;
      subresource.setMipLevel      ( 0                                 ) ;
      
      extent.setWidth ( this->width  ) ;
      extent.setHeight( this->height ) ;
      extent.setDepth ( 1            ) ;
      
      offset.setX( 0 ) ;
      offset.setY( 0 ) ;
      offset.setZ( 0 ) ;

      region.setBufferImageHeight( 0           ) ;
      region.setBufferOffset     ( 0           ) ;
      region.setBufferRowLength  ( 0           ) ;
      region.setImageSubresource ( subresource ) ;
      region.setImageOffset      ( offset      ) ;
      region.setImageExtent      ( extent      ) ;
      
      this->cmd_buffer.record() ;
      
      this->cmd_buffer.buffer( 0 ).copyBufferToImage( buffer, this->img, ::vk::ImageLayout::eTransferDstOptimal, 1, &region ) ;
      this->cmd_buffer.stop() ;
      this->cmd_buffer.submit() ;
    }

    void ImageData::createBuffer( ::vk::DeviceSize size, ::vk::BufferUsageFlags usage, ::vk::MemoryPropertyFlags properties, ::vk::Buffer& buffer, ::vk::DeviceMemory& mem )
    {
      ::vk::BufferCreateInfo   info       ;
      ::vk::MemoryRequirements req        ;
      ::vk::MemoryAllocateInfo alloc_info ;
      ::vk::DeviceSize         offset     ;

      info.setSize       ( size                          ) ;
      info.setUsage      ( usage                         ) ;
      info.setSharingMode( ::vk::SharingMode::eExclusive ) ;
      
      buffer = this->device.createBuffer               ( info, nullptr ) ;
      req    = this->device.getBufferMemoryRequirements( buffer        ) ;
      

      alloc_info.setAllocationSize ( req.size                                                           ) ;
      alloc_info.setMemoryTypeIndex( ::kgl::vk::memoryType( req.memoryTypeBits, properties, this->gpu ) ) ;
      
      mem = this->device.allocateMemory( alloc_info          ) ;
      this->device.bindBufferMemory    ( buffer, mem, offset ) ;
    }
    
    void ImageData::allocateVulkanImage()
    {
      ::vk::MemoryRequirements mem_req ;
      ::vk::MemoryAllocateInfo info    ;

      mem_req = this->device.getImageMemoryRequirements( this->img ) ;
      
      info.setAllocationSize ( mem_req.size                                                                                     ) ;
      info.setMemoryTypeIndex( ::kgl::vk::memoryType( mem_req.memoryTypeBits, ::vk::MemoryPropertyFlagBits::eDeviceLocal, this->gpu  ) ) ;
      
      this->memory = this->device.allocateMemory( info, nullptr ) ;
      this->device.bindImageMemory( this->img, this->memory, 0 )  ;
    }

    void ImageData::generateVulkanImage()
    {
      auto usage =  ::vk::ImageUsageFlagBits::eTransferDst | ::vk::ImageUsageFlagBits::eSampled ;

      ::vk::ImageCreateInfo info   ;
      ::vk::Extent3D        extent ;
      
      extent.setWidth ( this->width  ) ;
      extent.setHeight( this->height ) ;
      extent.setDepth ( 1            ) ;
      
      info.setExtent       ( extent                        ) ;
      info.setUsage        ( usage                         ) ;
      info.setImageType    ( ::vk::ImageType::e2D          ) ;
      info.setFormat       ( ::vk::Format::eR8G8B8A8Srgb   ) ;
      info.setTiling       ( ::vk::ImageTiling::eOptimal   ) ;
      info.setInitialLayout( ::vk::ImageLayout::eUndefined ) ;
      info.setSharingMode  ( ::vk::SharingMode::eExclusive ) ;
      info.setSamples      ( ::vk::SampleCountFlagBits::e1 ) ;
              
      
      this->img = this->device.createImage( info, nullptr ) ;
    }

    Image::Image()
    {
      this->img_data = new ImageData() ;
    }
    
    Image::~Image()
    {
      delete this->img_data ;
    }
    
    const ::vk::ImageView Image::view() const
    {
      return data().image_view ;
    }
    
    const ::vk::Sampler Image::sampler() const
    {
      return data().sampler ;
    }

    void Image::copy( const unsigned char* pixels )
    { 
      const auto prop  = ::vk::MemoryPropertyFlagBits::eHostVisible | ::vk::MemoryPropertyFlagBits::eHostCoherent ;
      const auto usage = ::vk::BufferUsageFlagBits::eTransferSrc                                                  ;
      void*                stage_data         ;
      ::vk::Buffer         staging_buffer     ;
      ::vk::DeviceMemory   staging_buffer_mem ;
      ::vk::DeviceSize     offset             ;
      ::vk::DeviceSize     size               ;
      ::vk::MemoryMapFlags flags              ;

      size   = data().width * data().height * 4 ;
      offset = 0                                ;

      data().createBuffer( size, usage, prop, staging_buffer, staging_buffer_mem ) ;
      data().device.mapMemory( staging_buffer_mem, offset, size, flags, &stage_data ) ;
      std::memcpy( stage_data, pixels, static_cast<size_t>( size ) ) ;
      data().device.unmapMemory( staging_buffer_mem ) ;
      
      data().transitionLayout( ::vk::Format::eR8G8B8A8Srgb, ::vk::ImageLayout::eUndefined, ::vk::ImageLayout::eTransferDstOptimal ) ;
      data().copyBufferToImage( staging_buffer ) ;
      data().transitionLayout( ::vk::Format::eR8G8B8A8Srgb, ::vk::ImageLayout::eTransferDstOptimal, ::vk::ImageLayout::eShaderReadOnlyOptimal ) ;
      
      data().device.destroyBuffer( staging_buffer    , nullptr ) ;
      data().device.freeMemory   ( staging_buffer_mem, nullptr ) ;
    }
    
    void Image::copy( const Image& img )
    {
      // TODO implement lmao
    }

    void Image::initialize( unsigned gpu, unsigned w, unsigned h ) 
    {
      data().gpu    = gpu ;
      data().width  = w   ;
      data().height = h   ;
      
      data().cmd_buffer.initialize( gpu, 1, ::kgl::vk::BufferLevel::Primary ) ;

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
