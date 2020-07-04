#include "casper.h"
#include "window/casper_library.h"
#include "casper_vulkan_descriptor_pool.h"
#include <casper_vulkan.h>
#include <casper_vulkan_window.h>
#include <casper_vulkan_surface.h>
#include <casper_vulkan_pipeline.h>
#include <casper_vulkan_swapchain.h>
#include <casper_vulkan_image.h>
#include <casper_vulkan_device.h>
#include <casper_vulkan_command_buffer.h>
#include <casper_vulkan_semaphore.h>
#include <casper_vulkan_buffer.h>
#include <Bus.h>
#include <Signal.h>
#include <iostream>
#include <SDL2/SDL_events.h>

struct vec2
{
  float x ;
  float y ;
};

const unsigned GPU_ID = 0 ;

static ::casper::vulkan::Window        window         ;
static ::casper::vulkan::Pipeline      pipeline       ;
static ::casper::vulkan::CommandBuffer cmd_buff       ;
static ::casper::vulkan::Buffer        vertex_buffer  ;
static ::casper::vulkan::Buffer        index_buffer   ;
static ::casper::vulkan::Buffer        uniform_buffer ;
static ::casper::vulkan::Image         image          ;
static ::data::module::Bus             bus            ;

static bool running = true ;
static vec2 pos = { 0.f, 0.f } ;

static void handleEvent( const SDL_Event& e )
{
  if( e.type == SDL_QUIT ) running = false ;
  
  if( e.type == SDL_KEYDOWN )
  {
    switch( e.key.keysym.sym )
    {
      case SDLK_UP     : pos.y -= 0.01   ; break ;
      case SDLK_DOWN   : pos.y += 0.01   ; break ;
      case SDLK_LEFT   : pos.x -= 0.01   ; break ;
      case SDLK_RIGHT  : pos.x += 0.01   ; break ;
      case SDLK_ESCAPE : running = false ; break ;
      default      : break ;
    }
  }
}

const float vertices[] = 
{ 
  // positions     // texture coords
 -0.5f, -0.5f,  1.0f, 1.0f, // top right
  0.5f, -0.5f,  1.0f, 0.0f, // bottom right
  0.5f,  0.5f,  0.0f, 0.0f, // bottom left
 -0.5f,  0.5f,  0.0f, 1.0f  // top left
};

 const unsigned int indices[] = 
 {
   0, 1, 3, // first triangle
   1, 2, 3  // second triangle
 };


int main( int argc, char** argv )
{
  // Initialize Subsystems.
  casper::casperInit()                       ;
  casper::library::initializeAPI()           ;
  pipeline.subscribe( "2D_image_shader", 1 ) ;
  
  // Attach for input handling.
  bus[ "key_input" ].attach( &::handleEvent ) ;

  // Initialize Vulkan Objects.
  window        .initialize          ( "casper", GPU_ID, 1280, 1024                    ) ; 
  pipeline      .initialize          ( "/usr/local/karma/shaders/shader.uwu", window   ) ;
  image         .initialize          ( 0, "/usr/local/karma/data/images/test.jpeg"     ) ;
  vertex_buffer .initialize<float   >( GPU_ID, casper::vulkan::BufferType::VERTEX , 16 ) ;
  uniform_buffer.initialize<vec2    >( GPU_ID, casper::vulkan::BufferType::UNIFORM, 1  ) ;
  index_buffer  .initialize<unsigned>( GPU_ID, casper::vulkan::BufferType::INDEX  , 6  ) ;
  cmd_buff      .initRender          ( window                                          ) ;

  // Copy data to GPU.
  vertex_buffer  .copyToDevice ( vertices                  ) ;
  index_buffer   .copyToDevice ( indices                   ) ;
  uniform_buffer .copyToDevice ( pos                       ) ;
  pipeline     .set().setImage ( image         , "image"   ) ;
  pipeline     .set().setBuffer( uniform_buffer, "offsets" ) ;

  // Record commands to command buffer.
  cmd_buff     .record()                                   ;
  pipeline     .bind( cmd_buff                           ) ;
  cmd_buff     .drawIndexed( index_buffer, vertex_buffer ) ;
  cmd_buff     .stop()                                     ;

  // Render loop.
  while( running )
  {
    // Update Uniform every frame for movement.
    uniform_buffer.copyToDevice( pos ) ;

    window.pollEvents() ; //Deprecated 
    window.submit( cmd_buff ) ;
    window.present() ;
  }  

  return 0 ;
}

