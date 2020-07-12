//#include "casper.h"
//#include "window/casper_library.h"
//#include "casper_vulkan_descriptor_pool.h"
//#include <casper_vulkan.h>
////#include <Window.h>
////#include <Surface.h>
//#include <casper_vulkan_pipeline.h>
////#include <Swapchain.h>
//#include <casper_vulkan_image.h>
////#include <Device.h>
//#include <casper_vulkan_command_buffer.h>
//#include <casper_vulkan_semaphore.h>
//#include <casper_vulkan_buffer.h>
//#include <Bus.h>
//#include <Signal.h>
//#include <iostream>
//#include <SDL2/SDL_events.h>
//#include <vector>
//
//struct vec2
//{
//  float x ;
//  float y ;
//};
//
//const unsigned GPU_ID = 0 ;
//
//static ::casper::vulkan::Window        window           ;
//static ::casper::vulkan::Pipeline      pipeline         ;
//static ::casper::vulkan::Pipeline      compute_pipeline ;
//static ::casper::vulkan::CommandBuffer cmd_buff         ;
//static ::casper::vulkan::CommandBuffer compute_buff     ;
//static ::casper::vulkan::Buffer        vertex_buffer    ;
//static ::casper::vulkan::Buffer        index_buffer     ;
//static ::casper::vulkan::Buffer        compute_buffer   ;
//static ::casper::vulkan::Buffer        uniform_buffer_2 ;
//static ::casper::vulkan::Buffer        uniform_buffer   ;
//static ::casper::vulkan::Image         image            ;
//static ::data::module::Bus             bus              ;
//static std::vector<float>              compute_data     ;
//
//static bool running = true ;
//static vec2 pos = { 0.f, 0.f } ;
//
//static void handleEvent( const SDL_Event& e )
//{
//  if( e.type == SDL_QUIT ) running = false ;
//  
//  if( e.type == SDL_KEYDOWN )
//  {
//    switch( e.key.keysym.sym )
//    {
//      case SDLK_UP     : pos.y -= 0.01   ; break ;
//      case SDLK_DOWN   : pos.y += 0.01   ; break ;
//      case SDLK_LEFT   : pos.x -= 0.01   ; break ;
//      case SDLK_RIGHT  : pos.x += 0.01   ; break ;
//      case SDLK_ESCAPE : running = false ; break ;
//      default          :                   break ;
//    }
//    
//    uniform_buffer.copyToDevice( pos ) ;
//  }
//}
//
//static void handleCompute()
//{
//  ::casper::vulkan::Buffer uni_buff_1 ;
//  ::casper::vulkan::Buffer uni_buff_2 ;
//
//  uni_buff_1    .initialize<unsigned>( GPU_ID, casper::vulkan::BufferType::UNIFORM, 1                  , true ) ;
//  uni_buff_2    .initialize<unsigned>( GPU_ID, casper::vulkan::BufferType::UNIFORM, 1                  , true ) ;
//  compute_buffer.initialize<float   >( GPU_ID, casper::vulkan::BufferType::SSBO   , compute_data.size(), true ) ;
//
//  compute_pipeline.initialize           ( "/usr/local/karma/shaders/compute.uwuc", window,
//                                          casper::vulkan::Pipeline::PipelineType::COMPUTE   ) ;  
//
//  uni_buff_1      .copyToDevice   ( 32                        ) ;
//  uni_buff_2      .copyToDevice   ( 32                        ) ;
//  compute_buffer  .copyToDevice   ( compute_data.data()       ) ;
//  compute_pipeline.set().setBuffer( compute_buffer, "data"    ) ;
//  compute_pipeline.set().setBuffer( uni_buff_1, "data_width"  ) ;
//  compute_pipeline.set().setBuffer( uni_buff_2, "data_height" ) ;
//  
////  compute_buff.record() ;
////  compute_pipeline.bind( compute_buff ) ;
////  compute_buff.stop() ;
//}
//
//const float vertices[] = 
//{ 
//  // positions     // texture coords
// -0.5f, -0.5f,  1.0f, 1.0f, // top right
//  0.5f, -0.5f,  1.0f, 0.0f, // bottom right
//  0.5f,  0.5f,  0.0f, 0.0f, // bottom left
// -0.5f,  0.5f,  0.0f, 1.0f  // top left
//};
//
// const unsigned int indices[] = 
// {
//   0, 1, 3, // first triangle
//   1, 2, 3  // second triangle
// };
//
//
//int main( int argc, char** argv )
//{
//  // Initialize Subsystems.
//  casper::casperInit()                       ;
//  casper::library::initializeAPI()           ;
//  pipeline.subscribe( "2D_image_shader", 1 ) ;
//  
//  // Attach for input handling.
//  bus[ "key_input" ].attach( &::handleEvent ) ;
//  
//  
//  // Initialize compute data.
//  compute_data.resize( 1024 ) ;
////  compute_data.assign( 1024, 0.0f ) ;
//
//  // Initialize Vulkan Objects.
//  window           .initialize           ( "casper", GPU_ID, 1280, 1024                      ) ; 
//  handleCompute() ;
//  pipeline         .initialize           ( "/usr/local/karma/shaders/shader.uwu"  , window   ) ;
//  image            .initialize          ( 0, "/usr/local/karma/data/images/test.jpeg"       ) ;
//  vertex_buffer    .initialize<float   >( GPU_ID, casper::vulkan::BufferType::VERTEX , 16   ) ;
//  uniform_buffer_2 .initialize<float   >( GPU_ID, casper::vulkan::BufferType::UNIFORM, 1    ) ;
//  uniform_buffer   .initialize<vec2    >( GPU_ID, casper::vulkan::BufferType::UNIFORM, 1    ) ;
//  index_buffer     .initialize<unsigned>( GPU_ID, casper::vulkan::BufferType::INDEX  , 6    ) ;
//  cmd_buff         .initRender          ( window                                            ) ;
//  
//  float f = 0.0f ;
//  // Copy data to GPU.
//  vertex_buffer   .copyToDevice    ( vertices                  ) ;
//  index_buffer    .copyToDevice    ( indices                   ) ;
//  uniform_buffer  .copyToDevice    ( pos                       ) ;
//  uniform_buffer_2.copyToDevice    ( f                         ) ;
//  
//  
//  pipeline     .set().setImage    ( image           , "image"   ) ;
//  pipeline     .set().setBuffer   ( uniform_buffer  , "offsets" ) ;
//  pipeline     .set().setBuffer   ( uniform_buffer_2, "colors" ) ;
//  
//  // Record commands to command buffer.
//  cmd_buff     .record()                                   ;
//  pipeline     .bind( cmd_buff                           ) ;
//  cmd_buff     .drawIndexed( index_buffer, vertex_buffer ) ;
//  cmd_buff     .stop()                                     ;
//
//  // Render loop.
//  while( running )
//  {
//    // Update Uniform every frame for movement.
//    f += 0.01f ;
//    if( f > 1.0f ) f = 0.0f ;
//    
//    uniform_buffer_2.copyToDevice( f ) ;
//    window.pollEvents() ; //Deprecated 
//    window.submit( cmd_buff ) ;
//    window.present() ;
//  }  
//
//  return 0 ;
//}
//
