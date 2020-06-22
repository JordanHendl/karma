#include "casper_vulkan_swapchain.h"
#include "casper_vulkan_device.h"
#include "casper_vulkan_surface.h"
#include <vulkan/vulkan.h>
#include <stdexcept>
#include <vector>

namespace casper
{
  namespace vulkan
  {
    struct SwapChainSupportDetails
    {
      VkSurfaceCapabilitiesKHR        capabilities  ;
      std::vector<VkSurfaceFormatKHR> formats       ;
      std::vector<VkPresentModeKHR>   present_modes ;

      VkSurfaceFormatKHR format( VkFormat value, VkColorSpaceKHR color )
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
      
      VkPresentModeKHR mode( VkPresentModeKHR value )
      {
        for( const auto& pmode : present_modes )
        {
          if( pmode == value )
          {
            return pmode ;
          }
        }

        return VK_PRESENT_MODE_FIFO_KHR ;
      }

      VkExtent2D chooseExtent( unsigned width, unsigned height )
      {
        VkExtent2D extent ;
        if( capabilities.currentExtent.width != UINT32_MAX )
        {
          return capabilities.currentExtent ;
        }
        extent = { width, height } ;
        extent.width  = std::max( capabilities.minImageExtent.width , std::min( capabilities.maxImageExtent.width , extent.width  ) ) ;
        extent.height = std::max( capabilities.minImageExtent.height, std::min( capabilities.maxImageExtent.height, extent.height ) ) ;
        
        return extent ;
      }
    };

    struct SwapChainData
    {
      typedef std::vector<VkImage>       SwapImages     ;
      typedef std::vector<VkImageView>   SwapImageViews ;
      typedef std::vector<VkFramebuffer> FrameBuffers   ;

      FrameBuffers            buffers    ;
      SwapChainSupportDetails details    ;
      SwapImages              images     ;
      SwapImageViews          views      ;
      unsigned                present    ;
      VkSwapchainKHR          chain      ;
      VkFormat                img_format ;
      VkQueue                 presentQ   ;
      VkExtent2D              extent     ;
    };

    void SwapChain::createFrameBuffers( const VkRenderPass& render_pass, const Device& device )
    {
      VkFramebufferCreateInfo info           ;
      VkImageView             attachments[1] ;

      data().buffers.resize( data().views.size() ) ;
      
      for( unsigned i = 0; i < data().views.size(); i++ )
      {
        attachments[0] = { data().views.at( i ) } ;

        info                 = {}                                        ;
        info.sType           = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO ;
        info.renderPass      = render_pass                               ;
        info.attachmentCount = 1                                         ;
        info.pAttachments    = attachments                               ;
        info.width           = data().extent.width                       ;
        info.height          = data().extent.height                      ;
        info.layers          = 1                                         ;

        if( vkCreateFramebuffer( device.device(), &info, nullptr, &data().buffers[ i ] ) != VK_SUCCESS )
        {
          throw std::runtime_error( "Failed to create Vulkan Framebuffer." ) ;
        }
      }
    }

    void SwapChain::findProperties( const casper::vulkan::Device& device, const Surface& surface )
    {
      const VkPhysicalDevice pdevice = device.physicalDevice() ;
      const VkSurfaceKHR     surf    = surface.surface()       ;
      uint32_t format_count  ;
      uint32_t present_count ;

      vkGetPhysicalDeviceSurfaceCapabilitiesKHR( pdevice, surf, &data().details.capabilities ) ;  

      vkGetPhysicalDeviceSurfaceFormatsKHR( pdevice, surf, &format_count, NULL ) ;
      data().details.formats.resize( format_count ) ;
      vkGetPhysicalDeviceSurfaceFormatsKHR( pdevice, surf, &format_count, data().details.formats.data() ) ;

      vkGetPhysicalDeviceSurfacePresentModesKHR( pdevice, surf, &present_count, NULL ) ;
      data().details.present_modes.resize( present_count ) ;
      vkGetPhysicalDeviceSurfacePresentModesKHR( pdevice, surf, &present_count, data().details.present_modes.data() ) ;
    }

    int SwapChain::format() const
    {
      return data().img_format ;
    }

    SwapChain::SwapChain()
    {
      this->chain_data = new SwapChainData() ;
    }

    unsigned SwapChain::findPresent( const Surface& surface, const Device& device )
    {
      typedef std::vector<VkQueueFamilyProperties> FamilyPropList ;

      FamilyPropList list    ;
      uint32_t       count   ;
      uint32_t       id      ;
      VkBool32       support ;

      vkGetPhysicalDeviceQueueFamilyProperties( device.physicalDevice(), &count, nullptr ) ;
      list.resize( count ) ;
      vkGetPhysicalDeviceQueueFamilyProperties( device.physicalDevice(), &count, list.data() ) ;
      
      id = 0 ;
      support = false ;
      for (const auto& queueFamily : list ) 
      {
        vkGetPhysicalDeviceSurfaceSupportKHR( device.physicalDevice(), id, surface.surface(), &support ) ;
        if( support )
        {
          vkGetDeviceQueue( device.device(), id, 0, &data().presentQ ) ;
          data().present = id ;
          return id ;
        }

        id++ ;
      }
      return id ;
    }

    void SwapChain::initialize( const Surface& surface, const Device& device )
    {
      this->findProperties( device, surface ) ;
      
      const unsigned present   = findPresent( surface, device ) ;
      const uint32_t indices[] = { device.graphicsFamily(), present }  ;
      const unsigned width     = surface.width()                       ;
      const unsigned height    = surface.height()                      ;
      const auto     format    = data().details.format( VK_FORMAT_B8G8R8A8_SRGB ,VK_COLOR_SPACE_SRGB_NONLINEAR_KHR ) ;

      unsigned                 count ;
      VkSwapchainCreateInfoKHR info  ; 
      VkSwapchainKHR           chain ;

      info                  = {} ;

      info.sType            = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR    ;
      info.surface          = surface.surface()                              ;
      info.minImageCount    = data().details.capabilities.minImageCount + 1  ;
      info.imageFormat      = format.format                                  ;
      info.imageColorSpace  = format.colorSpace                              ;
      info.imageExtent      = data().details.chooseExtent( width, height )   ;
      info.imageArrayLayers = 1                                              ;
      info.imageUsage       = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT            ;
      info.preTransform     = data().details.capabilities.currentTransform   ;
      info.compositeAlpha   = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR              ;
      info.presentMode      = data().details.mode( VK_PRESENT_MODE_FIFO_KHR );
      info.oldSwapchain     = VK_NULL_HANDLE                                 ;

      if( device.graphicsFamily() != present )
      {
        info.imageSharingMode      = VK_SHARING_MODE_CONCURRENT ;
        info.queueFamilyIndexCount = 2                          ;
        info.pQueueFamilyIndices   = indices                    ;
      }
      else
      {
        info.imageSharingMode      = VK_SHARING_MODE_EXCLUSIVE  ;
        info.queueFamilyIndexCount = 0                          ;
        info.pQueueFamilyIndices   = NULL                       ;
      }

      if( vkCreateSwapchainKHR( device.device(), &info, nullptr, &chain ) != VK_SUCCESS )
      {
        throw std::runtime_error( "VULKAN::Failed to create swap chain!") ;
      }
        
      vkGetSwapchainImagesKHR( device.device(), chain, &count, NULL ) ;
      data().images.resize( count ) ;
      vkGetSwapchainImagesKHR( device.device(), chain, &count, data().images.data() ) ;
      
      data().img_format = format.format                                 ;
      data().extent     = data().details.chooseExtent( width, height )  ;
      data().chain      = chain                                         ;

      this->findImageViews( surface, device ) ;
    }
    unsigned SwapChain::width() const
    {
      return data().extent.width ;
    }

    unsigned SwapChain::height() const
    {
      return data().extent.height ;
    }

    const VkFramebuffer* SwapChain::buffers() const
    {
      return data().buffers.data() ;
    }

    unsigned SwapChain::numBuffers() const
    {
      return data().buffers.size() ;
    }

    void SwapChain::findImageViews( const Surface& surface, const Device& device ) 
    {
      VkImageViewCreateInfo info ;

      data().views.resize( data().images.size() ) ;

      for( unsigned i = 0; i < data().views.size(); i++ )
      {
        info.sType    = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO ;
        info.image    = data().images.at( i )                    ;
        info.viewType = VK_IMAGE_VIEW_TYPE_2D                    ;
        info.format   = data().img_format                        ;
        info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY        ;
        info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY        ;
        info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY        ;
        info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY        ;

        info.subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT ;
        info.subresourceRange.baseMipLevel   = 0                         ;
        info.subresourceRange.levelCount     = 1                         ;
        info.subresourceRange.baseArrayLayer = 0                         ;
        info.subresourceRange.layerCount     = 1                         ;

        if ( vkCreateImageView(device.device(), &info, nullptr, &data().views.at( i ) ) != VK_SUCCESS )
        {
          throw std::runtime_error( "failed to create image views!" ) ;
        }
      }
    }
    
    const VkSwapchainKHR* SwapChain::chain() const
    {
      return &data().chain ;
    }

    SwapChain::~SwapChain()
    {
      delete this->chain_data ;
    }

    void SwapChain::reset()
    {
    
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