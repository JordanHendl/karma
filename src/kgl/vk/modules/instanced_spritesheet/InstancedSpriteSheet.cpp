#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_LEFT_HANDED
#include "InstancedSpriteSheet.h"
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
#include <vk/context/Shader.h>
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
      glm::mat4 model ;
      glm::vec2 bl    ;
      glm::vec2 tr    ;
      glm::vec2 tl    ;
      glm::vec2 br    ;
    };
    
    
    /** Struct to contain the InstancedSpriteSheet object data.
     */
    struct InstancedSpriteSheetData
    {
      struct Material
      {
        Uniform       uniform ;
        DescriptorSet set     ;
      };

      static constexpr unsigned BUFFERS = 3 ; // Todo make this configurable
      typedef std::map<std::string, Material>                             Materials    ;
      typedef kgl::containers::Layered<kgl::vk::render::CommandBuffer, 3> CmdBuffers   ;
      typedef kgl::containers::Layered<Synchronization               , 4> Syncs        ;
      typedef std::map<std::string, std::vector<Transformation>>          TransformMap ;
      
      karma::ms::Timer                 profiler          ;
      Syncs                            syncs             ;
      TransformMap                     transforms        ;
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
      ::vk::Device                     device            ; ///< The handle of vulkan device this object uses.
      std::string                      window_name       ; ///< The name of this object's window.
      std::string                      output_name       ; ///< The name of this object's synchronization output.
      std::string                      output_image_name ; ///< The name of this object's framebuffer output.
      std::string                      name              ; ///< The name of this module.
      glm::mat4                        view              ;
      bool                             debug             ;
      unsigned                         resx              ;
      unsigned                         resy              ;
      std::mutex                       mutex             ;
      
      /** Default Constructor. Initializes member data.
       */
      InstancedSpriteSheetData() ;
      
      /** Method to set this object's window's name.
       * @param name The name of window this object uses.
       */
      void setWindowName( const char* name ) ;

      /** Method to set the GPU this object uses for operations.
       */
      void setGPU( unsigned gpu ) ;
      
      /** Method to set the x resolution of this object.
       * @param x The value to set this object's x resolution to.
       */
      void setResolutionX( unsigned x ) ;
      
      /** Method to set the y resolution of this object.
       * @param y The value to set this object's y resolution to.
       */
      void setResolutionY( unsigned y ) ;

      /** Method to set this object's output framebuffer name.
       * @param output The name of this object's framebuffer output.
       */
      void setOutputImageName( const char* output ) ;
      
      /** Method to push a command to this object for operation.
      */
      void setCommand( const kgl::List<::kgl::SheetCommand>& commands ) ;
      
      /** Method to retrieve a camera to use for this object's rendering.
       */
      void setCamera( const ::kgl::Camera& camera ) ;

      /** Method to convert a sheet command to a transformation.
       * @param cmd The command to convert
       * @return The transformation object equivalent of the input command.
       */
      Transformation transformFromCommand( const SheetCommand& cmd ) ;

      /** Method to prepare the transformation matrix of this object for rendering.
       */
      void setUpModelMatrix() ;
      
      /** Method to set the texture coordinates of the vertices to match what the sprite being drawn.
       */
      Transformation setUpTextureCoords( const ::kgl::man::Atlas& sheet, unsigned index ) ;

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
    
    Transformation InstancedSpriteSheetData::transformFromCommand( const SheetCommand& cmd )
    {
      const float       x   = cmd.posX()     ;
      const float       y   = cmd.posY()     ;
      const float       w   = cmd.width()    ;
      const float       h   = cmd.height()   ;
      const float       r   = cmd.rotation() ;
      const std::string img = cmd.sheet()    ;
      Transformation trans ;
      glm::mat4      model ;
      glm::vec2      size  ;
     
      if( this->manager.contains( cmd.sheet() ) )
      {
        if( w < 0.1f && h < 0.1f )
        {
          const auto image = &this->manager.atlas( cmd.sheet() ) ;
          size = glm::vec2( image->spriteWidth(), image->spriteHeight() ) ;
        }
        else
        {
          size = glm::vec2( w, h ) ;
        }
  
        model = glm::mat4( 1.f ) ;
        
        glm::vec3 pos( x, y, 0.0f ) ;
        model = glm::translate( model, pos ) ;
        model = glm::translate( model, glm::vec3   ( 0.5f  * w , 0.5f * h, 0.0f       ) ) ; 
        model = glm::rotate   ( model, glm::radians( r ), glm::vec3( 0.0f, 0.0f, 1.0f ) ) ; 
        model = glm::translate( model, glm::vec3   ( -0.5f * w, -0.5f * h, 0.0f       ) ) ;
        model = glm::scale    ( model, glm::vec3( size, 1.0f ) ) ; 
  
        
        trans = this->setUpTextureCoords( this->manager.atlas( cmd.sheet() ), cmd.index() ) ;
        trans.model = model ;
      }
      
      return trans ;
    }

    void InstancedSpriteSheetData::setCamera( const ::kgl::Camera& camera )
    {
      const glm::vec3 pos   = glm::vec3( camera.posX()  , camera.posY()  , camera.posZ()   ) ;
      const glm::vec3 front = glm::vec3( camera.frontX(), camera.frontY(), camera.frontZ() ) ;
      const glm::vec3 up    = glm::vec3( camera.upX()   , camera.upY()   , camera.upZ()    ) ;
      
      this->view = glm::lookAt( pos, pos + front, up ) ;
    }
    
    void InstancedSpriteSheetData::setResolutionX( unsigned x )
    {
      this->resx = x ;
    }
    
    void InstancedSpriteSheetData::setResolutionY( unsigned y )
    {
      this->resy = y ;
    }
    
    InstancedSpriteSheetData::InstancedSpriteSheetData()
    {
      this->debug          = false            ;
      this->view           = glm::mat4( 1.f ) ;
      this->resx           = 0                ;
      this->resy           = 0                ;
    }
    
    void InstancedSpriteSheetData::setDebug( bool val )
    {
      this->debug = val ;
    }

    void InstancedSpriteSheetData::setCommand( const ::kgl::List<::kgl::SheetCommand>& commands )
    {
      this->transforms.clear() ;

      for( const auto &command : commands )
      {
        this->transforms[ command.sheet() ].push_back( this->transformFromCommand( command ) ) ;
      }
      
      for( auto list : this->transforms )
      {
        auto iter = this->materials.find( list.first ) ;
  
        // If we don't have enough uniform objects for the amount of commands we have, scale up.
        if( iter == this->materials.end() )
        {
          if( this->manager.contains( list.first.c_str() ) )
          {
            auto mat = this->materials.emplace( list.first, Material() ) ;
  
            mat.first->second.set = this->pool.makeDescriptorSet( this->pass.numBuffers()                                   ) ;
            mat.first->second.uniform.initialize( this->gpu                                                                 ) ;
            mat.first->second.uniform.addImage  ( "image"     , this->manager.atlas( list.first.c_str() ).image()           ) ;
            mat.first->second.uniform.add       ( "projection", Uniform::Type::UBO , this->projection                       ) ;
            mat.first->second.uniform.add       ( "offsets"   , Uniform::Type::SSBO, list.second.data(), list.second.size() ) ;
            mat.first->second.set.set( mat.first->second.uniform ) ;
          }
        }
      }
    }

    Transformation InstancedSpriteSheetData::setUpTextureCoords( const ::kgl::man::Atlas& sheet, unsigned index )
    {
      Transformation ret ;
      const unsigned sprite             = index                                            ;
      const unsigned sprite_width       = sheet.spriteWidth()                              ;
      const unsigned sprite_height      = sheet.spriteHeight()                             ;
      const unsigned image_width        = sheet.image().width()                            ;
      const unsigned image_height       = sheet.image().height()                           ;
      const unsigned num_sprites_in_row = image_width  / sprite_width                      ; 
      const unsigned sprite_y_index     = sprite / num_sprites_in_row                      ;
      const unsigned sprite_x_index     = sprite - ( sprite_y_index * num_sprites_in_row ) ;
      const unsigned sprite_ypixel      = sprite_y_index * sprite_height                   ;
      const unsigned sprite_xpixel      = sprite_x_index * sprite_width                    ;
      
      const float top_left_x     = static_cast<float>( sprite_xpixel                 ) / static_cast<float>( image_width  ) ;
      const float top_left_y     = static_cast<float>( sprite_ypixel                 ) / static_cast<float>( image_height ) ;
      const float top_right_x    = static_cast<float>( sprite_xpixel + sprite_width  ) / static_cast<float>( image_width  ) ;
      const float top_right_y    = static_cast<float>( sprite_ypixel                 ) / static_cast<float>( image_height ) ;
      const float bottom_left_x  = static_cast<float>( sprite_xpixel                 ) / static_cast<float>( image_width  ) ;
      const float bottom_left_y  = static_cast<float>( sprite_ypixel + sprite_height ) / static_cast<float>( image_height ) ;
      const float bottom_right_x = static_cast<float>( sprite_xpixel + sprite_width  ) / static_cast<float>( image_width  ) ;
      const float bottom_right_y = static_cast<float>( sprite_ypixel + sprite_height ) / static_cast<float>( image_height ) ;
      
      ret.bl.x = bottom_left_x  ;
      ret.bl.y = bottom_left_y  ;
      ret.br.x = bottom_right_x ;
      ret.br.y = bottom_right_y ;
      ret.tr.x = top_right_x    ;
      ret.tr.y = top_right_y    ;
      ret.tl.x = top_left_x     ;
      ret.tl.y = top_left_y     ;
      
      return ret ;
    }

    void InstancedSpriteSheetData::setOutputImageName( const char* output ) 
    {
      this->output_image_name = output ;
    }

    void InstancedSpriteSheetData::setWindowName( const char* name )
    {
      this->window_name = name ;
    }

    void InstancedSpriteSheetData::setGPU( unsigned gpu )
    {
      this->gpu    = gpu                                ;
      this->device = this->context.virtualDevice( gpu ) ;
    }
    
    void InstancedSpriteSheet::input( const ::kgl::vk::Synchronization& sync )
    {
      data().mutex.lock() ;
      data().syncs.value().clear() ;
      data().syncs.value().addWait( sync.signalSem( this->id() ) ) ;
      this->semIncrement() ;
      data().mutex.unlock() ;
    }

    void InstancedSpriteSheetData::output( const Synchronization& sync )
    {
      const unsigned index = this->context.currentSwap( this->window_name.c_str() ) ;
     
      this->bus( this->output_image_name.c_str() ).emit( this->pass.image( index ) ) ;
      this->bus( this->output_name      .c_str() ).emit( sync                      ) ;
    }

    void InstancedSpriteSheet::setInputName( const char* name )
    {
      static bool found_input = false ;

      // We now know what input we're getting. Subscribe & add as a dependancy.
      if( !found_input )
      {
        data().bus( name ).attach( this, &InstancedSpriteSheet::input ) ;
        
        found_input = true ;
      }
    }

    void InstancedSpriteSheetData::setOutputName( const char* name )
    {
      this->output_name = name ;
    }

    InstancedSpriteSheet::InstancedSpriteSheet()
    {
      this->isprite_data = new InstancedSpriteSheetData() ;
    }
    
    InstancedSpriteSheet::~InstancedSpriteSheet()
    {
      delete this->isprite_data ;
    }

    void InstancedSpriteSheet::resize()
    {
      const unsigned     width    = data().context.width ( data().window_name.c_str() ) ; ///< Width of the screen.
      const unsigned     height   = data().context.height( data().window_name.c_str() ) ; ///< Height of the screen.
      static const char* path     = "/uwu/isprite.uwu"                                  ; ///< Path to this object's shader in the local-directory.
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

    void InstancedSpriteSheet::initialize()
    {
      const unsigned     MAX_SETS = 20                                                  ; ///< The max number of descriptor sets allowed.
      const unsigned     width    = data().context.width ( data().window_name.c_str() ) ; ///< Width of the screen.
      const unsigned     height   = data().context.height( data().window_name.c_str() ) ; ///< Height of the screen.
      static const char* path     = "/uwu/isprite.uwu"                                  ; ///< Path to this object's shader in the local-directory.
      std::string pipeline_path ;
      
      std::vector<float> vert = 
      { 
        // pos    
        0.0f, 1.0f, 
        1.0f, 0.0f,
        0.0f, 0.0f, 
        0.0f, 1.0f,
        1.0f, 1.0f, 
        1.0f, 0.0f
      };

      data().pass.setImageFinalLayout( ::vk::ImageLayout::eGeneral ) ;
      data().pipeline.setPushConstantByteSize ( sizeof( glm::mat4 )                                         ) ;
      data().pipeline.setPushConstantStageFlag( static_cast<unsigned>( ::vk::ShaderStageFlagBits::eVertex ) ) ;

      this->setNumDependancies( 1 ) ;

      // Build path to this object's shdder.
      pipeline_path = ::kgl::vk::basePath() ;
      pipeline_path = pipeline_path + path  ;
      
//      data().pipeline.shader().setVertexInputType( ::vk::VertexInputRate::eInstance ) ;

      // Initialize vulkan objects.
      data().vertices        .initialize<float>( data().gpu, Buffer::Type::VERTEX, 12                                                   ) ;
      data().pass            .initialize       ( data().window_name.c_str(), data().gpu                                                 ) ;
      data().buffer.seek( 0 ).initialize       ( data().window_name.c_str(), data().gpu, data().pass.numBuffers(), BufferLevel::Primary ) ;
      data().buffer.seek( 1 ).initialize       ( data().window_name.c_str(), data().gpu, data().pass.numBuffers(), BufferLevel::Primary ) ;
      data().buffer.seek( 2 ).initialize       ( data().window_name.c_str(), data().gpu, data().pass.numBuffers(), BufferLevel::Primary ) ;
      data().pipeline        .initialize       ( pipeline_path.c_str(), data().gpu, width, height, data().pass.pass()                   ) ;
      data().pool            .initialize       ( data().gpu, MAX_SETS, data().pipeline.shader()                                         ) ;
      data().syncs.seek( 0 ) .initialize       ( data().gpu                                                                             ) ;
      data().syncs.seek( 1 ) .initialize       ( data().gpu                                                                             ) ;
      data().syncs.seek( 2 ) .initialize       ( data().gpu                                                                             ) ;
      data().syncs.seek( 3 ) .initialize       ( data().gpu                                                                             ) ;
      
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
      
      // Copy vertex data to the device.
      data().vertices.copyToDevice( vert.data() ) ;
    }

    void InstancedSpriteSheet::shutdown()
    {
//      data().sync    .reset() ; ///TODO
//      data().uniform .reset() ; ///TODO
      data().vertices.reset() ; 
//      data().pass    .reset() ; ///TODO
//      data().buffer  .reset() ; ///TODO
//      data().pipeline.reset() ; ///TODO
    }

    void InstancedSpriteSheet::subscribe( const char* pipeline, unsigned id )
    {
      const std::string json_path = std::string( "Graphs::" ) + std::string( pipeline ) + std::string( "::Modules::" ) + std::string( this->name() ) ;
      data().name = this->name() ;
      data().pass.subscribe( this->name(), id ) ;
      
      data().pipeline.subscribe( json_path.c_str(), id ) ;
      
      // Graph-specific JSON-parameters.
      data().bus( "Graphs::", pipeline, "::window" ).attach( this->isprite_data, &InstancedSpriteSheetData::setWindowName ) ;
      data().bus( "Graphs::", pipeline, "::gpu"    ).attach( this->isprite_data, &InstancedSpriteSheetData::setGPU        ) ;
      
      // Module-specific JSON-parameters.
      data().bus( json_path.c_str(), "::input"        ).attach( this         , &InstancedSpriteSheet::setInputName                ) ;
      data().bus( json_path.c_str(), "::sem_id"       ).attach( dynamic_cast<Module*>( this ), &Module::setId                     ) ;
      data().bus( json_path.c_str(), "::output"       ).attach( this->isprite_data, &InstancedSpriteSheetData::setOutputName      ) ;
      data().bus( json_path.c_str(), "::output_image" ).attach( this->isprite_data, &InstancedSpriteSheetData::setOutputImageName ) ;
      data().bus( json_path.c_str(), "::debug"        ).attach( this->isprite_data, &InstancedSpriteSheetData::setDebug           ) ;
      data().bus( json_path.c_str(), "::resx"         ).attach( this->isprite_data, &InstancedSpriteSheetData::setResolutionX     ) ;
      data().bus( json_path.c_str(), "::resy"         ).attach( this->isprite_data, &InstancedSpriteSheetData::setResolutionY     ) ;

      // Module-specific inputs.
      data().bus( this->name(), "::cmd"    ).attach( this->isprite_data, &InstancedSpriteSheetData::setCommand ) ;
      data().bus( this->name(), "::camera" ).attach( this->isprite_data, &InstancedSpriteSheetData::setCamera  ) ;

      // Set our own Render Pass information, but allow it to be overwritten by JSON configuration.
      data().bus( json_path.c_str(), "::ViewportX"        ).emit( 0, 0.f ) ;
      data().bus( json_path.c_str(), "::ViewportY"        ).emit( 0, 0.f ) ;
      data().bus( json_path.c_str(), "::ViewportMinDepth" ).emit( 0, 0.f ) ;
      data().bus( json_path.c_str(), "::ViewportMaxDepth" ).emit( 0, 1.f ) ;
    }

    void InstancedSpriteSheet::execute()
    {
      Synchronization sync    ;
      data().profiler.start() ;
      
//      data().mutex.lock() ;
      sync = data().syncs.value() ;
      data().syncs .swap() ;
      data().buffer.swap() ;
//      data().mutex.unlock() ;
      
      if( !data().transforms.empty() )
      {
        data().buffer.value().record( data().pass ) ;
        for( auto transform : data().transforms )
        {
          auto iter = data().materials.find( transform.first ) ;
          
          data().pipeline        .bind( data().buffer.value(), iter->second.set                                                                     ) ;
          data().buffer.value().pushConstant( data().view, data().pipeline.layout(), static_cast<unsigned>( ::vk::ShaderStageFlagBits::eVertex ), 1 ) ;
          data().buffer.value().drawInstanced( data().vertices.buffer(), 12, transform.second.size() ) ;
        }
        data().buffer.value().stop() ;
        data().pass.submit( sync, data().buffer.value(), data().buffer.current() ) ;
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

    InstancedSpriteSheetData& InstancedSpriteSheet::data()
    {
      return *this->isprite_data ;
    }

    const InstancedSpriteSheetData& InstancedSpriteSheet::data() const
    {
      return *this->isprite_data ;
    }
  }
}
/** Exported function to retrive the name of this module type.
 * @return The name of this object's type.
 */
exported_function const char* name()
{
  return "InstancedSpriteSheet" ;
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
  return new ::kgl::vk::InstancedSpriteSheet() ;
}

/** Exported function to destroy an instance of this module.
 * @param module A Pointer to a Module object that is of this type.
 */
exported_function void destroy( ::kgl::vk::Module* module )
{
  ::kgl::vk::InstancedSpriteSheet* mod ;
  
  mod = dynamic_cast<::kgl::vk::InstancedSpriteSheet*>( module ) ;
  delete mod ;
}

