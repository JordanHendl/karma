/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Present.cpp
 * Author: jhendl
 * 
 * Created on July 19, 2020, 8:37 PM
 */

#include "Present.h"
#include <vk/context/Synchronization.h>
#include "../../context/Context.h"
#include "../../context/Pipeline.h"
#include "../../context/CommandBuffer.h"
#include "../../context/Descriptor.h"
#include "../../context/Buffer.h"
#include "../../context/RenderPass.h"
#include <containers/BufferedStack.h>
#include <containers/Layered.h>
#include "vk/context/Window.h"
#include "vk/context/Swapchain.h"
#include <vk/context//Descriptor.h>
#include "vk/context/Uniform.h"
#include "vk/context/Image.h"
#include <log/Log.h>
#include <profiling/Timer.h>
#include <Bus.h>
#include <Signal.h>
#include <vulkan/vulkan.hpp>
#include <string>
#include <map>
#include <iostream>
#include <queue>

static inline const float vertex_data[] = 
{ 
  // positions     // texture coords
 -1.0f, -1.0f,  0.0f, 0.0f, // top right
  1.0f, -1.0f,  1.0f, 0.0f, // bottom right
  1.0f,  1.0f,  1.0f, 1.0f, // bottom left
 -1.0f,  1.0f,  0.0f, 1.0f  // top left
};


static inline const unsigned index_data[] = 
{
  0, 1, 3, // first triangle
  1, 2, 3  // second triangle
};

namespace kgl
{
  namespace vk
  {
    struct PresentData
    { 
      static constexpr unsigned BUFFERS = 3 ;
      typedef ::kgl::BufferedStack<::kgl::vk::Image, BUFFERS >              ImageStack ;
      typedef std::vector<::kgl::vk::Uniform >                              Uniforms   ;
      typedef kgl::containers::Layered<Synchronization, 3>                  Syncs      ;
      typedef kgl::containers::Layered<::kgl::vk::render::CommandBuffer, 3> CmdBuffs   ;
      
      std::queue<::kgl::vk::DescriptorSet> sets  ;
      std::queue<unsigned>                 swaps ;
      Syncs                            syncs        ;
      Uniforms                         uniforms     ; ///< TODO
      ImageStack                       stack        ; ///< TODO
      CmdBuffs                         buffers      ; ///< TODO
      karma::ms::Timer                 profiler     ; ///< TODO
      ::kgl::vk::DescriptorPool        pool         ; ///< TODO
      ::kgl::vk::render::Context       context      ; ///< TODO
      ::kgl::vk::render::Pipeline      pipeline     ; ///< TODO
      ::kgl::vk::Uniform               uniform      ; ///< TODO
      ::kgl::vk::RenderPass            pass         ; ///< TODO
      ::kgl::vk::Buffer                vertices     ; ///< TODO
      ::kgl::vk::Buffer                indices      ; ///< TODO
      ::data::module::Bus              bus          ; ///< TODO
      unsigned                         gpu          ; ///< TODO
      ::vk::Device                     device       ; ///< TODO
      std::string                      window_name  ; ///< TODO
      std::string                      output_name  ; ///< TODO
      std::string                      install_path ; ///< TODO
      std::string                      name         ;
      std::mutex                       mutex        ;
      
      unsigned current_command ;

      void setGPU( unsigned gpu ) ;
      void setWindowName( const char* window ) ;
      void setInputImageName( const char* name ) ; 
      void setImage( const ::kgl::vk::Image& image ) ;
      void setCurrentSwap( unsigned index ) ;
    };
    
    void PresentData::setCurrentSwap( unsigned index )
    {
      this->mutex.lock() ;
      this->swaps.push( index ) ; 
      this->mutex.unlock() ;
    }
    
    void Present::setInputName( const char* name )
    {
      static bool found_input = false ;

      // We now know what input we're getting. Subscribe & add as a dependancy.
      if( !found_input )
      {
        data().bus( name ).attach( this, &Present::input ) ;
        
        found_input = true ;
      }
    }

    void PresentData::setInputImageName( const char* name )
    {
      static bool found_input = false ;
      // We now know what input we're getting. Subscribe & add as a dependancy.
      if( !found_input )
      {
        this->bus( name ).attach( this, &PresentData::setImage ) ;
        found_input = true ;
      }
    }
    
    void PresentData::setGPU( unsigned gpu )
    {
      this->gpu    = gpu                                ;
      this->device = this->context.virtualDevice( gpu ) ;
    }

    void PresentData::setWindowName( const char* window )
    {
      this->window_name = window ;
    }

    void Present::input( const ::kgl::vk::Synchronization& sync )
    {
      data().syncs.value().copy( sync ) ;
      this->semIncrement() ;
    }

    void PresentData::setImage( const ::kgl::vk::Image& image )
    {
      this->stack.insert( image, this->stack.next() ) ;
      
      // If we don't have enough uniform objects for the amount of commands we have, scale up.
      if( this->stack.size( static_cast<int>( this->stack.next() ) ) > this->uniforms.size() )
      {
        unsigned start = this->uniforms.size() != 0 ? this->uniforms.size() - 1 : 0 ;
        
        this->uniforms.resize( this->stack.size( static_cast<int>( this->stack.next() ) ) ) ;

        for( unsigned index = start; index < this->stack.size(); index++ )
        {
          this->uniforms[ index ].initialize( this->gpu ) ;
        }
      }
    }

    Present::Present()
    {
      this->present_data = new PresentData() ;
    }

    Present::~Present()
    {
      delete this->present_data ;
    }

    void Present::initialize()
    {
      static const unsigned MAX_SETS = 200 ;
      const char*       path          = "/uwu/present.uwu"                                           ;
      const std::string pipeline_path = data().install_path + path                                   ;
      const unsigned    num_buffers   = data().context.numFrameBuffers( data().window_name.c_str() ) ;
      const unsigned     width        = data().context.width ( data().window_name.c_str() )          ;
      const unsigned     height       = data().context.height( data().window_name.c_str() )          ;
      
      this->setNumDependancies( 1 ) ;

      data().pass.setFormat( static_cast<::vk::Format>( data().context.window( data().window_name.c_str() ).chain().format() ) ) ;
      data().pass.setImageFinalLayout( ::vk::ImageLayout::ePresentSrcKHR ) ;
      
      data().profiler       .initialize( ""         ) ;
      data().syncs.seek( 0 ).initialize( data().gpu ) ;
      data().syncs.seek( 1 ).initialize( data().gpu ) ;
      data().syncs.seek( 2 ).initialize( data().gpu ) ;
      data().uniform        .initialize( data().gpu ) ;
      data().vertices       .initialize<float   >( data().gpu, Buffer::Type::VERTEX, 16  ) ;
      data().indices        .initialize<unsigned>( data().gpu, Buffer::Type::INDEX , 6   ) ;
      data().pass           .initialize( data().window_name.c_str(), data().gpu, false   ) ;

      data().context.window( data().window_name.c_str() ).chain().createFrameBuffers( data().pass ) ;

      data().buffers.seek( 0 ).initialize( data().window_name.c_str(), data().gpu, num_buffers, BufferLevel::Primary ) ;
      data().buffers.seek( 1 ).initialize( data().window_name.c_str(), data().gpu, num_buffers, BufferLevel::Primary ) ;
      data().buffers.seek( 2 ).initialize( data().window_name.c_str(), data().gpu, num_buffers, BufferLevel::Primary ) ;
      data().pipeline         .initialize( pipeline_path.c_str(), data().gpu, width, height, data().pass.pass()      ) ;
      data().pool             .initialize( data().gpu, MAX_SETS, data().pipeline.shader()                            ) ;

      data().vertices.copyToDevice( vertex_data ) ;
      data().indices .copyToDevice( index_data  ) ;
    }

    void Present::shutdown()
    {
    
    }
    
    void Present::resize()
    {
      const unsigned    num_buffers   = data().context.numFrameBuffers( data().window_name.c_str() ) ;
      const unsigned    width         = data().context.width ( data().window_name.c_str() ) ;
      const unsigned    height        = data().context.height( data().window_name.c_str() ) ;
      const char*       path          = "/uwu/present.uwu"                        ;
      const std::string pipeline_path = data().install_path + path                ;

      data().buffers.seek( 0 ).reset() ;
      data().buffers.seek( 1 ).reset() ;
      data().buffers.seek( 2 ).reset() ;
      data().pipeline         .reset() ;
      data().pass             .reset() ;
      
      data().pass             .initialize( data().window_name.c_str(), data().gpu, false   ) ;
      data().context.window( data().window_name.c_str() ).chain().createFrameBuffers( data().pass ) ;
      data().buffers.seek( 0 ).initialize( data().window_name.c_str(), data().gpu, num_buffers, BufferLevel::Primary ) ;
      data().buffers.seek( 1 ).initialize( data().window_name.c_str(), data().gpu, num_buffers, BufferLevel::Primary ) ;
      data().buffers.seek( 2 ).initialize( data().window_name.c_str(), data().gpu, num_buffers, BufferLevel::Primary ) ;
      data().pipeline         .initialize( pipeline_path.c_str(), data().gpu, width, height, data().pass.pass()      ) ;
    }

    void Present::subscribe( const char* pipeline, unsigned id )
    {
      const std::string json_path = std::string( "Graphs::" ) + std::string( pipeline ) + std::string( "::Modules::" ) + std::string( this->name() ) ;

      data().name         = this->name()          ;
      data().pass.subscribe( this->name(), id )   ;
      data().install_path = ::kgl::vk::basePath() ;
      
      data().pipeline.subscribe( json_path.c_str(), id ) ;

      data().bus( "current_swap" ).attach( this->present_data, &PresentData::setCurrentSwap ) ;
      data().bus( json_path.c_str(), "::input"       ).attach( this         , &Present::setInputName          ) ;
      data().bus( json_path.c_str(), "::input_image" ).attach( this->present_data, &PresentData::setInputImageName ) ;
      data().bus( json_path.c_str(), "::image"       ).attach( this->present_data, &PresentData::setImage          ) ;
      data().bus( "Graphs::", pipeline, "::window"   ).attach( this->present_data, &PresentData::setWindowName     ) ;
      data().bus( "Graphs::", pipeline, "::gpu"      ).attach( this->present_data, &PresentData::setGPU            ) ;
      
      data().bus( json_path.c_str(), "::ViewportX"        ).emit( 0, 0.f ) ;
      data().bus( json_path.c_str(), "::ViewportY"        ).emit( 0, 0.f ) ;
      data().bus( json_path.c_str(), "::ViewportMinDepth" ).emit( 0, 0.f ) ;
      data().bus( json_path.c_str(), "::ViewportMaxDepth" ).emit( 0, 1.f ) ;
    }

    void Present::execute()
    {
      ::kgl::vk::DescriptorSet set  ;
      Synchronization          sync ;
      
      sync = data().syncs.value() ;
      data().syncs .swap()        ;

      data().current_command = 0 ;

      data().stack   .swap()  ;
      data().profiler.start() ;
      
      // If we have no commands, flip the sync to skip this module's operation.
      if( data().stack.empty() )
      {
        sync.flip() ;
      }
      else
      {
        data().buffers.value().record( data().pass ) ;
        while( !data().stack.empty() )
        {
          auto img  = data().stack.pop()                                        ;
          set       = data().pool.makeDescriptorSet( data().pass.numBuffers() ) ;
          
          data().uniforms[ data().current_command ].addImage( "framebuffer", img ) ;
          set.set( data().uniforms[ data().current_command ] ) ;
          data().pipeline.bind( data().buffers.value(), set ) ;
          data().buffers.value().drawIndexed( 6, data().indices.buffer(), data().vertices.buffer() ) ;
          
          data().sets.push( set ) ;
          if( data().sets.size() > 2 )
          {
            data().sets.front().reset() ;
            data().sets.pop() ;
          }
        }
        data().buffers.value().stop() ;
        data().pass.submit( sync, data().buffers.value(), data().swaps.front() ) ;
        data().swaps.pop() ;
        data().buffers.swap() ;
      }

      
      data().profiler.stop() ;
      karma::log::Log::output( this->name(), " CPU Time: ", data().profiler.output(), "ms" ) ;
      data().bus( data().window_name.c_str(), "::present" ).emit( sync ) ;
    }

    PresentData& Present::data()
    {
      return *this->present_data ;
    }

    const PresentData& Present::data() const
    {
      return *this->present_data ;
    }
  }
}

/** Exported function to retrive the name of this module type.
 * @return The name of this object's type.
 */
exported_function const char* name()
{
  return "Present" ;
}

/** Exported function to retrieve the version of this module.
 * @return The version of this module.
 */
exported_function unsigned version()
{
  return VERSION ;
}

/** Exported function to make one instance of this module.
 * @return A single instance of this module.
 */
exported_function ::kgl::vk::Module* make()
{
  return new ::kgl::vk::Present() ;
}

/** Exported function to destroy an instance of this module.
 * @param module A Pointer to a Module object that is of this type.
 */
exported_function void destroy( ::kgl::vk::Module* module )
{
  ::kgl::vk::Present* mod ;
  
  mod = dynamic_cast<::kgl::vk::Present*>( module ) ;
  delete mod ;
}
