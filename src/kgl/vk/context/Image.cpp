#include "Image.h"
#include "CommandBuffer.h"
#include "Context.h"
#include "Synchronization.h"
#include <vulkan/vulkan.hpp>
#include <iostream>

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
    
    /**
     * @param layout
     * @return 
     */
    static inline ::vk::PipelineStageFlags flagFromLayout( ::vk::ImageLayout layout ) ;
    static inline ::vk::AccessFlags accessFromLayout( ::vk::ImageLayout layout ) ;

    struct ImageData
    {
      ::vk::Image                       img        ; ///< TODO
      ::vk::Format                      format     ;
      ::vk::ImageView                   image_view ; ///< TODO
      ::vk::Sampler                     sampler    ; ///< TODO
      ::vk::DeviceMemory                memory     ; ///< TODO
      ::vk::Device                      device     ; ///< TODO
      ::kgl::vk::compute::CommandBuffer cmd_buffer ; ///< TODO
      mutable ::vk::ImageLayout         layout     ; ///< TODO
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
      void transitionLayout( ::vk::Format format, ::vk::ImageLayout old_layout, ::vk::ImageLayout new_layout, ::vk::CommandBuffer& buffer ) const ;
      
      /**
       * @param format
       * @param old_layout
       * @param new_layout
       */
      void transitionLayout( ::vk::Format format, ::vk::ImageLayout old_layout, ::vk::ImageLayout new_layout ) const ;
      
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

      /**
       * 
       * @param buffer
       */
      void copyImageToImage( ::vk::Image img, ::vk::ImageLayout layout ) ;
    };
    
    ::vk::PipelineStageFlags flagFromLayout( ::vk::ImageLayout layout )
    {
      switch( layout ) 
      {
        case ::vk::ImageLayout::eUndefined          : return ::vk::PipelineStageFlagBits::eTopOfPipe   ;
        case ::vk::ImageLayout::eTransferDstOptimal : return ::vk::PipelineStageFlagBits::eTransfer    ;
        case ::vk::ImageLayout::eTransferSrcOptimal : return ::vk::PipelineStageFlagBits::eTransfer    ;
        case ::vk::ImageLayout::eGeneral            : return ::vk::PipelineStageFlagBits::eAllCommands ;
        default : return ::vk::PipelineStageFlagBits::eAllCommands ;
      };
    }
    
    ::vk::AccessFlags accessFromLayout( ::vk::ImageLayout layout )
    {
      using ::vk::AccessFlagBits ;
      switch( layout ) 
      {
        case ::vk::ImageLayout::eUndefined          : return ::vk::AccessFlagBits::eMemoryRead                                       ;
        case ::vk::ImageLayout::eTransferDstOptimal : return ::vk::AccessFlagBits::eTransferWrite                                    ;
        case ::vk::ImageLayout::eTransferSrcOptimal : return ::vk::AccessFlagBits::eTransferRead                                     ;
        case ::vk::ImageLayout::eGeneral            : return ::vk::AccessFlagBits::eShaderRead | ::vk::AccessFlagBits::eShaderWrite  ;
        default : return ::vk::AccessFlagBits::eMemoryRead   | ::vk::AccessFlagBits::eMemoryWrite ;
      };
    }

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
      this->gpu    = 0                             ;
      this->width  = 1280                          ;
      this->height = 1024                          ;
      this->layout = ::vk::ImageLayout::eUndefined ;
      this->format = ::vk::Format::eR8G8B8A8Srgb   ;
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
      info.setFormat          ( this->format                ) ;
      info.setSubresourceRange( range                       ) ;
      
      this->image_view = this->device.createImageView( info, nullptr ) ;
    }
    
    void ImageData::createSampler()
    {
      const auto max_anisotropy = 16.0f ;
      ::vk::SamplerCreateInfo info ;
      
      info.setMagFilter              ( ::vk::Filter::eNearest                  ) ;
      info.setMinFilter              ( ::vk::Filter::eNearest                  ) ;
      info.setAddressModeU           ( ::vk::SamplerAddressMode::eClampToBorder ) ;
      info.setAddressModeV           ( ::vk::SamplerAddressMode::eClampToBorder ) ;
      info.setAddressModeW           ( ::vk::SamplerAddressMode::eClampToBorder ) ;
      info.setBorderColor            ( ::vk::BorderColor::eIntTransparentBlack  ) ;
      info.setCompareOp              ( ::vk::CompareOp::eNever                  ) ;
      info.setMipmapMode             ( ::vk::SamplerMipmapMode::eNearest        ) ;
      info.setAnisotropyEnable       ( ::vk::Bool32( true  )                    ) ;
      info.setUnnormalizedCoordinates( ::vk::Bool32( false )                    ) ;
      info.setCompareEnable          ( ::vk::Bool32( false )                    ) ;
      info.setMaxAnisotropy          ( max_anisotropy                           ) ;
      info.setMipLodBias             ( 0.0f                                     ) ;
      info.setMinLod                 ( 0.0f                                     ) ;
      info.setMaxLod                 ( 0.0f                                     ) ;
      
      this->sampler = this->device.createSampler( info, nullptr ) ;
    }

    void ImageData::transitionLayout( ::vk::Format format, ::vk::ImageLayout old_layout, ::vk::ImageLayout new_layout, ::vk::CommandBuffer& buffer ) const
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
      barrier.setSrcAccessMask   ( ::vk::AccessFlagBits::eShaderRead ) ;
      barrier.setDstAccessMask   ( ::vk::AccessFlagBits::eColorAttachmentWrite ) ;
      
//      if( old_layout != ::vk::ImageLayout::eUndefined )
//      {
//        barrier.setSrcAccessMask( accessFromLayout( old_layout ) ) ;
//      }
      flags = ::vk::DependencyFlagBits::eDeviceGroup ;
      barrier.setDstAccessMask( accessFromLayout( new_layout ) ) ;
//      src  = flagFromLayout( old_layout ) ;
//      dest = flagFromLayout( new_layout ) ;
      src  = ::vk::PipelineStageFlagBits::eAllCommands           ;
      dest = ::vk::PipelineStageFlagBits::eFragmentShader        ;

      buffer.pipelineBarrier( src, dest, flags, 0, nullptr, 0, nullptr, 1, &barrier ) ;
      this->layout = new_layout ;
    }
    
    void ImageData::transitionLayout( ::vk::Format format, ::vk::ImageLayout old_layout, ::vk::ImageLayout new_layout ) const
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
      barrier.setSrcAccessMask   ( ::vk::AccessFlagBits::eMemoryRead ) ;
      barrier.setDstAccessMask   ( ::vk::AccessFlagBits::eMemoryWrite ) ;
      
      
      if( old_layout != ::vk::ImageLayout::eUndefined )
      {
        barrier.setSrcAccessMask( accessFromLayout( old_layout ) ) ;
      }
      
      barrier.setDstAccessMask( accessFromLayout( new_layout ) ) ;
      src  = flagFromLayout( old_layout ) ;
      dest = flagFromLayout( new_layout ) ;

      this->cmd_buffer.record() ;
      this->cmd_buffer.buffer( 0 ).pipelineBarrier( src, dest, flags, 0, nullptr, 0, nullptr, 1, &barrier ) ;
      this->cmd_buffer.stop() ;
      this->cmd_buffer.submit() ;
    }

    void ImageData::copyBufferToImage( ::vk::Buffer buffer )
    {
      const ::kgl::vk::compute::Context context ;
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
      return this->cmd_buffer.submit() ;
    }

    void ImageData::copyImageToImage( ::vk::Image img, ::vk::ImageLayout layout )
    {
      const ::kgl::vk::compute::Context context ;
      
      ::vk::ImageMemoryBarrier     barrier     ;
      ::vk::ImageCopy              region      ;
      ::vk::ImageSubresourceRange  range       ;
      ::vk::ImageSubresourceLayers subresource ;
      ::vk::Extent3D               extent      ;
      ::vk::Offset3D               offset      ;
      ::vk::DependencyFlags        flags       ;

      range.setBaseArrayLayer( 0                                 ) ;
      range.setBaseMipLevel  ( 0                                 ) ;
      range.setLevelCount    ( 1                                 ) ;
      range.setLayerCount    ( 1                                 ) ;
      range.setAspectMask    ( ::vk::ImageAspectFlagBits::eColor ) ;

      subresource.setAspectMask    ( ::vk::ImageAspectFlagBits::eColor ) ;
      subresource.setBaseArrayLayer( 0                                 ) ;
      subresource.setLayerCount    ( 1                                 ) ;
      subresource.setMipLevel      ( 0                                 ) ;
      
      extent.setWidth ( this->width  ) ;
      extent.setHeight( this->height ) ;
      extent.setDepth ( 1            ) ;
      
      barrier.setOldLayout       ( this->layout ) ;
      barrier.setNewLayout       ( this->layout ) ;
      barrier.setImage           ( this->img    ) ;
      barrier.setSubresourceRange( range        ) ;
      
      offset.setX( 0 ) ;
      offset.setY( 0 ) ;
      offset.setZ( 0 ) ;
      
      region.setExtent        ( extent      ) ;
      region.setSrcSubresource( subresource ) ;
      region.setSrcOffset     ( 0           ) ;
      region.setDstSubresource( subresource ) ;
      region.setDstOffset     ( 0           ) ;
      
      
      this->cmd_buffer.record() ;
         
      this->cmd_buffer.buffer( 0 ).copyImage( img, layout, this->img, this->layout, 1, &region ) ;
      this->cmd_buffer.buffer( 0 ).pipelineBarrier( ::vk::PipelineStageFlagBits::eAllGraphics, ::vk::PipelineStageFlagBits::eAllGraphics, flags, 0, nullptr, 0, nullptr, 1, &barrier ) ;
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
      offset = 0                                                         ;
      
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
      
      info.setAllocationSize ( mem_req.size                                                                                            ) ;
      info.setMemoryTypeIndex( ::kgl::vk::memoryType( mem_req.memoryTypeBits, ::vk::MemoryPropertyFlagBits::eDeviceLocal, this->gpu  ) ) ;
      
      this->memory = this->device.allocateMemory( info, nullptr ) ;
      this->device.bindImageMemory( this->img, this->memory, 0 )  ;
    }

    void ImageData::generateVulkanImage()
    {
      auto usage =  ::vk::ImageUsageFlagBits::eTransferSrc | ::vk::ImageUsageFlagBits::eTransferDst | ::vk::ImageUsageFlagBits::eSampled ;
      if( this->format == ::vk::Format::eR8G8B8A8Srgb )
      {
        usage |= ::vk::ImageUsageFlagBits::eColorAttachment ;
      }
      else
      {
      }

      ::vk::ImageCreateInfo info   ;
      ::vk::Extent3D        extent ;
      
      extent.setWidth ( this->width  ) ;
      extent.setHeight( this->height ) ;
      extent.setDepth ( 1            ) ;
      
      info.setExtent       ( extent                        ) ;
      info.setUsage        ( usage                         ) ;
      info.setImageType    ( ::vk::ImageType::e2D          ) ;
      info.setFormat       ( this->format                  ) ;
      info.setTiling       ( ::vk::ImageTiling::eOptimal   ) ;
      info.setInitialLayout( ::vk::ImageLayout::eUndefined ) ;
      info.setSharingMode  ( ::vk::SharingMode::eExclusive ) ;
      info.setSamples      ( ::vk::SampleCountFlagBits::e1 ) ;
      info.setMipLevels    ( 1                             ) ;
      info.setArrayLayers  ( 1                             ) ;
      
      this->img = this->device.createImage( info, nullptr ) ;
    }
    
    void Image::reset()
    {
      const ::kgl::vk::compute::Context context ;
      const ::vk::Device device = context.virtualDevice( data().gpu ) ;

      device.destroy( data().sampler    ) ;
      device.destroy( data().image_view ) ;
      device.free   ( data().memory     ) ;
      device.destroy( data().img        ) ;
    }

    Image::Image()
    {
      this->img_data = new ImageData() ;
    }
    
    Image::Image( const Image& image )
    {
      this->img_data = new ImageData() ;
      *this->img_data = *image.img_data ;
      data().cmd_buffer = image.data().cmd_buffer ;
    }
    
    Image::~Image()
    {
      delete this->img_data ;
    }
    
    const ::vk::ImageLayout& Image::layout() const
    {
      return data().layout ;
    }
    
    void Image::setFormat( const ::vk::Format& format )
    {
      data().format = format ;
    }

    void Image::setLayout( const ::vk::ImageLayout& layout ) const
    {
      data().transitionLayout( data().format, data().layout, layout ) ;
    }
    
    unsigned Image::width() const
    {
      return data().width ;
    }
    
    unsigned Image::height() const
    {
      return data().height ;
    }

    const ::vk::ImageView Image::view() const
    {
      return data().image_view ;
    }
    
    const ::vk::Sampler Image::sampler() const
    {
      return data().sampler ;
    }

    void Image::copy( const unsigned char* pixels, unsigned channels )
    { 
      const auto prop  = ::vk::MemoryPropertyFlagBits::eHostVisible | ::vk::MemoryPropertyFlagBits::eHostCoherent ;
      const auto usage = ::vk::BufferUsageFlagBits::eTransferSrc                                                  ;
      void*                stage_data         ;
      ::vk::Buffer         staging_buffer     ;
      ::vk::DeviceMemory   staging_buffer_mem ;
      ::vk::DeviceSize     offset             ;
      ::vk::DeviceSize     size               ;
      ::vk::MemoryMapFlags flags              ;

      size   = data().width * data().height * channels ;
      offset = 0                                       ;

      data().createBuffer( size, usage, prop, staging_buffer, staging_buffer_mem ) ;
      data().device.mapMemory( staging_buffer_mem, offset, size, flags, &stage_data ) ;
      std::memcpy( stage_data, pixels, static_cast<size_t>( size ) ) ;
      data().device.unmapMemory( staging_buffer_mem ) ;
      
      data().transitionLayout( data().format, data().layout, ::vk::ImageLayout::eTransferDstOptimal ) ;
      data().copyBufferToImage( staging_buffer ) ;
      data().transitionLayout( data().format, ::vk::ImageLayout::eTransferDstOptimal, ::vk::ImageLayout::eGeneral ) ;
      
      data().device.destroyBuffer( staging_buffer    , nullptr ) ;
      data().device.freeMemory   ( staging_buffer_mem, nullptr ) ;
    }
    
    void Image::copy( const Image& img, Synchronization& sync, ::vk::CommandBuffer& buffer )
    {
      const auto old_layout = img.data().layout ;

      if( data().width != img.data().width || data().height != img.data().height )
      {
        data().width  = img.data().width  ;
        data().height = img.data().height ;
        this->reset() ;
        this->initialize( data().gpu, data().width, data().height ) ;
      }

          data().transitionLayout( data().format    , data().layout    , ::vk::ImageLayout::eTransferDstOptimal, buffer ) ;
      img.data().transitionLayout( img.data().format, img.data().layout, ::vk::ImageLayout::eTransferSrcOptimal, buffer ) ;
          data().copyImageToImage( img.data().img, img.data().layout ) ;
          data().transitionLayout( data().format    , ::vk::ImageLayout::eTransferDstOptimal, ::vk::ImageLayout::eGeneral, buffer ) ;
      img.data().transitionLayout( img.data().format, ::vk::ImageLayout::eTransferSrcOptimal, old_layout                 , buffer ) ;
    } 

    Image& Image::operator=( const Image& image )
    {
      *this->img_data = *image.img_data ;
      data().cmd_buffer = image.data().cmd_buffer ;
      return *this ;
    }

    bool Image::operator<( const Image& image ) const
    {
      return ( this->data().image_view ) < ( image.data().image_view ) ;
    }
        
    void Image::initialize( unsigned gpu, unsigned w, unsigned h ) 
    {
      const ::kgl::vk::compute::Context context ;
      data().gpu    = gpu                          ;
      data().width  = w                            ;
      data().height = h                            ;
      data().device = context.virtualDevice( gpu ) ;
      
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
