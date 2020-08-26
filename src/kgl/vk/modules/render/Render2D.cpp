#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_LEFT_HANDED
#include "Render2D.h"
#include "Bus.h"
#include "Signal.h"
#include <log/Log.h>
#include <profiling/Timer.h>
#include "vk/context/Buffer.h"
#include "vk/context/Window.h"
#include "vk/context/Swapchain.h"
#include <vk/context/Pipeline.h>
#include <vk/context/Uniform.h>
#include <vk/context/Uniform.h>
#include <vk/context/Descriptor.h>
#include <vk/context/RenderPass.h>
#include <vk/context/Synchronization.h>
#include <vk/context/Context.h>
#include <vk/context/Image.h>
#include <vk/context/CommandBuffer.h>
#include <containers/BufferedStack.h>
#include <containers/List.h>
#include <containers/Layered.h>
#include <managers/AssetManager.h>
#include <vulkan/vulkan.hpp>
#include <cmd/DrawCommand.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtx/transform.hpp>
#include <string>
#include <stack>
#include <sstream>
#include <iostream>
#include <map>
#include <thread>

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
      struct Material
      {
        Uniform       uniform ;
        DescriptorSet set     ;
      };
      
      static constexpr unsigned BUFFERS = 3 ; // Todo make this 
      typedef std::map<std::string, Material>                 Materials  ;
      typedef ::kgl::BufferedStack<ImageCommand, BUFFERS>     Stack      ;
      typedef std::array<::kgl::vk::render::CommandBuffer, 2> CmdBuffers ;
      typedef kgl::containers::Layered<Synchronization, 3>    Syncs      ;
      
      karma::ms::Timer                 profiler          ;
      Syncs                            syncs             ;
      Stack                            commands          ;
      CmdBuffers                       buffer            ; ///< The command buffer to use for all GPU calls.
      unsigned                         cmd_buff_index    ; ///< THe index of command buffer currently in use.
      Materials                        materials         ; ///< Objects to contain all data to be sent to a uniform.
      ::kgl::vk::render::Context       context           ; ///< The context to use for vulkan state information.
      ::kgl::vk::Window*               window            ; ///< The window this object is a child of.
      ::kgl::vk::render::Pipeline      pipeline          ; ///< The pipeline that is associated with this object's rendering.
      ::kgl::vk::RenderPass            pass              ; ///< The render pass that defines this object's rendering.
      ::kgl::vk::Buffer                vertices          ; ///< The buffer that contains the vertex data that this object uses to render.
      ::kgl::vk::DescriptorPool        pool              ; ///< The object that contains the metadata to make descriptor sets.
      ::kgl::man::AssetManager         manager           ; ///< Asset manager object for grabbing loaded images.
      ::data::module::Bus              bus               ; ///< The data bus to use for communication with the rest of the program.
      unsigned                         gpu               ; ///< The GPU to use for this object's operations.
      glm::mat4                        model_matrix      ; ///< The transformation matrix for this object to use in rendering.
      glm::mat4                        projection        ; ///< The projection matrix that converts pixel locations to NDC coordinates.
      glm::mat4                        view              ; ///< The projection matrix that converts pixel locations to NDC coordinates.
      ::vk::Device                     device            ; ///< The handle of vulkan device this object uses.
      std::string                      window_name       ; ///< The name of this object's window.
      std::string                      output_name       ; ///< The name of this object's synchronization output.
      std::string                      output_image_name ; ///< The name of this object's framebuffer output.
      std::string                      name              ; ///< The name of this module.
      ImageCommand                     current_cmd       ; ///< The current command that is being processed.
      bool                             debug             ; ///< Whether or not this module outputs debug information.
      bool                             found_input       ;
      unsigned                         resx              ;
      unsigned                         resy              ;
      std::mutex                       mat_mutex         ;
      
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
      
      /** Method to set the x-resolution of this module.
       * @param val The x-res of this module.
       */
      void setResolutionX( unsigned val ) ;
      
      /** Method to set the y-resolution of this module.
       * @param val The y-resolution of this module.
       */
      void setResolutionY( unsigned val ) ;

      /** Method to retrieve a camera to use for this object's rendering.
       */
      void setCamera( const ::kgl::Camera& camera ) ;

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
      
      /** Method to output this object's data.
       */
      void output( const Synchronization& sync ) ;

      /** Method to set the name of this object's synchronization output.
       */
      void setOutputName( const char* name ) ;
      
      /** Method to set whether or not this module is outputting debug information.
       */
      void setDebug( bool val ) ;
    };
    
    Render2DData::Render2DData()
    {
      this->materials.clear() ;
      this->cmd_buff_index = 0                ;
      this->debug          = false            ;
      this->found_input    = false            ;
      this->view           = glm::mat4( 1.f ) ;
      this->resx           = 0                ;
      this->resy           = 0                ;
    }
    
    void Render2DData::pop()
    {
      this->current_cmd = this->commands.pop() ;
    }
    
    void Render2DData::setDebug( bool val )
    {
      this->debug = val ; 
    }

    void Render2DData::setCommand( const ::kgl::ImageCommand& cmd )
    {
      this->mat_mutex.lock() ;
      auto iter = this->materials.find( cmd.image() ) ;
      
      this->commands.insert( cmd, this->commands.next() ) ;

      // If we don't have enough uniform objects for the amount of commands we have, scale up.
      if( iter == this->materials.end() )
      {
        if( this->manager.contains( cmd.image() ) )
        {
          auto mat = this->materials.emplace( cmd.image(), Material() ) ;

          mat.first->second.set = this->pool.makeDescriptorSet( this->pass.numBuffers()             ) ;
          mat.first->second.uniform.initialize( this->gpu                                           ) ;
          mat.first->second.uniform.addImage  ( "image"     , this->manager.image( cmd.image() )    ) ;
          mat.first->second.uniform.add       ( "projection", Uniform::Type::UBO , this->projection ) ;
          mat.first->second.set.set( mat.first->second.uniform ) ;
        }
      }
      this->mat_mutex.unlock() ;
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
    
    void Render2DData::setResolutionX( unsigned val )
    {
      this->resx = val ;
    }
    
    void Render2DData::setResolutionY( unsigned val )
    {
      this->resy = val ;
    }
    
    void Render2DData::setCamera( const ::kgl::Camera& camera )
    {
      const glm::vec3 pos   = glm::vec3( camera.posX()  , camera.posY()  , camera.posZ()   ) ;
      const glm::vec3 front = glm::vec3( camera.frontX(), camera.frontY(), camera.frontZ() ) ;
      const glm::vec3 up    = glm::vec3( camera.upX()   , camera.upY()   , camera.upZ()    ) ;
      
      this->view = glm::lookAt( pos, pos + front, up ) ;
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
    
    void Render2D::input( const ::kgl::vk::Synchronization& sync )
    {
      data().syncs.value().clear() ;
      data().syncs.value().addWait( sync.signalSem( this->id() ) ) ;
      this->semIncrement() ;
    }

    void Render2DData::output( const Synchronization& sync )
    {
      const unsigned index = this->context.currentSwap( this->window_name.c_str() ) ;
     
      if( this->debug ) karma::log::Log::output( this->name.c_str(), ":: Outputting data." ) ;
      this->bus( this->output_image_name.c_str() ).emit( this->pass.image( index ) ) ;
      this->bus( this->output_name      .c_str() ).emit( sync                      ) ;
    }

    void Render2D::setInputName( const char* name )
    {
      // We now know what input we're getting. Subscribe & add as a dependancy.
      if( !data().found_input )
      {
        data().bus( name ).attach( this, &Render2D::input ) ;
        
        data().found_input = true ;
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
      const unsigned     width    = data().context.width ( data().window_name.c_str() ) ; ///< Width of the screen.
      const unsigned     height   = data().context.height( data().window_name.c_str() ) ; ///< Height of the screen.
      static const char* path     = "/uwu/2dimage.uwu"                                  ; ///< Path to this object's shader in the local-directory.
      std::string pipeline_path ;
      
      pipeline_path = ::kgl::vk::basePath() ;
      pipeline_path = pipeline_path + path  ;

      data().pass       .reset() ;
      data().buffer[ 0 ].reset() ; 
      data().buffer[ 1 ].reset() ; 
      data().pipeline   .reset() ; 
              
      // Initialize vulkan objects.
      data().pass       .initialize( data().window_name.c_str(), data().gpu                                                 ) ;
      data().buffer[ 0 ].initialize( data().window_name.c_str(), data().gpu, data().pass.numBuffers(), BufferLevel::Primary ) ;
      data().buffer[ 1 ].initialize( data().window_name.c_str(), data().gpu, data().pass.numBuffers(), BufferLevel::Primary ) ;
      data().pipeline   .initialize( pipeline_path.c_str(), data().gpu, width, height, data().pass.pass()                   ) ;
      
            if( data().resx == 0 && data().resy == 0 )
      {
        data().projection = glm::ortho(  0.0f, (float)width, 0.0f, (float)height, -1.0f, 1.0f ) ;
      }
      else
      {
        data().projection = glm::ortho(  0.0f, (float)data().resx, 0.0f, (float)data().resy, -1.0f, 1.0f ) ;
      }
      
      for( auto &uni : data().materials ) uni.second.uniform.add( "projection", Uniform::Type::UBO , data().projection ) ;
    }

    void Render2D::initialize()
    {
      const unsigned     MAX_SETS = 5                                                   ; ///< The max number of descriptor sets allowed.
      const unsigned     width    = data().context.width ( data().window_name.c_str() ) ; ///< Width of the screen.
      const unsigned     height   = data().context.height( data().window_name.c_str() ) ; ///< Height of the screen.
      static const char* path     = "/uwu/2dimage.uwu"                                  ; ///< Path to this object's shader in the local-directory.
      std::string pipeline_path ;
      
      this->setNumDependancies( 1 ) ;

      // Build path to this object's shdder.
      pipeline_path = ::kgl::vk::basePath() ;
      pipeline_path = pipeline_path + path  ;
      
      data().pipeline.setPushConstantByteSize ( sizeof( glm::mat4 ) * 2                                     ) ;
      data().pipeline.setPushConstantStageFlag( static_cast<unsigned>( ::vk::ShaderStageFlagBits::eVertex ) ) ;

      // Initialize vulkan objects.
      data().vertices       .initialize<float>( data().gpu, Buffer::Type::VERTEX, 24                                                   ) ;
      data().pass           .initialize       ( data().window_name.c_str(), data().gpu                                                 ) ;
      data().buffer[0]      .initialize       ( data().window_name.c_str(), data().gpu, data().pass.numBuffers(), BufferLevel::Primary ) ;
      data().buffer[1]      .initialize       ( data().window_name.c_str(), data().gpu, data().pass.numBuffers(), BufferLevel::Primary ) ;
      data().pipeline       .initialize       ( pipeline_path.c_str(), data().gpu, width, height, data().pass.pass()                   ) ;
      data().pool           .initialize       ( data().gpu, MAX_SETS, data().pipeline.shader()                                         ) ;
      data().syncs.seek( 0 ).initialize       ( data().gpu                                                                             ) ;
      data().syncs.seek( 1 ).initialize       ( data().gpu                                                                             ) ;
      data().syncs.seek( 2 ).initialize       ( data().gpu                                                                             ) ;
      
      // Initialize data.
      data().profiler.initialize() ;

      data().window = &data().context.window( data().window_name.c_str() ) ;
      
      if( data().resx == 0 && data().resy == 0 )
      {
        data().projection = glm::ortho(  0.0f, (float)width, 0.0f, (float)height, -1.0f, 1.0f ) ;
      }
      else
      {
        data().projection = glm::ortho(  0.0f, (float)data().resx, 0.0f, (float)data().resy, -1.0f, 1.0f ) ;
      }
      
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
      data().bus( "Graphs::", pipeline, "::window" ).attach( this->data_2d, &Render2DData::setWindowName ) ;
      data().bus( "Graphs::", pipeline, "::gpu"    ).attach( this->data_2d, &Render2DData::setGPU        ) ;
      
      // Module-specific JSON-parameters.
      data().bus( json_path.c_str(), "::input"        ).attach( this         , &Render2D::setInputName           ) ;
      data().bus( json_path.c_str(), "::sem_id"       ).attach( dynamic_cast<Module*>( this ), &Module::setId    ) ;
      data().bus( json_path.c_str(), "::output"       ).attach( this->data_2d, &Render2DData::setOutputName      ) ;
      data().bus( json_path.c_str(), "::output_image" ).attach( this->data_2d, &Render2DData::setOutputImageName ) ;
      data().bus( json_path.c_str(), "::debug"        ).attach( this->data_2d, &Render2DData::setDebug           ) ;
      data().bus( json_path.c_str(), "::resx"         ).attach( this->data_2d, &Render2DData::setResolutionX     ) ;
      data().bus( json_path.c_str(), "::resy"         ).attach( this->data_2d, &Render2DData::setResolutionY     ) ;

      // Module-specific inputs.
      data().bus( this->name(), "::cmd"    ).attach( this->data_2d, &Render2DData::setCommand ) ;
      data().bus( this->name(), "::camera" ).attach( this->data_2d, &Render2DData::setCamera  ) ;

      // Set our own Render Pass information, but allow it to be overwritten by JSON configuration.
      data().bus( json_path.c_str(), "::ViewportX"        ).emit( 0, 0.f ) ;
      data().bus( json_path.c_str(), "::ViewportY"        ).emit( 0, 0.f ) ;
      data().bus( json_path.c_str(), "::ViewportMinDepth" ).emit( 0, 0.f ) ;
      data().bus( json_path.c_str(), "::ViewportMaxDepth" ).emit( 0, 1.f ) ;
    }

    void Render2D::execute()
    {
      struct Transformation
      {
        glm::mat4 model ;
        glm::mat4 proj  ;
      };
      
      Transformation  transform ;
      Synchronization sync      ;

      data().mat_mutex.lock() ;
      data().profiler.start() ;
      data().commands.swap() ;
      sync = data().syncs.value() ;
      data().syncs.swap() ;
      data().mat_mutex.unlock() ;
      
      if( !data().commands.empty() )
      {
        data().buffer[ data().cmd_buff_index ].record( data().pass ) ;
        while( !data().commands.empty() )
        {
          // Pop latest draw command off the stack.
          data().pop() ;
          
          // Build Transformation Matrix.
          data().setUpModelMatrix() ;
          
          data().mat_mutex.lock() ;
          auto iter = data().materials.find( data().current_cmd.image() ) ;
          
          if( iter != data().materials.end() )
          {
            if( data().debug ) karma::log::Log::output( this->name(), ":: Binding command buffer & descriptor to pipeline." ) ;
            // Bind pipeline and descriptor set to the command buffer.
            data().pipeline.bind( data().buffer[ data().cmd_buff_index ], iter->second.set ) ;
            
            // Push Transformations to the GPU.
            transform.model = data().model_matrix ;
            transform.proj  = data().view         ;
          }
          data().mat_mutex.unlock() ;
          
          if( data().debug ) karma::log::Log::output( this->name(), ":: Pushing push constant for transformation to pipeline." ) ;
          data().buffer[ data().cmd_buff_index ].pushConstant( transform, data().pipeline.layout(), static_cast<unsigned>( ::vk::ShaderStageFlagBits::eVertex ), 1 ) ;

          // Draw using vertices.
          if( data().debug ) karma::log::Log::output( this->name(), ":: Drawing." ) ;
          data().buffer[ data().cmd_buff_index ].draw( data().vertices.buffer(), 24 ) ;
        }
        
        // Stop recording the command buffer & Submit to the graphics queue.
        data().buffer[ data().cmd_buff_index ].stop() ;
        
        if( data().debug ) karma::log::Log::output( this->name(), ":: Submitting command buffer." ) ;
        data().pass.submit( sync, data().buffer[ data().cmd_buff_index ] ) ;
      }
      else
      {
        sync.flip() ;
      }
      // Output our synchronization to next module in the graph & reset command index.
      data().output( sync ) ;
      if( data().cmd_buff_index == 1 ) data().cmd_buff_index-- ;
      else                             data().cmd_buff_index++ ;
      data().profiler.stop() ;

      if( data().debug ) karma::log::Log::output( this->name(), " CPU Time: ", data().profiler.output(), "ms" ) ;
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

