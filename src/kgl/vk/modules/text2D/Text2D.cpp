#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_LEFT_HANDED
#include "Text2D.h"
#include "Bus.h"
#include "Signal.h"
#include <log/Log.h>
#include <profiling/Timer.h>
#include "io/Font.h"
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
#include <mutex>
#include <sstream>
#include <iostream>
#include <map>



namespace kgl
{
  namespace vk
  {
    struct Transformation
    {
      glm::vec2 vert  ;
      glm::vec2 tex   ;
      glm::mat4 model ;
    };

    struct Text2DData
    {
      struct Material
      {
        Uniform       uniform ;
        DescriptorSet set     ;
      };

      static constexpr unsigned BUFFERS = 6 ; // Todo make this 
      typedef std::map<std::string, Material>                               Materials  ;
      typedef ::kgl::BufferedStack<TextCommand, BUFFERS>                    Stack      ;
      typedef kgl::containers::Layered<::kgl::vk::render::CommandBuffer, 3> CmdBuffers ;
      typedef kgl::containers::Layered<Synchronization                 , 3> Syncs      ;
      typedef std::vector<Transformation>                                   Transforms ;

      std::vector<float> vert = 
      { 
        // pos    
        0.0f, 1.0f, 0,
        1.0f, 0.0f, 1,
        0.0f, 0.0f, 2,
        0.0f, 1.0f, 0,
        1.0f, 1.0f, 3, 
        1.0f, 0.0f, 1
      };
      
      Transforms                       transforms        ;
      karma::ms::Timer                 profiler          ;
      Syncs                            syncs             ;
      Stack                            commands          ;
      CmdBuffers                       buffer            ; ///< The command buffer to use for all GPU calls.
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
      glm::mat4                        view              ; 
      ::vk::Device                     device            ; ///< The handle of vulkan device this object uses.
      std::string                      window_name       ; ///< The name of this object's window.
      std::string                      output_name       ; ///< The name of this object's synchronization output.
      std::string                      output_image_name ; ///< The name of this object's framebuffer output.
      std::string                      name              ; ///< The name of this module.
      TextCommand                      current_cmd       ; ///< The current command that is being processed.
      bool                             debug             ;
      std::mutex                       mutex             ;
      unsigned                         resx              ;
      unsigned                         resy              ;
      unsigned                         buffer_offset     ;
      bool                             found_input       ;
      
      /** Default Constructor. Initializes member data.
       */
      Text2DData() ;
      
      /** Method to set this object's window's name.
       * @param name The name of window this object uses.
       */
      void setWindowName( const char* name ) ;
      
      /** Method to pop a command off the stack for processing.
       */
      void pop() ;

      /** Method to set the x resolution of this object.
       * @param x The value to set this object's x resolution to.
       */
      void setResolutionX( unsigned x ) ;
      
      /** Method to set the y resolution of this object.
       * @param y The value to set this object's y resolution to.
       */
      void setResolutionY( unsigned y ) ;

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
      void setCommand( const ::kgl::TextCommand& cmd ) ;
      
      /** Method to set the texture coordinates of the vertices to match what the sprite being drawn.
       */
      unsigned setUpTextureCoords( const ::kgl::TextCommand& cmd ) ;

      /** Method to output this object's data.
       */
      void output( const Synchronization& sync ) ;

      /** Method to set the name of this object's synchronization output.
       */
      void setOutputName( const char* name ) ;
      
      /** Method to set whether or not this module should output debug information.
       */
      void setDebug( bool val ) ;
    };
    
    Text2DData::Text2DData()
    {
      this->resx           = 0     ;
      this->resy           = 0     ;
      this->debug          = false ;
      this->found_input    = false ;
      this->view           = glm::mat4( 1.f ) ;
    }
    
    void Text2DData::setDebug( bool val )
    {
      this->debug = val ;
    }

    void Text2DData::pop()
    {
      this->current_cmd = this->commands.pop() ;
    }

    void Text2DData::setResolutionX( unsigned x )
    {
      this->resx = x ;
    }
    
    void Text2DData::setResolutionY( unsigned y )
    {
      this->resy = y ;
    }
    
    void Text2DData::setCamera( const ::kgl::Camera& camera )
    {
      const glm::vec3 pos   = glm::vec3( camera.posX()  , camera.posY()  , camera.posZ()   ) ;
      const glm::vec3 front = glm::vec3( camera.frontX(), camera.frontY(), camera.frontZ() ) ;
      const glm::vec3 up    = glm::vec3( camera.upX()   , camera.upY()   , camera.upZ()    ) ;
      
        this->view = glm::lookAt( pos, pos + front, up ) ;
    }
    
    void Text2DData::setCommand( const ::kgl::TextCommand& cmd )
    {
      auto iter = this->materials.find( cmd.font() ) ;
      
      this->commands.insert( cmd, this->commands.next() ) ;

      // If we don't have enough uniform objects for the amount of commands we have, scale up.
      if( iter == this->materials.end() )
      {
        if( this->manager.contains( cmd.font() ) )
        {
          auto mat = this->materials.emplace( cmd.font(), Material() ) ;

          mat.first->second.set = this->pool.makeDescriptorSet( this->pass.numBuffers()            ) ;
          mat.first->second.uniform.initialize( this->gpu                                          ) ;
          mat.first->second.uniform.addImage  ( "image"     , this->manager.fontMap( cmd.font() )  ) ;
          mat.first->second.uniform.add       ( "projection", Uniform::Type::UBO, this->projection ) ;
          mat.first->second.uniform.add       ( "camera", Uniform::Type::UBO    , this->view       ) ;
          mat.first->second.set.set( mat.first->second.uniform ) ;
          this->device.waitIdle() ;
        }
      }
    }

    unsigned Text2DData::setUpTextureCoords( const ::kgl::TextCommand& cmd )
    {
      this->transforms.clear() ;
      
      const std::string text   = std::string( " " ) + cmd.text()                               ;
      const auto        ptr    = this->manager.font( cmd.font() ).glyphs( text.c_str(), 0, 0 ) ;
      const float w = cmd.width()    ;
      const float h = cmd.height()   ;
      const float r = cmd.rotation() ;

      glm::mat4                   model     ;
      std::vector<kgl::io::Glyph> glyphs    ;
      Transformation              transform ;

      glm::vec2 size ;
      size = glm::vec2( w, h ) ;

      glyphs.resize( text.size() ) ;
      glyphs.assign( ptr, ptr + text.size() ) ;
      for( auto glyph : glyphs )
      {
        model = glm::mat4( 1.f ) ;

        glm::vec3 pos( cmd.posX(), cmd.posY(), 0.0f ) ;
        model = glm::translate( model, pos ) ;

        model = glm::translate( model, glm::vec3   ( 0.5f  * w , 0.5f * h, 0.0f       ) ) ; 
        model = glm::rotate   ( model, glm::radians( r ), glm::vec3( 0.0f, 0.0f, 1.0f ) ) ; 
        model = glm::translate( model, glm::vec3   ( -0.5f * w, -0.5f * h, 0.0f       ) ) ;

        model = glm::scale( model, glm::vec3( size, 1.0f ) ) ; 
        transform.model = model ;

        transform.tex   = glm::vec2( glyph.topLeftX(), glyph.topLeftY() ) ;
        transform.vert  = glm::vec2( glyph.v0, glyph.v1                 ) ;
        this->transforms.push_back( transform ) ;

        transform.tex   = glm::vec2( glyph.bottomRightX(), glyph.bottomRightY() ) ;
        transform.vert  = glm::vec2( glyph.v2, glyph.v3                         ) ;
        this->transforms.push_back( transform ) ;

        transform.tex   = glm::vec2( glyph.bottomLeftX(), glyph.bottomLeftY() ) ;
        transform.vert  = glm::vec2( glyph.v0, glyph.v3                       ) ;
        this->transforms.push_back( transform ) ;

        transform.tex   = glm::vec2( glyph.topLeftX(), glyph.topLeftY() ) ;
        transform.vert  = glm::vec2( glyph.v0, glyph.v1                 ) ;
        this->transforms.push_back( transform ) ;

        transform.tex   = glm::vec2( glyph.topRightX(), glyph.topRightY() ) ;
        transform.vert  = glm::vec2( glyph.v2, glyph.v1                   ) ;
        this->transforms.push_back( transform ) ;

        transform.tex   = glm::vec2( glyph.bottomRightX(), glyph.bottomRightY() ) ;
        transform.vert  = glm::vec2( glyph.v2, glyph.v3                         ) ;
        this->transforms.push_back( transform ) ;
      }

      this->vertices.copyToDevice( this->transforms.data(), this->transforms.size(), this->buffer_offset ) ;
      
      this->buffer_offset += this->transforms.size() * sizeof( Transformation ) ;
      
      return this->transforms.size() ;
    }

    void Text2DData::setOutputImageName( const char* output ) 
    {
      this->output_image_name = output ;
    }

    void Text2DData::setWindowName( const char* name )
    {
      this->window_name = name ;
    }

    void Text2DData::setGPU( unsigned gpu )
    {
      this->gpu    = gpu                                ;
      this->device = this->context.virtualDevice( gpu ) ;
    }
    
    void Text2D::input( const ::kgl::vk::Synchronization& sync )
    {
      data().mutex.lock() ;
      data().syncs.value().clear() ;
      data().syncs.value().addWait( sync.signalSem( this->id() ) ) ;
      this->semIncrement() ;
      data().mutex.unlock() ;
    }

    void Text2DData::output( const Synchronization& sync )
    {
      const unsigned index = this->context.currentSwap( this->window_name.c_str() ) ;
     
      this->bus( this->output_image_name.c_str() ).emit( this->pass.image( index ) ) ;
      this->bus( this->output_name      .c_str() ).emit( sync                      ) ;
    }

    void Text2D::setInputName( const char* name )
    {
      // We now know what input we're getting. Subscribe & add as a dependancy.
      if( !data().found_input )
      {
        data().bus( name ).attach( this, &Text2D::input ) ;
        data().found_input = true ;
      }
    }

    void Text2DData::setOutputName( const char* name )
    {
      this->output_name = name ;
    }

    Text2D::Text2D()
    {
      this->data_2d = new Text2DData() ;
    }
    
    Text2D::~Text2D()
    {
      delete this->data_2d ;
    }

    void Text2D::resize()
    {
      const unsigned     width    = data().context.width ( data().window_name.c_str() ) ; ///< Width of the screen.
      const unsigned     height   = data().context.height( data().window_name.c_str() ) ; ///< Height of the screen.
      static const char* path     = "/uwu/text2d.uwu"                                   ; ///< Path to this object's shader in the local-directory.
      std::string pipeline_path ;
      
      pipeline_path = ::kgl::vk::basePath() ;
      pipeline_path = pipeline_path + path  ;
      
      data().pass            .reset() ;
      data().buffer.seek( 0 ).reset() ; 
      data().buffer.seek( 1 ).reset() ; 
      data().buffer.seek( 2 ).reset() ; 
      data().pipeline        .reset() ; 
              
      // Initialize vulkan objects.
      data().pass            .initialize( data().window_name.c_str(), data().gpu                                                 ) ;
      data().buffer.seek( 0 ).initialize( data().window_name.c_str(), data().gpu, data().pass.numBuffers(), BufferLevel::Primary ) ;
      data().buffer.seek( 1 ).initialize( data().window_name.c_str(), data().gpu, data().pass.numBuffers(), BufferLevel::Primary ) ;
      data().buffer.seek( 2 ).initialize( data().window_name.c_str(), data().gpu, data().pass.numBuffers(), BufferLevel::Primary ) ;
      data().pipeline        .initialize( pipeline_path.c_str(), data().gpu, width, height, data().pass.pass()                   ) ;
      
      if( data().resx == 0 && data().resy == 0 )
      {
        data().projection = glm::ortho(  0.0f, (float)width, 0.0f, (float)height, -100.0f, 100.0f ) ;
      }
      else
      {
        data().projection = glm::ortho(  0.0f, (float)data().resx, 0.0f, (float)data().resy, -100.0f, 100.0f ) ;
      }
      
      for( auto &uni : data().materials ) uni.second.uniform.add( "projection", Uniform::Type::UBO , data().projection ) ;
    }

    void Text2D::initialize()
    {
      const unsigned     MAX_SETS = 5                                                   ; ///< The max number of descriptor sets allowed.
      const unsigned     width    = data().context.width ( data().window_name.c_str() ) ; ///< Width of the screen.
      const unsigned     height   = data().context.height( data().window_name.c_str() ) ; ///< Height of the screen.
      static const char* path     = "/uwu/text2d.uwu"                                   ; ///< Path to this object's shader in the local-directory.
      std::string pipeline_path ;
      
      this->setNumDependancies( 1 ) ;

      // Build path to this object's shdder.
      pipeline_path = ::kgl::vk::basePath() ;
      pipeline_path = pipeline_path + path  ;
      
      data().pass.setImageFinalLayout( ::vk::ImageLayout::eGeneral ) ;
      data().pipeline.setPushConstantByteSize ( ( sizeof( glm::vec4 ) )                                     ) ;
      data().pipeline.setPushConstantStageFlag( static_cast<unsigned>( ::vk::ShaderStageFlagBits::eVertex ) ) ;

      // Initialize vulkan objects.
      data().vertices        .initialize<Transformation>( data().gpu, Buffer::Type::VERTEX, 10000, true                        ) ;
      data().pass            .initialize                ( data().window_name.c_str(), data().gpu                               ) ;
      data().buffer.seek( 0 ).initialize                ( data().window_name.c_str(), data().gpu, 3, BufferLevel::Primary      ) ;
      data().buffer.seek( 1 ).initialize                ( data().window_name.c_str(), data().gpu, 3, BufferLevel::Primary      ) ;
      data().buffer.seek( 2 ).initialize                ( data().window_name.c_str(), data().gpu, 3, BufferLevel::Primary      ) ;
      data().pipeline        .initialize                ( pipeline_path.c_str(), data().gpu, width, height, data().pass.pass() ) ;
      data().pool            .initialize                ( data().gpu, MAX_SETS, data().pipeline.shader()                       ) ;
      data().syncs.seek( 0 ) .initialize                ( data().gpu                                                           ) ;
      data().syncs.seek( 1 ) .initialize                ( data().gpu                                                           ) ;
      data().syncs.seek( 2 ) .initialize                ( data().gpu                                                           ) ;
      
      // Initialize data.
      data().profiler.initialize() ;

      data().window     = &data().context.window( data().window_name.c_str() ) ;

      if( data().resx == 0 && data().resy == 0 )
      {
        data().projection = glm::ortho(  0.0f, (float)width, 0.0f, (float)height, -100.0f, 100.0f ) ;
      }
      else
      {
        data().projection = glm::ortho(  0.0f, (float)data().resx, 0.0f, (float)data().resy, -100.0f, 100.0f ) ;
      }
    }

    void Text2D::shutdown()
    {
//      data().sync    .reset() ; ///TODO
//      data().uniform .reset() ; ///TODO
//      data().buffer  .reset() ; ///TODO
      data().vertices.reset() ; 
      data().pass    .reset() ; 
      data().pipeline.reset() ; 
    }

    void Text2D::subscribe( const char* pipeline, unsigned id )
    {
      const std::string json_path = std::string( "Graphs::" ) + std::string( pipeline ) + std::string( "::Modules::" ) + std::string( this->name() ) ;
      data().name = this->name() ;
      data().pass.subscribe( this->name(), id ) ;
      
      data().pipeline.subscribe( json_path.c_str(), id ) ;
      
      // Graph-specific JSON-parameters.
      data().bus( "Graphs::", pipeline, "::window" ).attach( this->data_2d, &Text2DData::setWindowName ) ;
      data().bus( "Graphs::", pipeline, "::gpu"    ).attach( this->data_2d, &Text2DData::setGPU        ) ;
      
      // Module-specific JSON-parameters.
      data().bus( json_path.c_str(), "::input"        ).attach( this         , &Text2D::setInputName           ) ;
      data().bus( json_path.c_str(), "::sem_id"       ).attach( dynamic_cast<Module*>( this ), &Module::setId  ) ;
      data().bus( json_path.c_str(), "::output"       ).attach( this->data_2d, &Text2DData::setOutputName      ) ;
      data().bus( json_path.c_str(), "::output_image" ).attach( this->data_2d, &Text2DData::setOutputImageName ) ;
      data().bus( json_path.c_str(), "::debug"        ).attach( this->data_2d, &Text2DData::setDebug           ) ;
      data().bus( json_path.c_str(), "::resx"         ).attach( this->data_2d, &Text2DData::setResolutionX     ) ;
      data().bus( json_path.c_str(), "::resy"         ).attach( this->data_2d, &Text2DData::setResolutionY     ) ;
      
      // Module-specific inputs.
      data().bus( this->name(), "::cmd"    ).attach( this->data_2d, &Text2DData::setCommand ) ;
      data().bus( this->name(), "::camera" ).attach( this->data_2d, &Text2DData::setCamera  ) ;

      // Set our own Render Pass information, but allow it to be overwritten by JSON configuration.
      data().bus( json_path.c_str(), "::ViewportX"        ).emit( 0, 0.f ) ;
      data().bus( json_path.c_str(), "::ViewportY"        ).emit( 0, 0.f ) ;
      data().bus( json_path.c_str(), "::ViewportMinDepth" ).emit( 0, 0.f ) ;
      data().bus( json_path.c_str(), "::ViewportMaxDepth" ).emit( 0, 1.f ) ;
    }

    void Text2D::execute()
    {
      Synchronization sync  ;
      glm::vec4       color ;
      unsigned        index ;
      unsigned        sz    ;
      
      data().profiler.start() ;
      data().commands.swap() ;
      data().mutex.lock() ;
      sync = data().syncs.value() ;
      data().syncs .swap() ;
      data().buffer.swap() ;
      data().mutex.unlock() ;
      
      if( !data().commands.empty() )
      {
        data().buffer.value().record() ;
        data().buffer.value().beginRenderPass( data().pass ) ;

        index                = 0 ;
        data().buffer_offset = 0 ;

        while( !data().commands.empty() )
        {
          // Pop latest draw command off the stack.
          data().pop() ;

          color.x = data().current_cmd.red()   ;
          color.y = data().current_cmd.green() ;
          color.z = data().current_cmd.blue()  ;
          color.w = data().current_cmd.alpha() ;

          // Build Transformation Matrix.
          sz = data().setUpTextureCoords( data().current_cmd ) ;
          
          auto iter = data().materials.find( data().current_cmd.font() ) ;
          
          // Bind pipeline and descriptor set to the command buffer.
          iter->second.uniform.add( "camera", Uniform::Type::UBO , data().view ) ;

          data().pipeline.bind( data().buffer.value(), iter->second.set ) ;
          data().buffer.value().pushConstant( color, data().pipeline.layout(), static_cast<unsigned>( ::vk::ShaderStageFlagBits::eVertex ), 1 ) ;

          // Draw using vertices.
          data().buffer.value().draw( data().vertices.buffer(), sz, data().buffer_offset - ( sz * sizeof( Transformation ) ) ) ;
          index++ ;
        }
          data().buffer.value().stop() ;
          data().pass.submit( sync, data().buffer.value(), data().buffer.current() ) ;
          data().buffer.swap() ;
        
        // Stop recording the command buffer & Submit to the graphics queue.
      }
      else
      {
        sync.flip() ;
      }
      
      // Output our synchronization to next module in the graph & reset command index.
      data().output( sync ) ;
      data().profiler.stop() ;

      if( data().debug ) karma::log::Log::output( this->name(), " CPU Time: ", data().profiler.output(), "ms" ) ;
    }

    Text2DData& Text2D::data()
    {
      return *this->data_2d ;
    }

    const Text2DData& Text2D::data() const
    {
      return *this->data_2d ;
    }
  }
}
/** Exported function to retrive the name of this module type.
 * @return The name of this object's type.
 */
exported_function const char* name()
{
  return "Text2D" ;
}

/** Exported function to retrieve the version of this module.
 * @return The version of this module.
 */
exported_function unsigned version()
{
  return 1 ;
}

/** Exported function to make one instance of this module.
 * @return A single instance of this module.
 */
exported_function ::kgl::vk::Module* make( unsigned version )
{
  return new ::kgl::vk::Text2D() ;
}

/** Exported function to destroy an instance of this module.
 * @param module A Pointer to a Module object that is of this type.
 */
exported_function void destroy( ::kgl::vk::Module* module )
{
  ::kgl::vk::Text2D* mod ;
  
  mod = dynamic_cast<::kgl::vk::Text2D*>( module ) ;
  delete mod ;
}

