#include "Swapchain.h"
#include "Device.h"
#include "Library.h"
#include "Surface.h"
#include "RenderPass.h"
#include <vulkan/vulkan.hpp>
#include <stdexcept>
#include <vector>

namespace kgl
{
  namespace vk
  {
    struct SwapChainSupport
    {
      ::vk::SurfaceCapabilitiesKHR        capabilities  ;
      std::vector<::vk::SurfaceFormatKHR> formats       ;
      std::vector<::vk::PresentModeKHR>   present_modes ;

      ::vk::SurfaceFormatKHR format( ::vk::Format value, ::vk::ColorSpaceKHR color )
      {
        for( const auto& format : formats )
        {
          if( format.format == value && format.colorSpace == color )
          {
            return format ;
          }
        }

        return formats.at( 0 ) ;
      }
      
      ::vk::PresentModeKHR mode( ::vk::PresentModeKHR value )
      {
        for( const auto& pmode : present_modes )
        {
          if( pmode == value )
          {
            return pmode ;
          }
        }

        return ::vk::PresentModeKHR::eFifo ;
      }

      ::vk::Extent2D chooseExtent( unsigned width, unsigned height )
      {
        ::vk::Extent2D extent ;

        if( capabilities.currentExtent.width != UINT32_MAX )
        {
          return capabilities.currentExtent ;
        }

        extent.width  = std::max( capabilities.minImageExtent.width , std::min( capabilities.maxImageExtent.width , extent.width  ) ) ;
        extent.height = std::max( capabilities.minImageExtent.height, std::min( capabilities.maxImageExtent.height, extent.height ) ) ;
        
        return extent ;
      }
    };

    struct SwapChainData
    {
      typedef std::vector<::vk::Image>       SwapImages     ;
      typedef std::vector<::vk::ImageView>   SwapImageViews ;
      typedef std::vector<::vk::Framebuffer> FrameBuffers   ;

      FrameBuffers       buffers    ; ///< TODO
      SwapImages         images     ; ///< TODO
      SwapImageViews     views      ; ///< TODO
      SwapChainSupport   details    ; ///< TODO
      unsigned           present    ; ///< TODO
      ::vk::SwapchainKHR chain      ; ///< TODO
      ::vk::Format       img_format ; ///< TODO
      ::vk::Queue        presentQ   ; ///< TODO
      ::vk::Extent2D     extent     ; ///< TODO
      ::vk::Device       device     ; ///< TODO
    };

    void SwapChain::createFrameBuffers( ::kgl::vk::RenderPass& render_pass )
    {
      ::vk::FramebufferCreateInfo info           ;
      ::vk::ImageView             attachments[1] ;

      data().buffers.resize( data().views.size() ) ;
      
      for( unsigned i = 0; i < data().views.size(); i++ )
      {
        attachments[0] = { data().views.at( i ) } ;
        
        info.setRenderPass     ( render_pass.pass()   ) ;
        info.setAttachmentCount( 1                    ) ;
        info.setPAttachments   ( attachments          ) ;
        info.setWidth          ( data().extent.width  ) ;
        info.setHeight         ( data().extent.height ) ;
        info.setLayers         ( 1                    ) ;
        
        data().device.createFramebuffer( &info, nullptr, &data().buffers[ i ] ) ;
      }
      render_pass.setArea( 0, 0, data().extent.width, data().extent.height ) ;
      render_pass.setFramebuffers( data().buffers.data(), data().buffers.size() ) ;
    }

    void SwapChain::findProperties( const Device& device, const Surface& surface )
    {
      const ::vk::PhysicalDevice pdevice = device.physicalDevice() ;
      const ::vk::SurfaceKHR     surf    = surface.surface()       ;

      data().details.formats       = pdevice.getSurfaceFormatsKHR     ( surf ) ;
      data().details.capabilities  = pdevice.getSurfaceCapabilitiesKHR( surf ) ;
      data().details.present_modes = pdevice.getSurfacePresentModesKHR( surf ) ;
    }

    int SwapChain::format() const
    {
      return static_cast<int>( data().img_format ) ;
    }
    
    SwapChain::SwapChain()
    {
      this->chain_data = new SwapChainData() ;
    }
    
    SwapChain::SwapChain( const SwapChain& chain )
    {
      this->chain_data = new SwapChainData() ;
      *this->chain_data = *chain.chain_data ;
    }
    void SwapChain::initialize( const Surface& surface, const Device& device )
    {
      this->findProperties( device, surface ) ;
      
      const unsigned present   = device.presentFamily()                                                                    ;
      const uint32_t indices[] = { device.graphicsFamily(), present }                                                      ;
      const unsigned width     = surface.width()                                                                           ;
      const unsigned height    = surface.height()                                                                          ;
      const auto     format    = data().details.format( ::vk::Format::eB8G8R8A8Srgb, ::vk::ColorSpaceKHR::eSrgbNonlinear ) ;

      ::vk::SwapchainCreateInfoKHR info  ; 
      ::vk::SwapchainKHR           chain ;

      info.setSurface         ( surface.surface()                                       ) ;
      info.setMinImageCount   ( data().details.capabilities.minImageCount + 1           ) ;
      info.setImageFormat     ( format.format                                           ) ;
      info.setImageColorSpace ( format.colorSpace                                       ) ;
      info.setImageExtent     ( data().details.chooseExtent( width, height )            ) ;
      info.setImageArrayLayers( 1                                                       ) ;
      info.setImageUsage      ( ::vk::ImageUsageFlagBits::eColorAttachment              ) ;
      info.setPreTransform    ( data().details.capabilities.currentTransform            ) ;
      info.setCompositeAlpha  ( ::vk::CompositeAlphaFlagBitsKHR::eOpaque                ) ;
      info.setPresentMode     ( data().details.mode( ::vk::PresentModeKHR::eImmediate ) ) ;
      

      if( device.graphicsFamily() != present )
      {
        info.setImageSharingMode     ( ::vk::SharingMode::eConcurrent ) ;
        info.setQueueFamilyIndexCount( 2                              ) ; 
        info.setPQueueFamilyIndices  ( indices                        ) ;
      }
      else
      {
        info.setImageSharingMode     ( ::vk::SharingMode::eExclusive ) ;
        info.setQueueFamilyIndexCount( 0                             ) ; 
        info.setPQueueFamilyIndices  ( nullptr                       ) ;
      }

      device.device().createSwapchainKHR( &info, nullptr, &chain ) ;
      
      data().images = device.device().getSwapchainImagesKHR( chain ) ;
      
      data().img_format = format.format                                 ;
      data().extent     = data().details.chooseExtent( width, height )  ;
      data().chain      = chain                                         ;
      data().device     = device.device()                               ;

      this->findImageViews( device, surface ) ;
    }
    unsigned SwapChain::width() const
    {
      return data().extent.width ;
    }

    unsigned SwapChain::height() const
    {
      return data().extent.height ;
    }

    const ::vk::Framebuffer SwapChain::buffer( unsigned id ) const
    {
      static ::vk::Framebuffer dummy ;

      return id < data().buffers.size() ? data().buffers[ id ] : dummy ;
    }

    unsigned SwapChain::numBuffers() const
    {
      return data().images.size() ;
    }

    void SwapChain::findImageViews( const Device& device, const Surface& surface ) 
    { 
      ::vk::ImageViewCreateInfo   info              ;
      ::vk::ComponentMapping      mapping           ;
      ::vk::ImageSubresourceRange subresource_range ;

      data().views.resize( data().images.size() ) ;
      
      mapping.r = ::vk::ComponentSwizzle::eIdentity ;
      mapping.g = ::vk::ComponentSwizzle::eIdentity ;
      mapping.b = ::vk::ComponentSwizzle::eIdentity ;
      mapping.a = ::vk::ComponentSwizzle::eIdentity ;
      
      subresource_range.setAspectMask    ( ::vk::ImageAspectFlagBits::eColor ) ;
      subresource_range.setLevelCount    ( 1                                 ) ;
      subresource_range.setLayerCount    ( 1                                 ) ;
      subresource_range.setBaseMipLevel  ( 0                                 ) ;
      subresource_range.setBaseArrayLayer( 0                                 ) ;

      info.setComponents      ( mapping                  ) ;
      info.setViewType        ( ::vk::ImageViewType::e2D ) ;
      info.setSubresourceRange( subresource_range        ) ;

      for( unsigned i = 0; i < data().views.size(); i++ )
      {
        info.setImage ( data().images[ i ] ) ;
        info.setFormat( data().img_format  ) ;
        
        device.device().createImageView( &info, nullptr, &data().views[ i ] ) ;
      }
    }
    
    const ::vk::SwapchainKHR SwapChain::chain() const
    {
      return data().chain ;
    }

    SwapChain::~SwapChain()
    {
      delete this->chain_data ;
    }

    void SwapChain::reset()
    {
      for( auto view : data().views )
      {
        data().device.destroy( view, nullptr ) ;
      }
      
      data().device.destroy( data().chain ) ;
    }

    SwapChainData& SwapChain::data()
    {
      return *this->chain_data ;
    }

    const SwapChainData& SwapChain::data() const
    {
      return *this->chain_data ;
    }
  }
}