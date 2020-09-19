#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_LEFT_HANDED
#include "SpriteSheet.h"
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
#include <mutex>
#include <sstream>
#include <iostream>
#include <map>
#include <atomic>


namespace kgl
{
  namespace vk
  {
    struct Transformation
    {
      glm::mat4  model   ;
      glm::uvec4 padding ;
    };
    
    struct SpriteInfo
    {
      unsigned sprite_width ;
      unsigned sprite_height;
      unsigned image_width  ;
      unsigned image_height ;
    };
    
    struct SpriteSheetData
    {
      struct Material
      {
        Uniform       uniform      ;
        DescriptorSet set          ;
        bool          init = false ;
      };

      static constexpr unsigned BUFFERS = 3 ; // Todo make this 
      typedef ::kgl::BufferedStack<SheetCommand, BUFFERS>                   Stack      ;
      typedef kgl::containers::Layered<::kgl::vk::render::CommandBuffer, 3> CmdBuffers ;
      typedef kgl::containers::Layered<Synchronization                 , 3> Syncs      ;

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
      
      karma::ms::Timer                 profiler          ;
      Syncs                            syncs             ;
      std::vector<float>               data_host_buff    ;
      CmdBuffers                       buffer            ; ///< The command buffer to use for all GPU calls.
      Material                         material          ; ///< Objects to contain all data to be sent to a uniform.
      ::kgl::vk::render::Context       context           ; ///< The context to use for vulkan state information.
      ::kgl::vk::Window*               window            ; ///< The window this object is a child of.
      ::kgl::vk::render::Pipeline      pipeline          ; ///< The pipeline that is associated with this object's rendering.
      ::kgl::vk::RenderPass            pass              ; ///< The render pass that defines this object's rendering.
      ::kgl::vk::Buffer                vertices          ; ///< The buffer that contains the vertex data that this object uses to render.
      ::kgl::vk::Buffer                data_buffer       ; ///< The buffer that contains the vertex data that this object uses to render.
      ::kgl::vk::DescriptorPool        pool              ; ///< The object that contains the metadata to make descriptor sets.
      ::kgl::man::AssetManager         manager           ; ///< Asset manager object for grabbing loaded images.
      std::string                      sprite_sheet      ;
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
      bool                             debug             ;
      std::mutex                       sync_mutex        ;
      std::mutex                       cmd_mutex         ;
      unsigned                         resx              ;
      unsigned                         start             ;
      unsigned                         stop              ;
      unsigned                         data_sz           ;
      unsigned                         resy              ;
      bool                             found_input       ;
      float                            camera_x          ;
      float                            camera_y          ;
      unsigned                         offset            ;
      bool                             has_image         ;
      
      /** Default Constructor. Initializes member data.
       */
      SpriteSheetData() ;
      
      /** Method to set this object's window's name.
       * @param name The name of window this object uses.
       */
      void setWindowName( const char* name ) ;
      
      /** Method to set where to start rendering.
       * @param start Where in this object's command list to start rendering.
       */
      void setStart( unsigned start ) ;
      
      /** Method to set where to stop rendering.
       * @param stop Where in this object's command list to stop rendering.
       */
      void setStop( unsigned stop ) ;
      
      /** Method to set the size of this object's command list.
       * @param size The size of this object's internal command list.
       */
      void setSize( unsigned size ) ;
      
      /**
       */
      void checkForImage() ;

      /**
       * @param cmd
       * @return 
       */
      bool insideCamera( const kgl::SheetCommand& cmd ) ;

      /** Method to pop a command off the stack for processing.
       */
      void pop() ;
      
      /**
       * @param sheet_name
       */
      void setSpriteSheet( const char* sheet_name ) ;

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
      void setCommand( unsigned index, const ::kgl::SheetCommand& cmd ) ;
      
      /** Method to push a command to this object for operation.
      */
      void setCommandListOffset( unsigned offset, const kgl::List<kgl::SheetCommand>& list ) ;
      
      /** Method to push a command to this object for operation.
      */
      void setCommandList( const kgl::List<kgl::SheetCommand>& list ) ;
      
      /** Method to prepare the transformation matrix of this object for rendering.
       */
      glm::mat4 setUpModelMatrix( const kgl::SheetCommand& current_cmd ) ;
      
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
    
    SpriteSheetData::SpriteSheetData()
    {
      this->resx           = 0                ;
      this->resy           = 0                ;
      this->debug          = false            ;
      this->found_input    = false            ;
      this->camera_x       = 0.0f             ;
      this->camera_y       = 0.0f             ;
      this->view           = glm::mat4( 1.f ) ;
      this->offset         = 0                ;
      this->start          = 0                ;
      this->stop           = 0                ;
      this->data_sz        = 2000             ;
      this->has_image      = false            ;
    }
    
    void SpriteSheetData::checkForImage() 
    {
      if( !this->has_image )
      {
        if( this->manager.contains( this->sprite_sheet.c_str() ) )
        {
          SpriteInfo  info ;

          info.sprite_width  = this->manager.atlas( this->sprite_sheet.c_str() ).spriteWidth()   ;
          info.sprite_height = this->manager.atlas( this->sprite_sheet.c_str() ).spriteHeight()  ;
          info.image_width   = this->manager.atlas( this->sprite_sheet.c_str() ).image().width() ;
          info.image_height  = this->manager.atlas( this->sprite_sheet.c_str() ).image().width() ;
          
          this->has_image = true ;
          
          this->material.set = this->pool.makeDescriptorSet( this->pass.numBuffers()                                 ) ;
          this->material.uniform.initialize( this->gpu                                                                ) ;
          this->material.uniform.addImage  ( "image"     , this->manager.atlas( this->sprite_sheet.c_str() ).image() ) ;
          this->material.uniform.add       ( "projection", Uniform::Type::UBO, this->projection                       ) ;
          this->material.uniform.add       ( "camera"    , Uniform::Type::UBO, this->view, true                       ) ;
          this->material.uniform.add       ( "SpriteInfo", Uniform::Type::UBO , info                                   ) ;
          this->material.set.set           ( this->material.uniform                                                   ) ;
          this->material.set.setSSBO       ( "offsets", this->data_buffer                                             ) ;
        }
      }
    }

    void SpriteSheetData::setDebug( bool val )
    {
      this->debug = val ;
    }

    void SpriteSheetData::setSpriteSheet( const char* sheet_name )
    {
      this->sprite_sheet = sheet_name ;
    }
    
    void SpriteSheetData::setResolutionX( unsigned x )
    {
      this->resx = x ;
    }
    
    void SpriteSheetData::setResolutionY( unsigned y )
    {
      this->resy = y ;
    }
    
    void SpriteSheetData::setStart( unsigned start )
    {
      this->start = start ;
    }
    
    void SpriteSheetData::setStop( unsigned stop )
    {
      this->stop = stop ;
    }
    
    void SpriteSheetData::setSize( unsigned size )
    {
      this->data_sz = size ;
      if( this->data_buffer.isInitialized() )
      {
        this->data_buffer.reset() ;
      }
      
      this->data_buffer.initialize<Transformation>( this->gpu, Buffer::Type::SSBO, size * ( sizeof( float ) * 20 ), true ) ;
    }
    
    void SpriteSheetData::setCamera( const ::kgl::Camera& camera )
    {
      const glm::vec3 pos   = glm::vec3( camera.posX()  , camera.posY()  , camera.posZ()   ) ;
      const glm::vec3 front = glm::vec3( camera.frontX(), camera.frontY(), camera.frontZ() ) ;
      const glm::vec3 up    = glm::vec3( camera.upX()   , camera.upY()   , camera.upZ()    ) ;
      this->camera_x = camera.posX() ;
      this->camera_y = camera.posY() ;
      this->view = glm::lookAt( pos, pos + front, up ) ;
      
      this->material.uniform.add( "camera", Uniform::Type::UBO , this->view, true ) ;
    }
    
    bool SpriteSheetData::insideCamera( const kgl::SheetCommand& cmd )
    {
      const float xpos = cmd.posX() ;
      const float ypos = cmd.posX() ;
      
      return ( xpos > this->camera_x && xpos < this->camera_x + this->resx &&
               ypos > this->camera_y && ypos < this->camera_y + this->resy   ) ;
    }
    
    void SpriteSheetData::setCommandListOffset( unsigned offset, const kgl::List<kgl::SheetCommand>& list ) 
    {
      unsigned index ;
      
      index = 0 ;
      for( auto sheet : list )
      {
        if( offset + index < this->data_sz )
        {
          this->setCommand( offset + index, sheet ) ;
        }
        index++ ;
      }
    }

    void SpriteSheetData::setCommandList( const kgl::List<kgl::SheetCommand>& list ) 
    {
      unsigned index ;
      
      index = 0 ;
      for( auto sheet : list )
      {
        this->setCommand( index, sheet ) ;
        index++ ;
      }
    }

    void SpriteSheetData::setCommand( unsigned index, const ::kgl::SheetCommand& cmd )
    {
      const unsigned input_size = sizeof( float ) * 20   ;
      const unsigned offset     = index * ( input_size ) ;
      float          tmp_buff[ 20 ] ;
      glm::mat4      model_matrix   ;

      model_matrix = setUpModelMatrix( cmd ) ;
      
      this->cmd_mutex.lock() ;
      tmp_buff[ 0  ] = model_matrix[ 0 ][ 0 ]            ; 
      tmp_buff[ 1  ] = model_matrix[ 0 ][ 1 ]            ; 
      tmp_buff[ 2  ] = model_matrix[ 0 ][ 2 ]            ; 
      tmp_buff[ 3  ] = model_matrix[ 0 ][ 3 ]            ; 
      tmp_buff[ 4  ] = model_matrix[ 1 ][ 0 ]            ; 
      tmp_buff[ 5  ] = model_matrix[ 1 ][ 1 ]            ; 
      tmp_buff[ 6  ] = model_matrix[ 1 ][ 2 ]            ; 
      tmp_buff[ 7  ] = model_matrix[ 1 ][ 3 ]            ; 
      tmp_buff[ 8  ] = model_matrix[ 2 ][ 0 ]            ; 
      tmp_buff[ 9  ] = model_matrix[ 2 ][ 1 ]            ; 
      tmp_buff[ 10 ] = model_matrix[ 2 ][ 2 ]            ; 
      tmp_buff[ 11 ] = model_matrix[ 2 ][ 3 ]            ; 
      tmp_buff[ 12 ] = model_matrix[ 3 ][ 0 ]            ; 
      tmp_buff[ 13 ] = model_matrix[ 3 ][ 1 ]            ; 
      tmp_buff[ 14 ] = model_matrix[ 3 ][ 2 ]            ; 
      tmp_buff[ 15 ] = model_matrix[ 3 ][ 3 ]            ; 
      tmp_buff[ 16 ] = static_cast<float>( cmd.index() ) ;
      
      this->data_buffer.copyToDevice( tmp_buff, input_size, offset )  ;
      this->cmd_mutex.unlock() ;
    }

    glm::mat4 SpriteSheetData::setUpModelMatrix( const kgl::SheetCommand& current_cmd )
    {
      const float       x   = current_cmd.posX()     ;
      const float       y   = current_cmd.posY()     ;
      const float       w   = current_cmd.width()    ;
      const float       h   = current_cmd.height()   ;
      const float       r   = current_cmd.rotation() ;
      const std::string img = this->sprite_sheet     ;
      glm::mat4 model_matrix ;
      glm::vec2 size         ;
     
      if( w < 0.1f && h < 0.1f && this->manager.contains( img.c_str() ) )
      {
        const auto image = &this->manager.atlas( this->sprite_sheet.c_str() ) ;
        size = glm::vec2( image->spriteWidth(), image->spriteHeight() ) ;
      }
      else
      {
        size = glm::vec2( w, h ) ;
      }

      model_matrix = glm::mat4( 1.f ) ;
      
      glm::vec3 pos( x, y, 0.0f ) ;
      model_matrix = glm::translate( model_matrix, pos ) ;
      model_matrix = glm::translate( model_matrix, glm::vec3   ( 0.5f  * w , 0.5f * h, 0.0f       ) ) ; 
      model_matrix = glm::rotate   ( model_matrix, glm::radians( r ), glm::vec3( 0.0f, 0.0f, 1.0f ) ) ; 
      model_matrix = glm::translate( model_matrix, glm::vec3   ( -0.5f * w, -0.5f * h, 0.0f       ) ) ;
      model_matrix = glm::scale    ( model_matrix, glm::vec3   ( size, 1.0f )                       ) ; 
      
      return model_matrix ;
    }

    void SpriteSheetData::setOutputImageName( const char* output ) 
    {
      this->output_image_name = output ;
    }

    void SpriteSheetData::setWindowName( const char* name )
    {
      this->window_name = name ;
    }

    void SpriteSheetData::setGPU( unsigned gpu )
    {
      this->gpu    = gpu                                ;
      this->device = this->context.virtualDevice( gpu ) ;
    }
    
    void SpriteSheet::input( const ::kgl::vk::Synchronization& sync )
    {
      data().sync_mutex.lock() ;
      data().syncs.value().clear() ;
      data().syncs.value().addWait( sync.signalSem( this->id() ) ) ;
      this->semIncrement() ;
      data().sync_mutex.unlock() ;
    }

    void SpriteSheetData::output( const Synchronization& sync )
    {
      const unsigned index = this->context.currentSwap( this->window_name.c_str() ) ;
     
      this->bus( this->output_image_name.c_str() ).emit( this->pass.image( index ) ) ;
      this->bus( this->output_name      .c_str() ).emit( sync                      ) ;
    }

    void SpriteSheet::setInputName( const char* name )
    {
      // We now know what input we're getting. Subscribe & add as a dependancy.
      if( !data().found_input )
      {
        data().bus( name ).attach( this, &SpriteSheet::input ) ;
        data().found_input = true ;
      }
    }

    void SpriteSheetData::setOutputName( const char* name )
    {
      this->output_name = name ;
    }

    SpriteSheet::SpriteSheet()
    {
      this->data_2d = new SpriteSheetData() ;
    }
    
    SpriteSheet::~SpriteSheet()
    {
      delete this->data_2d ;
    }

    void SpriteSheet::resize()
    {
      const unsigned     width    = data().context.width ( data().window_name.c_str() ) ; ///< Width of the screen.
      const unsigned     height   = data().context.height( data().window_name.c_str() ) ; ///< Height of the screen.
      static const char* path     = "/uwu/sprite.uwu"                                   ; ///< Path to this object's shader in the local-directory.
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
      
      data().material.uniform.add( "projection", Uniform::Type::UBO , data().projection ) ;
    }

    void SpriteSheet::initialize()
    {
      const unsigned     MAX_SETS = 3                                                   ; ///< The max number of descriptor sets allowed.
      const unsigned     width    = data().context.width ( data().window_name.c_str() ) ; ///< Width of the screen.
      const unsigned     height   = data().context.height( data().window_name.c_str() ) ; ///< Height of the screen.
      static const char* path     = "/uwu/sprite.uwu"                                   ; ///< Path to this object's shader in the local-directory.

      SpriteInfo  info          ;
      std::string pipeline_path ;
      
      this->setNumDependancies( 1 ) ;

      // Build path to this object's shdder.
      pipeline_path = ::kgl::vk::basePath() ;
      pipeline_path = pipeline_path + path  ;
      
      if( !data().data_buffer.isInitialized() )
      {
        data().data_buffer.initialize<Transformation>( data().gpu, Buffer::Type::SSBO, data().data_sz, true ) ;
      }
      
      data().pass.setImageFinalLayout( ::vk::ImageLayout::eGeneral ) ;

      // Initialize vulkan objects.
      data().vertices        .initialize<float>         ( data().gpu, Buffer::Type::VERTEX, 18                                          ) ;
      data().data_buffer     .initialize<float>( data().gpu, Buffer::Type::SSBO, sizeof( Transformation ) * 8000, true                  ) ;
      data().pass            .initialize       ( data().window_name.c_str(), data().gpu                                                 ) ;
      data().buffer.seek( 0 ).initialize       ( data().window_name.c_str(), data().gpu, data().pass.numBuffers(), BufferLevel::Primary ) ;
      data().buffer.seek( 1 ).initialize       ( data().window_name.c_str(), data().gpu, data().pass.numBuffers(), BufferLevel::Primary ) ;
      data().buffer.seek( 2 ).initialize       ( data().window_name.c_str(), data().gpu, data().pass.numBuffers(), BufferLevel::Primary ) ;
      data().pipeline        .initialize       ( pipeline_path.c_str(), data().gpu, width, height, data().pass.pass()                   ) ;
      data().pool            .initialize       ( data().gpu, MAX_SETS, data().pipeline.shader()                                         ) ;
      data().syncs.seek( 0 ) .initialize       ( data().gpu                                                                             ) ;
      data().syncs.seek( 1 ) .initialize       ( data().gpu                                                                             ) ;
      data().syncs.seek( 2 ) .initialize       ( data().gpu                                                                             ) ;
      data().profiler        .initialize       (                                                                                        ) ;
      
      if( data().manager.contains( data().sprite_sheet.c_str() ) )
      {
        info.sprite_width  = data().manager.atlas( data().sprite_sheet.c_str() ).spriteWidth()   ;
        info.sprite_height = data().manager.atlas( data().sprite_sheet.c_str() ).spriteHeight()  ;
        info.image_width   = data().manager.atlas( data().sprite_sheet.c_str() ).image().width() ;
        info.image_height  = data().manager.atlas( data().sprite_sheet.c_str() ).image().width() ;
        
        data().has_image = true ;
        
        data().material.set = data().pool.makeDescriptorSet( data().pass.numBuffers()                                 ) ;
        data().material.uniform.initialize( data().gpu                                                                ) ;
        data().material.uniform.addImage  ( "image"     , data().manager.atlas( data().sprite_sheet.c_str() ).image() ) ;
        data().material.uniform.add       ( "projection", Uniform::Type::UBO, data().projection                       ) ;
        data().material.uniform.add       ( "camera"    , Uniform::Type::UBO, data().view, true                       ) ;
        data().material.uniform.add       ( "SpriteInfo", Uniform::Type::UBO , info                                   ) ;
        data().material.set.set           ( data().material.uniform                                                   ) ;
        data().material.set.setSSBO       ( "offsets", data().data_buffer                                             ) ;
      }

      data().window = &data().context.window( data().window_name.c_str() ) ;

      if( data().resx == 0 && data().resy == 0 )
      {
        data().projection = glm::ortho(  0.0f, (float)width, 0.0f, (float)height, -100.0f, 100.0f ) ;
      }
      else
      {
        data().projection = glm::ortho(  0.0f, (float)data().resx, 0.0f, (float)data().resy, -100.0f, 100.0f ) ;
      }
      
      // Copy vertex data to the device.
      data().vertices.copyToDevice( data().vert.data() ) ;
    }

    void SpriteSheet::shutdown()
    {
      data().vertices        .reset() ; 
      data().pass            .reset() ;
      data().buffer.seek( 0 ).reset() ; 
      data().buffer.seek( 1 ).reset() ; 
      data().buffer.seek( 2 ).reset() ; 
      data().pipeline        .reset() ; 
    }

    void SpriteSheet::subscribe( const char* pipeline, unsigned id )
    {
      const std::string json_path = std::string( "Graphs::" ) + std::string( pipeline ) + std::string( "::Modules::" ) + std::string( this->name() ) ;
      data().name = this->name() ;
      data().pass.subscribe( this->name(), id ) ;
      
      data().pipeline.subscribe( json_path.c_str(), id ) ;
      
      // Graph-specific JSON-parameters.
      data().bus( "Graphs::", pipeline, "::window" ).attach( this->data_2d, &SpriteSheetData::setWindowName ) ;
      data().bus( "Graphs::", pipeline, "::gpu"    ).attach( this->data_2d, &SpriteSheetData::setGPU        ) ;
      
      // Module-specific JSON-parameters.
      data().bus( json_path.c_str(), "::input"        ).attach( this         , &SpriteSheet::setInputName           ) ;
      data().bus( json_path.c_str(), "::sem_id"       ).attach( dynamic_cast<Module*>( this ), &Module::setId       ) ;
      data().bus( json_path.c_str(), "::output"       ).attach( this->data_2d, &SpriteSheetData::setOutputName      ) ;
      data().bus( json_path.c_str(), "::output_image" ).attach( this->data_2d, &SpriteSheetData::setOutputImageName ) ;
      data().bus( json_path.c_str(), "::debug"        ).attach( this->data_2d, &SpriteSheetData::setDebug           ) ;
      data().bus( json_path.c_str(), "::resx"         ).attach( this->data_2d, &SpriteSheetData::setResolutionX     ) ;
      data().bus( json_path.c_str(), "::resy"         ).attach( this->data_2d, &SpriteSheetData::setResolutionY     ) ;
      data().bus( json_path.c_str(), "::sheet"        ).attach( this->data_2d, &SpriteSheetData::setSpriteSheet     ) ;
      data().bus( json_path.c_str(), "::data_size"    ).attach( this->data_2d, &SpriteSheetData::setSize            ) ;
      
      // Module-specific inputs.
      data().bus( this->name(), "::set"    ).attach( this->data_2d, &SpriteSheetData::setCommand           ) ;
      data().bus( this->name(), "::start"  ).attach( this->data_2d, &SpriteSheetData::setStart             ) ;
      data().bus( this->name(), "::stop"   ).attach( this->data_2d, &SpriteSheetData::setStop              ) ;
      data().bus( this->name(), "::list"   ).attach( this->data_2d, &SpriteSheetData::setCommandList       ) ;
      data().bus( this->name(), "::list"   ).attach( this->data_2d, &SpriteSheetData::setCommandListOffset ) ;
      data().bus( this->name(), "::camera" ).attach( this->data_2d, &SpriteSheetData::setCamera            ) ;

      // Set our own Render Pass information, but allow it to be overwritten by JSON configuration.
      data().bus( json_path.c_str(), "::ViewportX"        ).emit( 0, 0.f ) ;
      data().bus( json_path.c_str(), "::ViewportY"        ).emit( 0, 0.f ) ;
      data().bus( json_path.c_str(), "::ViewportMinDepth" ).emit( 0, 0.f ) ;
      data().bus( json_path.c_str(), "::ViewportMaxDepth" ).emit( 0, 1.f ) ;
    }

    void SpriteSheet::execute()
    {
      Synchronization sync      ;
      unsigned        num_cmds  ;
      
      data().profiler.start() ;
      
      // Grab the synchronization from the previous device operation.
      data().sync_mutex.lock() ;
      sync = data().syncs.value() ;
      data().syncs .swap() ;
      data().sync_mutex.unlock() ;
      
      // Calculate the number of things to draw.
      num_cmds = data().stop - data().start ;
      
      // Check to make sure we have a spritesheet input to this object.
      data().checkForImage() ;
      
      // Swap the command buffer to the one to draw on. Previous one is still in flight.
      data().buffer.swap() ; 
      
      if( num_cmds != 0 && data().has_image )
      {
        data().buffer  .value().record     ( data().pass                                          ) ;
        data().pipeline.bind               ( data().buffer.value(), data().material.set           ) ;

        
        data().cmd_mutex.lock() ;
        data().buffer.value().drawInstanced( data().vertices.buffer(), 6, num_cmds, 0, 0, data().start ) ;
        data().buffer.value().stop         (                                                        ) ;
        data().pass.submit                 ( sync, data().buffer.value(), data().buffer.current()   ) ;
        data().cmd_mutex.unlock() ;
      }
      else
      {
        data().buffer.value().record( data().pass ) ;
        data().buffer.value().stop()                ;
        data().pass.submit( sync, data().buffer.value(), data().buffer.current() ) ;
      }
      data().output( sync ) ;
      
      data().profiler.stop() ;
      if( data().debug ) karma::log::Log::output( this->name(), " CPU Time: ", data().profiler.output(), "ms" ) ;
    }

    SpriteSheetData& SpriteSheet::data()
    {
      return *this->data_2d ;
    }

    const SpriteSheetData& SpriteSheet::data() const
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
  return "SpriteSheet" ;
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
  return new ::kgl::vk::SpriteSheet() ;
}

/** Exported function to destroy an instance of this module.
 * @param module A Pointer to a Module object that is of this type.
 */
exported_function void destroy( ::kgl::vk::Module* module )
{
  ::kgl::vk::SpriteSheet* mod ;
  
  mod = dynamic_cast<::kgl::vk::SpriteSheet*>( module ) ;
  delete mod ;
}

