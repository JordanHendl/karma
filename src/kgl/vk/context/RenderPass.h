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
}

namespace kgl
{
  namespace vk
  {
    class CommandBuffer ;
    
    class RenderPass
    {
      public:
        RenderPass() ;
        ~RenderPass() ;
        void initialize( const char* window_name ) ;
        void subscribe( const char* name, unsigned id ) ;
        const ::vk::RenderPass pass() ;
        unsigned numBuffers() const ;
        const ::vk::Framebuffer buffer( unsigned id ) ;
        void submit( const CommandBuffer& buffer ) ;
      private:
        struct RenderPassData *pass_data ;
        RenderPassData& data() ;
        const RenderPassData& data() const ;
    };
  }
}

#endif /* RENDERPASS_H */

