#include "Render2D.h"
#include "Bus.h"
#include <vk/node/Synchronization.h>
#include <vk/context/RenderPass.h>
#include <vk/context/Context.h>
#include <vk/context/CommandBuffer.h>
#include <vulkan/vulkan.hpp>
#include <string>

namespace kgl
{
  namespace vk
  {
    struct Render2DData
    {
      ::kgl::vk::render::CommandBuffer buffer      ; ///< TODO
      ::kgl::vk::render::Context       context     ; ///< TODO
      ::kgl::vk::Synchronization       sync        ; ///< TODO
      ::kgl::vk::RenderPass            pass        ; ///< TODO
      ::data::module::Bus              bus         ; ///< TODO
      std::string                      window_name ; ///< TODO
      
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
       */
      void generateRenderPass() ;
      
      /**
       * @param name
       */
      void setVertexInput( const char* name ) ;
      
      /**
       */
      void setOutput( const char* name ) ;
    };

    Render2D::Render2D()
    {
      this->data_2d = new Render2DData() ;
    }
    
    Render2D::~Render2D()
    {
      delete this->data_2d ;
    }

    void Render2D::add( const gpu::Command& command )
    {
      
    }

    void Render2D::initialize()
    {
      data().pass.initialize( data().window_name.c_str() ) ;
    }

    void Render2D::shutdown()
    {
    
    }
    
    void Render2D::clear()
    {
      
    }

    void Render2D::subscribe( unsigned id )
    {
      data().pass.subscribe( Module::name(), id ) ;
    }

    void Render2D::execute()
    {
      
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

