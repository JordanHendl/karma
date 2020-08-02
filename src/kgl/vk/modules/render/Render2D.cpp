#include "Render2D.h"
#include "Bus.h"
#include "Signal.h"
#include <vk/node/Synchronization.h>
#include "vk/context/Buffer.h"
#include <vk/context/Pipeline.h>
#include <vk/context/Uniform.h>
#include <vk/context/RenderPass.h>
#include <vk/context/Context.h>
#include <vk/context/Image.h>
#include <vk/context/CommandBuffer.h>
#include <vulkan/vulkan.hpp>
#include <string>

#include <sstream>
#include <iostream>

namespace kgl
{
  namespace vk
  {
    struct Render2DData
    {
      static inline const float vertex_data[] = 
      { 
        // positions     // texture coords
       -0.5f, -0.5f,  1.0f, 1.0f, // top right
        0.5f, -0.5f,  1.0f, 0.0f, // bottom right
        0.5f,  0.5f,  0.0f, 0.0f, // bottom left
       -0.5f,  0.5f,  0.0f, 1.0f  // top left
      };

      static inline const unsigned int index_data[] = 
      {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
      };

      ::kgl::vk::render::CommandBuffer buffer       ; ///< TODO
      ::kgl::vk::render::Context       context      ; ///< TODO
      ::kgl::vk::Synchronization       sync         ; ///< TODO
      ::kgl::vk::render::Pipeline      pipeline     ; ///< TODO
      ::kgl::vk::Uniform               uniform      ; ///< TODO
      ::kgl::vk::RenderPass            pass         ; ///< TODO
      ::kgl::vk::Buffer                vertices     ; ///< TODO
      ::kgl::vk::Buffer                indices      ; ///< TODO
      ::data::module::Bus              bus          ; ///< TODO
      unsigned                         gpu          ; ///< TODO
      ::vk::Device                     device       ; ///< TODO
      std::string                      window_name  ; ///< TODO
      std::string                      output_name  ;
      std::string                      install_path ; ///< TODO
      std::string                      image        ; ///< TODO
      std::string                      name         ;
      float                            xpos         ; ///< TODO
      float                            ypos         ; ///< TODO

      /**
       */
      Render2DData() ;
      
      /**
       * @param name
       */
      void setWindowName( const char* name ) ;
      
      /**
       */
      void setGPU( unsigned gpu ) ;
      
      /**
       * @param image
       */
      void setImageName( const char* image ) ;
      
      /**
       * @param x
       */
      void setXPos( float x ) ;
      
      /**
       * 
       */
      void setYPos( float y ) ;
      
      /**
       * @param sync
       */
      void input ( const ::kgl::vk::Synchronization sync ) ;
      
      /**
       * @param name
       */
      void output() ;

      /**
       */
      void setInputName( const char* name ) ;
      
      /**
       */
      void setOutputName( const char* name ) ;
    };
    
    Render2DData::Render2DData()
    {
    
    }

    void Render2DData::setWindowName( const char* name )
    {
      this->window_name = name ;
    }

    void Render2DData::setGPU( unsigned gpu )
    {
      this->gpu    = gpu                                ;
      this->device = this->context.virtualDevice( gpu ) ;
    }

    void Render2DData::setImageName( const char* image )
    {
      this->image = image ;
    }

    void Render2DData::setXPos( float x )
    {
      this->xpos = x ;
    }

    void Render2DData::setYPos( float y )
    {
      this->ypos = y ;
    }

    void Render2DData::input( const ::kgl::vk::Synchronization sync )
    {
      this->sync.swap( sync ) ;
    }

    void Render2DData::output()
    {
      this->bus( this->output_name.c_str() ).emit( this->sync ) ;
    }

    void Render2DData::setInputName( const char* name )
    {
      // We now know what input we're getting. Subscribe & add as a dependancy.
      this->bus( name ).attach       ( this, &Render2DData::input                     ) ;
//      this->bus( name ).addDependancy( this, &Render2DData::input, this->name.c_str() ) ;
    }

    void Render2DData::setOutputName( const char* name )
    {
      this->output_name = name ;
    }

    Render2D::Render2D()
    {
      this->data_2d = new Render2DData() ;
    }
    
    Render2D::~Render2D()
    {
      delete this->data_2d ;
    }

    void Render2D::initialize()
    {
      static const char* path = "/uwu/render2d.uwu" ;
      std::string pipeline_path ;
      
      pipeline_path = data().install_path + path ;
      
      // Initialize vulkan objects.
      data().sync    .initialize( data().gpu                                                                             ) ;
      data().uniform .initialize( data().gpu                                                                             ) ;
      data().vertices.initialize<float   >( data().gpu, Buffer::Type::VERTEX, sizeof( data().vertex_data )               ) ;
      data().indices .initialize<unsigned>( data().gpu, Buffer::Type::INDEX , sizeof( data().index_data  )               ) ;
      data().pass    .initialize( data().window_name.c_str(), data().gpu                                                 ) ;
      data().buffer  .initialize( data().window_name.c_str(), data().gpu, data().pass.numBuffers(), BufferLevel::Primary ) ;
      data().pipeline.initialize( pipeline_path.c_str(), data().gpu, data().pass.numBuffers(), data().pass.pass()        ) ;
    }

    void Render2D::shutdown()
    {
//      data().sync    .reset() ; ///TODO
//      data().uniform .reset() ; ///TODO
//      data().vertices.reset() ; ///TODO
//      data().indices .reset() ; ///TODO
//      data().pass    .reset() ; ///TODO
//      data().buffer  .reset() ; ///TODO
//      data().pipeline.reset() ; ///TODO
    }

    void Render2D::subscribe( const char* pipeline, unsigned id )
    {
      const std::string json_path = std::string( "Graphs::" ) + std::string( pipeline ) + std::string( "::Modules::" ) + std::string( this->name() ) ;
      data().name = this->name() ;
      data().pass.subscribe( this->name(), id ) ;
      data().install_path = ::kgl::vk::basePath() ;
      
      data().pipeline.subscribe( json_path.c_str(), id ) ;

      data().bus( json_path.c_str(), "::input"     ).attach( this->data_2d, &Render2DData::setInputName  ) ;
      data().bus( json_path.c_str(), "::output"    ).attach( this->data_2d, &Render2DData::setOutputName ) ;
      data().bus( "Graphs::", pipeline, "::window" ).attach( this->data_2d, &Render2DData::setWindowName ) ;
      data().bus( "Graphs::", pipeline, "::gpu"    ).attach( this->data_2d, &Render2DData::setGPU        ) ;
      data().bus( pipeline, "::image"              ).attach( this->data_2d, &Render2DData::setImageName  ) ;
      data().bus( pipeline, "::xpos"               ).attach( this->data_2d, &Render2DData::setXPos       ) ;
      data().bus( pipeline, "::ypos"               ).attach( this->data_2d, &Render2DData::setYPos       ) ;
      
      // Add dependancies for this module's operation.
      data().bus( "" ).onCompletion( this->name(), dynamic_cast<Module*>( this ), &Render2D::kick ) ;
      data().bus( pipeline, "::image"  ).addDependancy( this->data_2d, &Render2DData::setImageName, this->name() ) ;
      data().bus( pipeline, "::xpos"   ).addDependancy( this->data_2d, &Render2DData::setXPos     , this->name() ) ;
      data().bus( pipeline, "::ypos"   ).addDependancy( this->data_2d, &Render2DData::setYPos     , this->name() ) ;
      
      data().bus( json_path.c_str(), "::ViewportX"        ).emit( 0, 0.f ) ;
      data().bus( json_path.c_str(), "::ViewportY"        ).emit( 0, 0.f ) ;
      data().bus( json_path.c_str(), "::ViewportMinDepth" ).emit( 0, 0.f ) ;
      data().bus( json_path.c_str(), "::ViewportMaxDepth" ).emit( 0, 1.f ) ;
    }

    void Render2D::execute()
    {
      data().buffer.record( data().pass ) ;

      // Set image & other uniforms.
      /*data().pipeline.bind( data().buffer  ) ;
      data().pipeline.set ( data().uniform ) ;
      
      //Draw with buffer.
      data().buffer.drawIndexed( sizeof( data().index_data ), data().indices.buffer(), data().vertices.buffer() ) ;*/
      data().buffer.stop() ;
      
      // Submit to render pass with synchronization data.
      //data().pass.submit( data().sync, data().buffer ) ;
      
      data().output() ;
    }

    Render2DData& Render2D::data()
    {
      return *this->data_2d ;
    }

    const Render2DData& Render2D::data() const
    {
      return *this->data_2d ;
    }
  }
}

