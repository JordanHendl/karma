#include "Combiner.h"
#include "Bus.h"
#include "Signal.h"
#include <log/Log.h>
#include <profiling/Timer.h>
#include "vk/context/Window.h"
#include "vk/context/Swapchain.h"
#include "vk/context/RenderPass.h"
#include "vk/context/Descriptor.h"
#include "vk/context/Buffer.h"
#include <vk/context/Synchronization.h>
#include <vk/context/Context.h>
#include <vk/context/Image.h>
#include <vk/context/CommandBuffer.h>
#include <vk/context/Uniform.h>
#include <vk/context/Pipeline.h>
#include <containers/BufferedStack.h>
#include <containers/List.h>
#include <containers/Layered.h>
#include <vulkan/vulkan.hpp>
#include <string>

#include <sstream>
#include <iostream>
#include <map>
#include<mutex>
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
    struct CombinerData
    { 
      static constexpr unsigned BUFFERS = 3 ;
      typedef ::kgl::BufferedStack<::kgl::vk::Image, BUFFERS              > ImageStack ;
      typedef std::vector<::kgl::vk::Uniform                              > Uniforms   ;
      typedef kgl::containers::Layered<Synchronization, 3                 > Syncs      ;
      typedef kgl::containers::Layered<::kgl::vk::render::CommandBuffer, 3> CmdBuffs   ;
      
      std::queue<::kgl::vk::DescriptorSet> sets                  ;
      Syncs                                syncs                 ;
      Uniforms                             uniforms              ; ///< TODO
      ImageStack                           stack                 ; ///< TODO
      CmdBuffs                             buffers               ; ///< TODO
      karma::ms::Timer                     profiler              ; ///< TODO
      ::kgl::vk::DescriptorPool            pool                  ; ///< TODO
      ::kgl::vk::render::Context           context               ; ///< TODO
      ::kgl::vk::render::Pipeline          pipeline              ; ///< TODO
      ::kgl::vk::RenderPass                pass                  ; ///< TODO
      ::kgl::vk::Buffer                    vertices              ; ///< TODO
      ::kgl::vk::Buffer                    indices               ; ///< TODO
      ::data::module::Bus                  bus                   ; ///< TODO
      unsigned                             gpu                   ; ///< TODO
      ::vk::Device                         device                ; ///< TODO
      std::string                          window_name           ; ///< TODO
      std::string                          output_name           ; ///< TODO
      std::string                          install_path          ; ///< TODO
      std::string                          name                  ;
      std::string                          output_image_name     ; ///< The name of this object's framebuffer output.
      ::kgl::vk::Image                     images[10]            ; ///< Images to combine.
      bool                                 found_input[10]       ; ///< 
      bool                                 found_image_input[10] ; ///< 
      unsigned                             num_inputs            ; ///< The current number of inputs this module is expecting.
      std::mutex                           sync_mutex            ; ///< The lock to make sure input is recieved one at a time.
      std::mutex                           image_mutex[10]       ; ///< The lock to make sure input is recieved one at a time.
      unsigned                             current_command       ; ///< The current command being processed.
      bool                                 debug                 ;

      /** Default Constructor. Initializes member data.
       */
      CombinerData() ;
      
      /** Method to set this object's window's name.
       * @param name The name of window this object uses.
       */
      void setWindowName( const char* name ) ;
      
      /** Method to reset the input found flags.
       */
      void resetFound() ;
      
      /**
       * @param val
       */
      void setDebug( bool val ) ;

      /** Method to set the GPU this object uses for operations.
       */
      void setGPU( unsigned gpu ) ;

      /** Method to set this object's output framebuffer name.
       * @param output The name of this object's framebuffer output.
       */
      void setOutputImageName( const char* output ) ;
      
      /** Method to output this object's data.
       */
      void output( const Synchronization& sync ) ;

      /** Method to set the name of this object's synchronization output.
       */
      void setOutputName( const char* name ) ;
    };
    
    CombinerData::CombinerData()
    {
      this->num_inputs           = 0     ;
      this->found_input      [0] = false ;
      this->found_input      [1] = false ;
      this->found_input      [2] = false ;
      this->found_input      [3] = false ;
      this->found_input      [4] = false ;
      this->found_input      [5] = false ;
      this->found_input      [6] = false ;
      this->found_input      [7] = false ;
      this->found_input      [8] = false ;
      this->found_input      [9] = false ;
      this->found_image_input[0] = false ;
      this->found_image_input[1] = false ;
      this->found_image_input[2] = false ;
      this->found_image_input[3] = false ;
      this->found_image_input[4] = false ;
      this->found_image_input[5] = false ;
      this->found_image_input[6] = false ;
      this->found_image_input[7] = false ;
      this->found_image_input[8] = false ;
      this->found_image_input[9] = false ;
      this->debug                = false ;
    }
    
    void CombinerData::resetFound() 
    {
      this->found_input      [0] = false ;
      this->found_input      [1] = false ;
      this->found_input      [2] = false ;
      this->found_input      [3] = false ;
      this->found_input      [4] = false ;
      this->found_input      [5] = false ;
      this->found_input      [6] = false ;
      this->found_input      [7] = false ;
      this->found_input      [8] = false ;
      this->found_input      [9] = false ;
      this->found_image_input[0] = false ;
      this->found_image_input[1] = false ;
      this->found_image_input[2] = false ;
      this->found_image_input[3] = false ;
      this->found_image_input[4] = false ;
      this->found_image_input[5] = false ;
      this->found_image_input[6] = false ;
      this->found_image_input[7] = false ;
      this->found_image_input[8] = false ;
      this->found_image_input[9] = false ;
    }
    
    void CombinerData::setDebug( bool val )
    {
      this->debug = val ;
    }

    void CombinerData::setOutputImageName( const char* output ) 
    {
      this->output_image_name = output ;
    }

    void CombinerData::setWindowName( const char* name )
    {
      this->window_name = name ;
    }

    void CombinerData::setGPU( unsigned gpu )
    {
      this->gpu    = gpu                                ;
      this->device = this->context.virtualDevice( gpu ) ;
    }
    
    void Combiner::inputImageOne( const ::kgl::vk::Image& image )
    {
      while( data().found_input[ 0 ] ){} ;

      data().image_mutex[ 0 ].lock() ;
      data().images           [ 0 ] = image ;
      data().found_image_input[ 0 ] = true  ;
      this->semIncrement() ;
      data().image_mutex[ 0 ].unlock() ;
    }
    
    void Combiner::inputImageTwo( const ::kgl::vk::Image& image )
    {
      while( data().found_input[ 1 ] ){} ;

      data().image_mutex[ 1 ].lock() ;
      data().images           [ 1 ] = image ;
      data().found_image_input[ 1 ] = true  ;
      this->semIncrement() ;
      data().image_mutex[ 1 ].unlock() ;
    }
    
    void Combiner::inputImageThree( const ::kgl::vk::Image& image )
    {
      while( data().found_input[ 2 ] ){} ;

      data().image_mutex[ 2 ].lock() ;
      data().images           [ 2 ] = image ;
      data().found_image_input[ 2 ] = true  ;
      this->semIncrement() ;
      data().image_mutex[ 2 ].unlock() ;
    }
    
    void Combiner::inputImageFour( const ::kgl::vk::Image& image )
    {
      data().image_mutex[ 3 ].lock() ;
      data().images           [ 3 ] = image ;
      data().found_image_input[ 3 ] = true  ;
      this->semIncrement() ;
      data().image_mutex[ 3 ].unlock() ;
    }
    
    void Combiner::inputImageFive( const ::kgl::vk::Image& image )
    {
      data().image_mutex[ 4 ].lock() ;
      data().images           [ 4 ] = image ;
      data().found_image_input[ 4 ] = true  ;
      this->semIncrement() ;
      data().image_mutex[ 4 ].unlock() ;
    }
    
    void Combiner::inputImageSix( const ::kgl::vk::Image& image )
    {
      data().image_mutex[ 5 ].lock() ;
      data().images           [ 5 ] = image ;
      data().found_image_input[ 5 ] = true  ;
      this->semIncrement() ;
      data().image_mutex[ 5 ].unlock() ;
    }
    
    void Combiner::inputImageSeven( const ::kgl::vk::Image& image )
    {
      data().image_mutex[ 6 ].lock() ;
      data().images           [ 6 ] = image ;
      data().found_image_input[ 6 ] = true  ;
      this->semIncrement() ;
      data().image_mutex[ 6 ].unlock() ;
    }
    
    void Combiner::inputImageEight( const ::kgl::vk::Image& image )
    {
      data().image_mutex[ 0 ].lock() ;
      data().images           [ 7 ] = image ;
      data().found_image_input[ 7 ] = true  ;
      this->semIncrement() ;
      data().image_mutex[ 0 ].unlock() ;
    }
    
    void Combiner::inputImageNine( const ::kgl::vk::Image& image )
    {
      data().image_mutex[ 0 ].lock() ;
      data().images           [ 8 ] = image ;
      data().found_image_input[ 8 ] = true  ;
      this->semIncrement() ;
      data().image_mutex[ 0 ].unlock() ;
    }
    
    void Combiner::inputImageTen( const ::kgl::vk::Image& image )
    {
      data().image_mutex[ 0 ].lock() ;
      data().images           [ 9 ] = image ;
      data().found_image_input[ 9 ] = true  ;
      this->semIncrement() ;
      data().image_mutex[ 0 ].unlock() ;
    }

    void Combiner::inputOne( const ::kgl::vk::Synchronization& sync )
    {
      while( data().found_input[ 0 ] ) {} ;
      
      data().sync_mutex.lock() ;
      data().syncs.value().addWait( sync.signalSem( 0 ) ) ;
      data().found_input[ 0 ] = true ;
      this->semIncrement() ;
      data().sync_mutex.unlock() ;
    }
    
    void Combiner::inputTwo( const ::kgl::vk::Synchronization& sync )
    {
      while( data().found_input[ 1 ] ) {} ;
      
      data().sync_mutex.lock() ;
      data().syncs.value().addWait( sync.signalSem( 0 ) ) ;
      data().found_input[ 1 ] = true ;
      this->semIncrement() ;
      data().sync_mutex.unlock() ;
    }
    
    void Combiner::inputThree( const ::kgl::vk::Synchronization& sync )
    {
      while( data().found_input[ 02] ) {} ;
      
      data().sync_mutex.lock() ;
      data().syncs.value().addWait( sync.signalSem( 0 ) ) ;
      data().found_input[ 2 ] = true ;
      this->semIncrement() ;
      data().sync_mutex.unlock() ;
    }
    
    void Combiner::inputFour( const ::kgl::vk::Synchronization& sync )
    {
      data().sync_mutex.lock() ;
      data().syncs.value().addWait( sync.signalSem( 0 ) ) ;
      this->semIncrement() ;
      data().sync_mutex.unlock() ;
    }
    
    void Combiner::inputFive( const ::kgl::vk::Synchronization& sync )
    {
      data().sync_mutex.lock() ;
      data().syncs.value().addWait( sync.signalSem( 0 ) ) ;
      this->semIncrement() ;
      data().sync_mutex.unlock() ;
    }
    
    void Combiner::inputSix( const ::kgl::vk::Synchronization& sync )
    {
      data().sync_mutex.lock() ;
      data().syncs.value().addWait( sync.signalSem( 0 ) ) ;
      this->semIncrement() ;
      data().sync_mutex.unlock() ;
    }
    
    void Combiner::inputSeven( const ::kgl::vk::Synchronization& sync )
    {
      data().sync_mutex.lock() ;
      data().syncs.value().addWait( sync.signalSem( 0 ) ) ;
      this->semIncrement() ;
      data().sync_mutex.unlock() ;
    }
    
    void Combiner::inputEight( const ::kgl::vk::Synchronization& sync )
    {
      data().sync_mutex.lock() ;
      data().syncs.value().addWait( sync.signalSem( 0 ) ) ;
      this->semIncrement() ;
      data().sync_mutex.unlock() ;
    }
    
    void Combiner::inputNine( const ::kgl::vk::Synchronization& sync )
    {
      data().sync_mutex.lock() ;
      data().syncs.value().addWait( sync.signalSem( 0 ) ) ;
      this->semIncrement() ;
      data().sync_mutex.unlock() ;
    }
    
    void Combiner::inputTen( const ::kgl::vk::Synchronization& sync )
    {
      data().sync_mutex.lock() ;
      data().syncs.value().addWait( sync.signalSem( 0 ) ) ;
      this->semIncrement() ;
      data().sync_mutex.unlock() ;
    }

    void CombinerData::output( const Synchronization& sync )
    {
      const unsigned index = this->context.currentSwap( this->window_name.c_str() ) ;
     
      this->bus( this->output_image_name.c_str() ).emit( this->pass.image( index ) ) ;
      this->bus( this->output_name      .c_str() ).emit( sync                      ) ;
    }

    void Combiner::setInputNameOne( const char* name )
    {
      // We now know what input we're getting. Subscribe & add as a dependancy.
      if( !data().found_input[0] )
      {
        data().bus( name ).attach( this, &Combiner::inputOne ) ;
        data().num_inputs++ ;
        data().found_input[0] = true ;
      }
    }
    
    void Combiner::setInputNameTwo( const char* name )
    {
      // We now know what input we're getting. Subscribe & add as a dependancy.
      if( !data().found_input[1] )
      {
        data().bus( name ).attach( this, &Combiner::inputTwo ) ;
        data().num_inputs++ ;
        data().found_input[1] = true ;
      }
    }

    void Combiner::setInputNameThree( const char* name )
    {
      // We now know what input we're getting. Subscribe & add as a dependancy.
      if( !data().found_input[2] )
      {
        data().bus( name ).attach( this, &Combiner::inputThree ) ;
        data().num_inputs++ ;
        data().found_input[2] = true ;
      }
    }

    void Combiner::setInputNameFour( const char* name )
    {
      // We now know what input we're getting. Subscribe & add as a dependancy.
      if( !data().found_input[3] )
      {
        data().bus( name ).attach( this, &Combiner::inputFour ) ;
        data().num_inputs++ ;
        data().found_input[3] = true ;
      }
    }

    void Combiner::setInputNameFive( const char* name )
    {
      // We now know what input we're getting. Subscribe & add as a dependancy.
      if( !data().found_input[4] )
      {
        data().bus( name ).attach( this, &Combiner::inputFive ) ;
        data().num_inputs++ ;
        data().found_input[4] = true ;
      }
    }

    void Combiner::setInputNameSix( const char* name )
    {
      // We now know what input we're getting. Subscribe & add as a dependancy.
      if( !data().found_input[5] )
      {
        data().bus( name ).attach( this, &Combiner::inputSix ) ;
        data().num_inputs++ ;
        data().found_input[5] = true ;
      }
    }

    void Combiner::setInputNameSeven( const char* name )
    {
      // We now know what input we're getting. Subscribe & add as a dependancy.
      if( !data().found_input[6] )
      {
        data().bus( name ).attach( this, &Combiner::inputSeven ) ;
        data().num_inputs++ ;
        data().found_input[6] = true ;
      }
    }

    void Combiner::setInputNameEight( const char* name )
    {
      // We now know what input we're getting. Subscribe & add as a dependancy.
      if( !data().found_input[7] )
      {
        data().bus( name ).attach( this, &Combiner::inputEight ) ;
        data().num_inputs++ ;
        data().found_input[7] = true ;
      }
    }

    void Combiner::setInputNameNine( const char* name )
    {
      // We now know what input we're getting. Subscribe & add as a dependancy.
      if( !data().found_input[8] )
      {
        data().bus( name ).attach( this, &Combiner::inputNine ) ;
        data().num_inputs++ ;
        data().found_input[8] = true ;
      }
    }

    void Combiner::setInputNameTen( const char* name )
    {
      // We now know what input we're getting. Subscribe & add as a dependancy.
      if( !data().found_input[9] )
      {
        data().bus( name ).attach( this, &Combiner::inputTen ) ;
        data().num_inputs++ ;
        data().found_input[9] = true ;
      }
    }
    
    void Combiner::setInputImageNameOne( const char* name )
    {
      // We now know what input we're getting. Subscribe & add as a dependancy.
      if( !data().found_image_input[0] )
      {
        data().bus( name ).attach( this, &Combiner::inputImageOne ) ;
        data().num_inputs++ ;
        data().found_image_input[0] = true ;
      }
    }
    
    void Combiner::setInputImageNameTwo( const char* name )
    {
      // We now know what input we're getting. Subscribe & add as a dependancy.
      if( !data().found_image_input[1] )
      {
        data().bus( name ).attach( this, &Combiner::inputImageTwo ) ;
        data().num_inputs++ ;
        data().found_image_input[1] = true ;
      }
    }
    
    void Combiner::setInputImageNameThree( const char* name )
    {
      // We now know what input we're getting. Subscribe & add as a dependancy.
      if( !data().found_image_input[2] )
      {
        data().bus( name ).attach( this, &Combiner::inputImageThree ) ;
        data().num_inputs++ ;
        data().found_image_input[2] = true ;
      }
    }
    
    void Combiner::setInputImageNameFour( const char* name )
    {
      // We now know what input we're getting. Subscribe & add as a dependancy.
      if( !data().found_image_input[3] )
      {
        data().bus( name ).attach( this, &Combiner::inputImageFour ) ;
        data().num_inputs++ ;
        data().found_image_input[3] = true ;
      }
    }
    
    void Combiner::setInputImageNameFive( const char* name )
    {
      // We now know what input we're getting. Subscribe & add as a dependancy.
      if( !data().found_image_input[4] )
      {
        data().bus( name ).attach( this, &Combiner::inputImageFive ) ;
        data().num_inputs++ ;
        data().found_image_input[4] = true ;
      }
    }
    
    void Combiner::setInputImageNameSix( const char* name )
    {
      // We now know what input we're getting. Subscribe & add as a dependancy.
      if( !data().found_image_input[5] )
      {
        data().bus( name ).attach( this, &Combiner::inputImageSix ) ;
        data().num_inputs++ ;
        data().found_image_input[5] = true ;
      }
    }
    
    void Combiner::setInputImageNameSeven( const char* name )
    {
      // We now know what input we're getting. Subscribe & add as a dependancy.
      if( !data().found_image_input[6] )
      {
        data().bus( name ).attach( this, &Combiner::inputImageSeven ) ;
        data().num_inputs++ ;
        data().found_image_input[6] = true ;
      }
    }
    
    void Combiner::setInputImageNameEight( const char* name )
    {
      // We now know what input we're getting. Subscribe & add as a dependancy.
      if( !data().found_image_input[7] )
      {
        data().bus( name ).attach( this, &Combiner::inputImageEight ) ;
        data().num_inputs++ ;
        data().found_image_input[7] = true ;
      }
    }
    
    void Combiner::setInputImageNameNine( const char* name )
    {
      // We now know what input we're getting. Subscribe & add as a dependancy.
      if( !data().found_image_input[8] )
      {
        data().bus( name ).attach( this, &Combiner::inputImageNine ) ;
        data().num_inputs++ ;
        data().found_image_input[8] = true ;
      }
    }
    
    void Combiner::setInputImageNameTen( const char* name )
    {
      // We now know what input we're getting. Subscribe & add as a dependancy.
      if( !data().found_image_input[9] )
      {
        data().bus( name ).attach( this, &Combiner::inputImageTen ) ;
        data().num_inputs++ ;
        data().found_image_input[9] = true ;
      }
    }

    void CombinerData::setOutputName( const char* name )
    {
      this->output_name = name ;
    }

    Combiner::Combiner()
    {
      this->data_2d = new CombinerData() ;
    }
    
    Combiner::~Combiner()
    {
      delete this->data_2d ;
    }

    void Combiner::resize()
    {
      const unsigned     width    = data().context.width ( data().window_name.c_str() ) ; ///< Width of the screen.
      const unsigned     height   = data().context.height( data().window_name.c_str() ) ; ///< Height of the screen.
      static const char* path     = "/uwu/combine.uwu"                                  ; ///< Path to this object's shader in the local-directory.
      std::string pipeline_path ;
      
      pipeline_path = ::kgl::vk::basePath() ;
      pipeline_path = pipeline_path + path  ;
      
      data().pass              .reset() ;
      data().buffers.seek ( 0 ).reset() ; 
      data().buffers.seek ( 1 ).reset() ; 
      data().buffers.seek ( 2 ).reset() ; 
      data().pipeline          .reset() ; 
              
      // Initialize vulkan objects.
      data().pass             .initialize( data().window_name.c_str(), data().gpu                                                 ) ;
      data().buffers.seek( 0 ).initialize( data().window_name.c_str(), data().gpu, data().pass.numBuffers(), BufferLevel::Primary ) ;
      data().buffers.seek( 1 ).initialize( data().window_name.c_str(), data().gpu, data().pass.numBuffers(), BufferLevel::Primary ) ;
      data().buffers.seek( 2 ).initialize( data().window_name.c_str(), data().gpu, data().pass.numBuffers(), BufferLevel::Primary ) ;
      data().pipeline         .initialize( pipeline_path.c_str(), data().gpu, width, height, data().pass.pass()                   ) ;
    }

    void Combiner::initialize()
    {
      static const unsigned MAX_SETS  = 200 ;
      const char*       path          = "/uwu/combine.uwu"                                  ;
      const std::string pipeline_path = data().install_path + path                          ;
      const unsigned     width        = data().context.width ( data().window_name.c_str() ) ;
      const unsigned     height       = data().context.height( data().window_name.c_str() ) ;
      
      this->setNumDependancies( data().num_inputs ) ;
      data().uniforms.resize( data().num_inputs / 2 ) ;

      for( unsigned i = 0; i < data().num_inputs / 2; i++ )
      {
        data().images  [ i ].initialize( data().gpu, width, height ) ;
        data().uniforms[ i ].initialize( data().gpu                ) ;
      }
      
      data().pass.setImageFinalLayout( ::vk::ImageLayout::eGeneral ) ;
      data().profiler       .initialize          ( ""                                   ) ;
      data().syncs.seek( 0 ).initialize          ( data().gpu                           ) ;
      data().syncs.seek( 1 ).initialize          ( data().gpu                           ) ;
      data().syncs.seek( 2 ).initialize          ( data().gpu                           ) ;
      data().vertices       .initialize<float   >( data().gpu, Buffer::Type::VERTEX, 16 ) ;
      data().indices        .initialize<unsigned>( data().gpu, Buffer::Type::INDEX , 6  ) ;
      
//      data().pass.setImageFinalLayout( ::vk::ImageLayout::eColorAttachmentOptimal ) ;
      data().pass             .initialize( data().window_name.c_str(), data().gpu                                                 ) ;
      data().buffers.seek( 0 ).initialize( data().window_name.c_str(), data().gpu, data().pass.numBuffers(), BufferLevel::Primary ) ;
      data().buffers.seek( 1 ).initialize( data().window_name.c_str(), data().gpu, data().pass.numBuffers(), BufferLevel::Primary ) ;
      data().buffers.seek( 2 ).initialize( data().window_name.c_str(), data().gpu, data().pass.numBuffers(), BufferLevel::Primary ) ;
      data().pipeline         .initialize( pipeline_path.c_str(), data().gpu, width, height, data().pass.pass()                   ) ;
      data().pool             .initialize( data().gpu, MAX_SETS, data().pipeline.shader()                                         ) ;

      data().vertices.copyToDevice( vertex_data ) ;
      data().indices .copyToDevice( index_data  ) ;
      data().resetFound() ;
    }

    void Combiner::shutdown()
    {
    }

    void Combiner::subscribe( const char* pipeline, unsigned id )
    {
      const std::string json_path = std::string( "Graphs::" ) + std::string( pipeline ) + std::string( "::Modules::" ) + std::string( this->name() ) ;
      data().pass.subscribe( this->name(), id )   ;
      data().name         = this->name()          ;
      data().install_path = ::kgl::vk::basePath() ;
      
      data().pipeline.subscribe( json_path.c_str(), id ) ;
      // Graph-specific JSON-parameters.
      data().bus( "Graphs::", pipeline, "::window" ).attach( this->data_2d, &CombinerData::setWindowName ) ;
      data().bus( "Graphs::", pipeline, "::gpu"    ).attach( this->data_2d, &CombinerData::setGPU        ) ;
      
      // Module-specific JSON-parameters.
      data().bus( json_path.c_str(), "::input_one"         ).attach( this         , &Combiner::setInputNameOne        ) ;
      data().bus( json_path.c_str(), "::input_two"         ).attach( this         , &Combiner::setInputNameTwo        ) ;
      data().bus( json_path.c_str(), "::input_three"       ).attach( this         , &Combiner::setInputNameThree      ) ;
      data().bus( json_path.c_str(), "::input_four"        ).attach( this         , &Combiner::setInputNameFour       ) ;
      data().bus( json_path.c_str(), "::input_five"        ).attach( this         , &Combiner::setInputNameFive       ) ;
      data().bus( json_path.c_str(), "::input_six"         ).attach( this         , &Combiner::setInputNameSix        ) ;
      data().bus( json_path.c_str(), "::input_seven"       ).attach( this         , &Combiner::setInputNameSeven      ) ;
      data().bus( json_path.c_str(), "::input_eight"       ).attach( this         , &Combiner::setInputNameEight      ) ;
      data().bus( json_path.c_str(), "::input_nine"        ).attach( this         , &Combiner::setInputNameNine       ) ;
      data().bus( json_path.c_str(), "::input_ten"         ).attach( this         , &Combiner::setInputNameTen        ) ;
      data().bus( json_path.c_str(), "::input_image_one"   ).attach( this         , &Combiner::setInputImageNameOne   ) ;
      data().bus( json_path.c_str(), "::input_image_two"   ).attach( this         , &Combiner::setInputImageNameTwo   ) ;
      data().bus( json_path.c_str(), "::input_image_three" ).attach( this         , &Combiner::setInputImageNameThree ) ;
      data().bus( json_path.c_str(), "::input_image_four"  ).attach( this         , &Combiner::setInputImageNameFour  ) ;
      data().bus( json_path.c_str(), "::input_image_five"  ).attach( this         , &Combiner::setInputImageNameFive  ) ;
      data().bus( json_path.c_str(), "::input_image_six"   ).attach( this         , &Combiner::setInputImageNameSix   ) ;
      data().bus( json_path.c_str(), "::input_image_seven" ).attach( this         , &Combiner::setInputImageNameSeven ) ;
      data().bus( json_path.c_str(), "::input_image_eight" ).attach( this         , &Combiner::setInputImageNameEight ) ;
      data().bus( json_path.c_str(), "::input_image_nine"  ).attach( this         , &Combiner::setInputImageNameNine  ) ;
      data().bus( json_path.c_str(), "::input_image_ten"   ).attach( this         , &Combiner::setInputImageNameTen   ) ;
      data().bus( json_path.c_str(), "::output"            ).attach( this->data_2d, &CombinerData::setOutputName      ) ;
      data().bus( json_path.c_str(), "::output_image"      ).attach( this->data_2d, &CombinerData::setOutputImageName ) ;
      data().bus( json_path.c_str(), "::debug"             ).attach( this->data_2d, &CombinerData::setDebug           ) ;
      
      data().bus( json_path.c_str(), "::ViewportX"        ).emit( 0, 0.f ) ;
      data().bus( json_path.c_str(), "::ViewportY"        ).emit( 0, 0.f ) ;
      data().bus( json_path.c_str(), "::ViewportMinDepth" ).emit( 0, 0.f ) ;
      data().bus( json_path.c_str(), "::ViewportMaxDepth" ).emit( 0, 1.f ) ;
    }

    void Combiner::execute()
    {
      ::kgl::vk::DescriptorSet set        ;
      ::kgl::vk::Image         images[10] ;
      Synchronization          sync       ;

      data().profiler.start() ;
      
      data().sync_mutex.lock() ;
      
      sync = data().syncs.value() ;
      data().syncs.value().clear() ;
      data().syncs.swap() ;

      data().sync_mutex.unlock() ;

      for( unsigned i = 0; i < data().num_inputs / 2; i++ )
      {
        data().image_mutex[ i ].lock() ;
        images[ i ] = ( data().images[ i ] ) ;
        data().image_mutex[ i ].unlock() ;
      }
      
      data().resetFound() ;
      
      data().buffers.value().record( data().pass ) ;
      for( int i = ( data().num_inputs / 2 ) - 1;  i >= 0; i-- )
      {
        set = data().pool.makeDescriptorSet( data().pass.numBuffers() ) ;
        data().uniforms[ i ].addImage( "framebuffer", data().images[ i ] ) ;
        set.set( data().uniforms[ i ] ) ;
        data().pipeline.bind( data().buffers.value(), set ) ;
        data().buffers.value().drawIndexed( 6, data().indices.buffer(), data().vertices.buffer() ) ;
        data().sets.push( set ) ;
        
        if( data().sets.size() > 20 )
        {
          data().sets.front().reset() ;
          data().sets.pop() ;
        }
      }

      data().buffers.value().stop() ;
      data().pass.submit( sync, data().buffers.value(), data().buffers.current() ) ;
      data().buffers.swap() ;
      
      // Output our synchronization to next module in the graph & reset command index.
      data().output( sync ) ;
      
      data().profiler.stop() ;
      if( data().debug ) karma::log::Log::output( this->name(), " CPU Time: ", data().profiler.output(), "ms" ) ;
    }

    CombinerData& Combiner::data()
    {
      return *this->data_2d ;
    }

    const CombinerData& Combiner::data() const
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
  return "Combiner" ;
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
  return new ::kgl::vk::Combiner() ;
}

/** Exported function to destroy an instance of this module.
 * @param module A Pointer to a Module object that is of this type.
 */
exported_function void destroy( ::kgl::vk::Module* module )
{
  ::kgl::vk::Combiner* mod ;
  
  mod = dynamic_cast<::kgl::vk::Combiner*>( module ) ;
  delete mod ;
}

