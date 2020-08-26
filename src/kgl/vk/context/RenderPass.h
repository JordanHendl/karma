/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   RenderPass.h
 * Author: jhendl
 *
 * Created on July 9, 2020, 10:15 PM
 */

#ifndef KGL_VK_RENDERPASS_H
#define KGL_VK_RENDERPASS_H

namespace vk
{
  class RenderPass ;
  class Framebuffer ;
  class Sampler ;
  class Image ;
  class ImageView ;
  class Rect2D ;
  enum class Format ;
  enum class ImageLayout ;
}

namespace kgl
{
  namespace vk
  {
    namespace render
    {
      class CommandBuffer ;
    }
    
    class Image           ;
    class Synchronization ;
    class SwapChain       ;

    class RenderPass
    {
      public:
        RenderPass() ;
        ~RenderPass() ;
        void initialize( const char* window_name,unsigned gpu, bool build_framebuffers = true ) ;
        void subscribe( const char* name, unsigned id ) ;
        void begin() ;
        void end() ;
        void setFormat( const ::vk::Format& format ) ;
        void setImageFinalLayout( const ::vk::ImageLayout& layout ) ;
        const ::vk::RenderPass pass() ;
        unsigned numBuffers() const ;
        const ::vk::Rect2D area() const ;
        const ::vk::Framebuffer buffer( unsigned id ) ;
        const ::kgl::vk::Image& image( unsigned id ) ;
        void submit( Synchronization& sync, const render::CommandBuffer& buffer ) ;
        void reset() ;
      private:
        
        friend class SwapChain ;
        void setFramebuffers( ::vk::Framebuffer* buffers, unsigned count ) ;
        void setArea( unsigned offx, unsigned offy, unsigned width, unsigned height ) ;
        
        struct RenderPassData *pass_data ;
        RenderPassData& data() ;
        const RenderPassData& data() const ;
    };
  }
}

#endif /* RENDERPASS_H */

