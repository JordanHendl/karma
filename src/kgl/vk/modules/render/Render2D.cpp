#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_LEFT_HANDED
#include "Render2D.h"
#include "Bus.h"
#include "Signal.h"
#include <vk/node/Synchronization.h>
#include "vk/context/Buffer.h"
#include "vk/context/Window.h"
#include "vk/context/Swapchain.h"
#include <vk/context/Pipeline.h>
#include <vk/context/Uniform.h>
#include <vk/context/Uniform.h>
#include <vk/context/Descriptor.h>
#include <vk/context/RenderPass.h>
#include <vk/context/Context.h>
#include <vk/context/Image.h>
#include <vk/context/CommandBuffer.h>
#include <managers/AssetManager.h>
#include <vulkan/vulkan.hpp>
#include <DrawCommand.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtx/transform.hpp>
#include <string>
#include <stack>

#include <sstream>
#include <iostream>

static inline const float vertex_data[] = 
{ 
  // pos      // tex
  0.0f, 1.0f, 0.0f, 1.0f,
  1.0f, 0.0f, 1.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 

  0.0f, 1.0f, 0.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 0.0f, 1.0f, 0.0f
};

namespace kgl
{
  namespace vk
  {      
    struct Render2DData
    {
      typedef std::stack<ImageCommand>        Commands ;
      typedef std::vector<::kgl::vk::Uniform> Uniforms ;

      ::kgl::vk::render::Context       context           ; ///< The context to use for vulkan state information.
       Uniforms                        uniforms          ; ///< Objects to contain all data to be sent to a uniform.
      ::kgl::vk::Window*               window            ; ///< The window this object is a child of.
      ::kgl::vk::render::CommandBuffer buffer            ; ///< The command buffer to use for all GPU calls.
      ::kgl::vk::render::Pipeline      pipeline          ; ///< The pipeline that is associated with this object's rendering.
      ::kgl::vk::RenderPass            pass              ; ///< The render pass that defines this object's rendering.
      ::kgl::vk::Buffer                vertices          ; ///< The buffer that contains the vertex data that this object uses to render.
      ::kgl::vk::DescriptorPool        pool              ; ///< The object that contains the metadata to make descriptor sets.
      ::kgl::vk::Synchronization       sync              ; ///< Synchronization object for managing GPU-GPU & GPU-CPU Synchronization.
      ::kgl::man::AssetManager         manager           ; ///< Asset manager object for grabbing loaded images.
      ::data::module::Bus              bus               ; ///< The data bus to use for communication with the rest of the program.
      unsigned                         gpu               ; ///< The GPU to use for this object's operations.
      glm::mat4                        model_matrix      ; ///< The transformation matrix for this object to use in rendering.
      glm::mat4                        projection        ; ///< The projection matrix that converts pixel locations to NDC coordinates.
      ::vk::Device                     device            ; ///< The handle of vulkan device this object uses.
      std::string                      window_name       ; ///< The name of this object's window.
      std::string                      output_name       ; ///< The name of this object's synchronization output.
      std::string                      output_image_name ; ///< The name of this object's framebuffer output.
      std::string                      name              ; ///< The name of this module.
      ImageCommand                     current_cmd       ; ///< The current command that is being processed.
      Commands                         commands          ; ///< The stack of commands this object is to process when kicked.
      unsigned                         current_cmd_index ; ///< The current index of command being processed.

      /** Default Constructor. Initializes member data.
       */
      Render2DData() ;
      
      /** Method to set this object's window's name.
       * @param name The name of window this object uses.
       */
      void setWindowName( const char* name ) ;
      
      /** Method to pop a command off the stack for processing.
       */
      void pop() ;

      /** Method to set the GPU this object uses for operations.
       */
      void setGPU( unsigned gpu ) ;

      /** Method to set this object's output framebuffer name.
       * @param output The name of this object's framebuffer output.
       */
      void setOutputImageName( const char* output ) ;
      
      /** Method to push a command to this object for operation.
      */
      void setCommand( const ::kgl::ImageCommand& cmd ) ;
      
      /** Method to prepare the transformation matrix of this object for rendering.
       */
      void setUpModelMatrix() ;

      /** Method to recieve a synchronization object for rendering synchronization.
       * @param sync The object to wait on for operations.
       */
      void input ( const ::kgl::vk::Synchronization& sync ) ;
      
      /** Method to output this object's data.
       */
      void output() ;

      /** Method to set the name of this object's synchronization input.
       */
      void setInputName( const char* name ) ;
      
      /** Method to set the name of this object's synchronization output.
       */
      void setOutputName( const char* name ) ;
    };
    
    Render2DData::Render2DData()
    {
      this->current_cmd_index = 0 ;
    }
    
    void Render2DData::pop()
    {
      this->current_cmd = this->commands.top() ;
      this->commands.pop() ;
    }

    void Render2DData::setCommand( const ::kgl::ImageCommand& cmd )
    {
        this->commands.emplace( cmd ) ;
        
        // If we don't have enough uniform objects for the amount of commands we have, scale up.
        if( this->commands.size() > this->uniforms.size() )
        {
          unsigned start = this->uniforms.size() != 0 ? this->uniforms.size() - 1 : 0 ;
          
          this->uniforms.resize( this->commands.size() ) ;
          for( unsigned i = start; i < this->commands.size(); i++ )
          {
            this->uniforms[ i ].initialize( this->gpu ) ;
          }
        }
    }
    
    void Render2DData::setUpModelMatrix()
    {
      const float       x   = this->current_cmd.posX()     ;
      const float       y   = this->current_cmd.posY()     ;
      const float       w   = this->current_cmd.width()    ;
      const float       h   = this->current_cmd.height()   ;
      const float       r   = this->current_cmd.rotation() ;
      const std::string img = this->current_cmd.image()    ;

      glm::vec2 size ;
     
      if( w < 0.1f && h < 0.1f && this->manager.contains( img.c_str() ) )
      {
        const auto image = &this->manager.image( this->current_cmd.image() ) ;
        size = glm::vec2( image->width(), image->height() ) ;
      }
      else
      {
        size = glm::vec2( w, h ) ;
      }

      this->model_matrix = glm::mat4( 1.f ) ;
      
      glm::vec3 pos( x, y, 0.0f ) ;
      this->model_matrix = glm::translate( this->model_matrix, pos ) ;
      
      this->model_matrix = glm::translate( this->model_matrix, glm::vec3   ( 0.5f  * w , 0.5f * h, 0.0f        ) ) ; 
      this->model_matrix = glm::rotate   ( this->model_matrix, glm::radians( r ), glm::vec3( 0.0f, 0.0f, 1.0f ) ) ; 
      this->model_matrix = glm::translate( this->model_matrix, glm::vec3   ( -0.5f * w, -0.5f * h, 0.0f       ) ) ;

      this->model_matrix = glm::scale( this->model_matrix, glm::vec3( size, 1.0f ) ) ; 
    }

    void Render2DData::setOutputImageName( const char* output ) 
    {
      this->output_image_name = output ;
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
    
    void Render2DData::input( const ::kgl::vk::Synchronization& sync )
    {
      this->sync.copy( sync ) ;
      this->pool.reset() ;
    }

    void Render2DData::output()
    {
      const unsigned index = this->context.currentSwap( this->window_name.c_str() ) ;
     
      this->bus( this->output_name      .c_str() ).emit( this->sync                ) ;
      this->bus( this->output_image_name.c_str() ).emit( this->pass.image( index ) ) ;
    }

    void Render2DData::setInputName( const char* name )
    {
      static bool found_input = false ;

      // We now know what input we're getting. Subscribe & add as a dependancy.
      if( !found_input )
      {
        this->bus( name ).attach       ( this, &Render2DData::input                     ) ;
        this->bus( name ).addDependancy( this, &Render2DData::input, this->name.c_str() ) ;
        found_input = true ;
      }
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

    void Render2D::resize()
    {
      
      data().projection = glm::ortho(0.0f, (float)data().window->chain().width(), (float)data().window->chain().height(), 0.0f, -1.0f, 1.0f) ;
    }

    void Render2D::initialize()
    {
      const unsigned     MAX_SETS = 200                                                 ; ///< The max number of descriptor sets allowed.
      const unsigned     width    = data().context.width ( data().window_name.c_str() ) ; ///< Width of the screen.
      const unsigned     height   = data().context.height( data().window_name.c_str() ) ; ///< Height of the screen.
      static const char* path     = "/uwu/render2d.uwu"                                 ; ///< Path to this object's shader in the local-directory.
      std::string pipeline_path ;
      
      // Build path to this object's shdder.
      pipeline_path = ::kgl::vk::basePath() ;
      pipeline_path = pipeline_path + path  ;
      
      // Initialize vulkan objects.
      data().vertices.initialize<float>( data().gpu, Buffer::Type::VERTEX, 24                                                   ) ;
      data().pass    .initialize       ( data().window_name.c_str(), data().gpu                                                 ) ;
      data().buffer  .initialize       ( data().window_name.c_str(), data().gpu, data().pass.numBuffers(), BufferLevel::Primary ) ;
      data().pipeline.initialize       ( pipeline_path.c_str(), data().gpu, width, height, data().pass.pass()                   ) ;
      data().pool    .initialize       ( data().gpu, MAX_SETS, data().pipeline.shader()                                         ) ;
      data().sync    .initialize       ( data().gpu                                                                             ) ;
      
      // Initialize data.
      data().window     = &data().context.window( data().window_name.c_str() )                ;
      data().projection = glm::ortho(  0.0f, (float)width, 0.0f, (float)height, -1.0f, 1.0f ) ;
      
      // Copy vertex data to the device.
      data().vertices.copyToDevice( vertex_data ) ;
    }

    void Render2D::shutdown()
    {
//      data().sync    .reset() ; ///TODO
//      data().uniform .reset() ; ///TODO
      data().vertices.reset() ; 
//      data().pass    .reset() ; ///TODO
//      data().buffer  .reset() ; ///TODO
//      data().pipeline.reset() ; ///TODO
    }

    void Render2D::subscribe( const char* pipeline, unsigned id )
    {
      const std::string json_path = std::string( "Graphs::" ) + std::string( pipeline ) + std::string( "::Modules::" ) + std::string( this->name() ) ;
      data().name = this->name() ;
      data().pass.subscribe( this->name(), id ) ;
      
      data().pipeline.subscribe( json_path.c_str(), id ) ;
      
      // Graph-specific JSON-parameters.
      data().bus( "Graphs::", pipeline, "::window"    ).attach( this->data_2d, &Render2DData::setWindowName      ) ;
      data().bus( "Graphs::", pipeline, "::gpu"       ).attach( this->data_2d, &Render2DData::setGPU             ) ;
      
      // Module-specific JSON-parameters.
      data().bus( json_path.c_str(), "::input"        ).attach( this->data_2d, &Render2DData::setInputName       ) ;
      data().bus( json_path.c_str(), "::output"       ).attach( this->data_2d, &Render2DData::setOutputName      ) ;
      data().bus( json_path.c_str(), "::output_image" ).attach( this->data_2d, &Render2DData::setOutputImageName ) ;

      // Graph-specific inputs.
      data().bus( pipeline, "::cmd" ).attach( this->data_2d, &Render2DData::setCommand ) ;

      // Add dependancies for this module's operation.
      data().bus( "" ).onCompletion( this->name(), dynamic_cast<Module*>( this ), &Render2D::kick ) ;
      data().bus( pipeline, "::cmd" ).addDependancy( this->data_2d, &Render2DData::setCommand, data().name.c_str() ) ;
      
      // Set our own Render Pass information, but allow it to be overwritten by JSON configuration.
      data().bus( json_path.c_str(), "::ViewportX"        ).emit( 0, 0.f ) ;
      data().bus( json_path.c_str(), "::ViewportY"        ).emit( 0, 0.f ) ;
      data().bus( json_path.c_str(), "::ViewportMinDepth" ).emit( 0, 0.f ) ;
      data().bus( json_path.c_str(), "::ViewportMaxDepth" ).emit( 0, 1.f ) ;
    }

    void Render2D::execute()
    {
      ::kgl::vk::DescriptorSet set ;
      if( !data().commands.empty() )
      {
        data().buffer.record( data().pass ) ;
        while( !data().commands.empty() )
        {
          // Make new descriptor set.
          set = data().pool.makeDescriptorSet( data().pass.numBuffers() ) ;
          
          // Pop latest draw command off the stack.
          data().pop() ;
          
          // Build Transformation Matrix.
          data().setUpModelMatrix() ;
          
          // Set Uniform Variable Data on shaders.
          data().uniforms[ data().current_cmd_index ].add( "model"     , Uniform::Type::UBO, data().model_matrix ) ;
          data().uniforms[ data().current_cmd_index ].add( "projection", Uniform::Type::UBO, data().projection   ) ;
    
          // If input image is loaded, put it on the shader as well.
          if( data().manager.contains( data().current_cmd.image() ) )
          {
             data().uniforms[ data().current_cmd_index ].addImage( "image", data().manager.image( data().current_cmd.image() ) ) ;
          } 

          // Pass uniform object to the Descriptor Set to be set.
          set.set(  data().uniforms[ data().current_cmd_index ] ) ;
          
          // Bind pipeline and descriptor set to the command buffer.
          data().pipeline.bind( data().buffer, set ) ;
    
          // Draw using vertices.
          data().buffer.draw( data().vertices.buffer(), 24 ) ;
          
          // Increment the index of the current command being processed.
          data().current_cmd_index++ ;
        }
        
        // Stop recording the command buffer & Submit to the graphics queue.
        data().buffer.stop() ;
        data().pass.submit( data().sync, data().buffer ) ;
        
        // Output our synchronization to next module in the graph & reset command index.
        data().output() ;
        data().current_cmd_index = 0 ;
      }
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

