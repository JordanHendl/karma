#include "RenderPass.h"
#include "CommandBuffer.h"
#include "Context.h"
#include "Image.h"
#include "Synchronization.h"
#include "Bus.h"
#include <vulkan/vulkan.hpp>
#include <vector>
#include <string>
#include <array>

namespace kgl
{
  namespace vk
  {
    struct AttachmentDescriptionConf
    {
      ::vk::Format              format          ;
      ::vk::SampleCountFlagBits samples         ;
      ::vk::AttachmentLoadOp    loadop          ;
      ::vk::AttachmentLoadOp    stencil_loadop  ;
      ::vk::AttachmentStoreOp   stencil_storeop ;
      ::vk::AttachmentStoreOp   storeop         ;
      ::vk::ImageLayout         init_layout     ;
      ::vk::ImageLayout         final_layout    ;
      
      AttachmentDescriptionConf()
      {
        this->samples         = ::vk::SampleCountFlagBits::e1              ;
        this->loadop          = ::vk::AttachmentLoadOp::eClear             ;
        this->storeop         = ::vk::AttachmentStoreOp::eStore            ;
        this->stencil_loadop  = ::vk::AttachmentLoadOp::eDontCare          ;
        this->stencil_storeop = ::vk::AttachmentStoreOp::eDontCare         ;
        this->init_layout     = ::vk::ImageLayout::eUndefined              ;
        this->final_layout    = ::vk::ImageLayout::eColorAttachmentOptimal ;
        this->format          = ::vk::Format::eR8G8B8A8Srgb                ;
      }
    };
    
    struct SubpassDependencyConf
    {
      uint32_t src_subpass                     ;
      uint32_t dst_subpass                     ;
      ::vk::PipelineStageFlags src_stage_mask  ;
      ::vk::PipelineStageFlags dst_stage_mask  ;
      ::vk::AccessFlags        src_access_mask ;
      ::vk::AccessFlags        dst_access_mask ;
      
      SubpassDependencyConf()
      {
        this->src_subpass     = VK_SUBPASS_EXTERNAL                                 ;
        this->dst_subpass     = 0                                                   ;
        this->src_access_mask = ::vk::AccessFlagBits::eShaderRead                   ;
        this->dst_access_mask = ::vk::AccessFlagBits::eColorAttachmentWrite         ;
        this->src_stage_mask  = ::vk::PipelineStageFlagBits::eColorAttachmentOutput ;
      }
    };

    struct RenderPassData
    {
      typedef std::vector<::vk::AttachmentReference> PassAttachments ;
      typedef std::vector<::vk::Semaphore          > Semaphores      ;
      typedef std::vector<::vk::Framebuffer        > Framebuffers    ;
      typedef std::vector<::kgl::vk::Image         > Images          ;
      
      AttachmentDescriptionConf attach_desc_conf ;
      SubpassDependencyConf     dependency_conf  ;
      
      Framebuffers               framebuffers ;
      Images                     images       ;
      PassAttachments            attachments  ;
      ::vk::ImageLayout          image_layout ;
      ::kgl::vk::render::Context context      ;
      ::vk::Device               device       ;
      ::vk::RenderPass           render_pass  ;
      ::vk::Rect2D               area         ;
      unsigned                   gpu          ;
      std::string                window       ;
     
      
      RenderPassData() ;
      void createRenderPass() ;
      void createFramebuffers() ;
    };
    
    RenderPassData::RenderPassData()
    {
      this->image_layout = ::vk::ImageLayout::eColorAttachmentOptimal ;
    }
    
    void RenderPassData::createFramebuffers()
    {
      const unsigned num_framebuffers = this->context.numFrameBuffers( this->window.c_str() ) ;
      const unsigned width            = this->context.width          ( this->window.c_str() ) ;
      const unsigned height           = this->context.height         ( this->window.c_str() ) ;
      
      ::vk::FramebufferCreateInfo info           ;
      ::vk::ImageView             attachments[1] ;
      
      this->images      .resize( num_framebuffers ) ;
      this->framebuffers.resize( num_framebuffers ) ;
      this->area.setExtent( { width, height } ) ;
      this->area.setOffset( { 0    , 0      } ) ;

      for( auto &image : this->images )
      {
        image.initialize( this->gpu, width, height ) ;
        image.setLayout( ::vk::ImageLayout::eColorAttachmentOptimal ) ;
      }
      
      for( unsigned i = 0; i < num_framebuffers; i++ )
      {
        attachments[0] = { this->images[ i ].view() } ;
        
        info.setRenderPass     ( render_pass          ) ;
        info.setAttachmentCount( 1                    ) ;
        info.setPAttachments   ( attachments          ) ;
        info.setWidth          ( width                ) ;
        info.setHeight         ( height               ) ;
        info.setLayers         ( 1                    ) ;
        
        this->device.createFramebuffer( &info, nullptr, &this->framebuffers[ i ] ) ;
      }
    }
    void RenderPassData::createRenderPass()
    {
      ::vk::RenderPassCreateInfo  info        ;
      ::vk::AttachmentDescription attach_desc ;
      ::vk::AttachmentReference   attach_ref  ;
      ::vk::SubpassDescription    sub_desc    ;
      ::vk::SubpassDependency     sub_dep     ;
      
      attach_desc.setFormat        ( this->attach_desc_conf.format          ) ;
      attach_desc.setSamples       ( this->attach_desc_conf.samples         ) ;
      attach_desc.setLoadOp        ( this->attach_desc_conf.loadop          ) ;
      attach_desc.setStoreOp       ( this->attach_desc_conf.storeop         ) ;
      attach_desc.setStencilLoadOp ( this->attach_desc_conf.stencil_loadop  ) ;
      attach_desc.setStencilStoreOp( this->attach_desc_conf.stencil_storeop ) ;
      attach_desc.setInitialLayout ( this->attach_desc_conf.init_layout     ) ;
      attach_desc.setFinalLayout   ( this->attach_desc_conf.final_layout    ) ;
      
      sub_dep.setSrcSubpass   ( this->dependency_conf.src_subpass     ) ;
      sub_dep.setDstSubpass   ( this->dependency_conf.dst_subpass     ) ;
      sub_dep.setSrcAccessMask( this->dependency_conf.src_access_mask ) ;
      sub_dep.setDstAccessMask( this->dependency_conf.dst_access_mask ) ;
      sub_dep.setSrcStageMask ( this->dependency_conf.src_stage_mask  ) ;
      sub_dep.setDstStageMask ( this->dependency_conf.dst_stage_mask  ) ;
      
      attach_ref.setLayout    ( ::vk::ImageLayout::eColorAttachmentOptimal ) ;
      attach_ref.setAttachment( 0                                          ) ;
       
      sub_desc.setPipelineBindPoint   ( ::vk::PipelineBindPoint::eGraphics                                 ) ;
      sub_desc.setColorAttachmentCount( this->attachments.empty() ? 1           : this->attachments.size() ) ;
      sub_desc.setPColorAttachments   ( this->attachments.empty() ? &attach_ref : this->attachments.data() ) ;
      
      info.setAttachmentCount( 1            ) ;
      info.setPAttachments   ( &attach_desc ) ;
      info.setSubpassCount   ( 1            ) ;
      info.setPSubpasses     ( &sub_desc    ) ;
//      info.setPDependencies  ( &sub_dep     ) ;
      
      this->device.createRenderPass( &info, nullptr, &this->render_pass ) ;
    }

    RenderPass::RenderPass()
    {
      this->pass_data = new RenderPassData() ;
    }

    RenderPass::~RenderPass()
    {
      delete this->pass_data ;
    }

    void RenderPass::initialize( const char* window_name, unsigned gpu, bool build_framebuffers )
    {
      data().window = window_name                         ;
      data().device = data().context.virtualDevice( gpu ) ;
      data().gpu    = gpu                                 ;
      
      data().createRenderPass  () ;
      if( build_framebuffers ) data().createFramebuffers() ;
    }
    
    void RenderPass::setFormat( const ::vk::Format& format )
    {
      data().attach_desc_conf.format = format ;
    }

    void RenderPass::setImageFinalLayout( const ::vk::ImageLayout& layout )
    {
      data().attach_desc_conf.final_layout = layout ;
    }

    void RenderPass::subscribe( const char* name, unsigned id )
    {
      ::data::module::Bus bus ;
      
      // subscribe for inputs here.
    }

    const ::vk::Rect2D RenderPass::area() const
    {
      return data().area ;
    }

    const ::vk::RenderPass RenderPass::pass()
    {
      return data().render_pass ;
    }

    unsigned RenderPass::numBuffers() const
    {
      return data().framebuffers.size() ;
    }

    const ::vk::Framebuffer RenderPass::buffer( unsigned id )
    {
      return id < data().framebuffers.size() ? data().framebuffers[ id ] : ::vk::Framebuffer() ;
    }
    
    void RenderPass::setFramebuffers( ::vk::Framebuffer* buffers, unsigned count )
    {
      data().framebuffers = std::vector<::vk::Framebuffer>( buffers, buffers + count ) ;
    }
    
    void RenderPass::setArea( unsigned offx, unsigned offy, unsigned width, unsigned height )
    {
      data().area.extent.width  = width  ;
      data().area.extent.height = height ;
      data().area.offset.x      = offx   ;
      data().area.offset.y      = offy   ;
    }
    
    static constexpr auto flags()
    {
    }

    void RenderPass::submit( Synchronization& sync, const render::CommandBuffer& buffer )
    {
      static const std::vector<::vk::PipelineStageFlags> flags( 100, ::vk::PipelineStageFlagBits::eColorAttachmentOutput ) ;

      const ::vk::Queue              queue          = data().context.graphicsQueue( data().gpu            ) ;
      const unsigned                 current_image  = data().context.currentSwap  ( data().window.c_str() ) ;
      const ::vk::CommandBuffer      cmd            = buffer.buffer               ( current_image         ) ;
      const ::vk::Fence              fence          = sync.fence() ;
      ::vk::SubmitInfo  info  ;
      
      
      info.setWaitSemaphoreCount  ( sync.numWaitSems()   ) ;
      info.setPWaitSemaphores     ( sync.waitSems()      ) ;
      info.setSignalSemaphoreCount( sync.numSignalSems() ) ;
      info.setPSignalSemaphores   ( sync.signalSems()    ) ;
      info.setPWaitDstStageMask   (  flags.data()        ) ;
      

      info.setCommandBufferCount  ( 1    ) ;
      info.setPCommandBuffers     ( &cmd ) ;

      data().device.resetFences( 1, &fence ) ;
      queue.submit( 1, &info, sync.fence() ) ;
    }

    const ::kgl::vk::Image& RenderPass::image( unsigned id )
    {
      static const ::kgl::vk::Image dummy ;
      return id < data().images.size() ? data().images[ id ] : dummy ;
    }

    RenderPassData& RenderPass::data()
    {
      return *this->pass_data ;
    }

    const RenderPassData& RenderPass::data() const
    {
      return *this->pass_data ;
    }
  }
}