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

    class RenderPass
    {
      public:
        RenderPass() ;
        ~RenderPass() ;
        void initialize( const char* window_name,unsigned gpu ) ;
        void subscribe( const char* name, unsigned id ) ;
        void begin() ;
        void end() ;
        const ::vk::RenderPass pass() ;
        unsigned numBuffers() const ;
        const ::vk::Rect2D area() const ;
        const ::vk::Framebuffer buffer( unsigned id ) ;
        const ::kgl::vk::Image& image( unsigned id ) ;
        void submit( const Synchronization& sync, const render::CommandBuffer& buffer ) ;
      private:
        struct RenderPassData *pass_data ;
        RenderPassData& data() ;
        const RenderPassData& data() const ;
    };
  }
}

#endif /* RENDERPASS_H */

