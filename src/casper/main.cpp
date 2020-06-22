 
#include "casper.h"
#include "window/casper_window.h"
#include "window/casper_library.h"
#include <casper_vulkan.h>
#include <casper_vulkan_surface_manager.h>
#include <casper_vulkan_surface.h>
#include <casper_vulkan_pipeline.h>
#include <casper_vulkan_swapchain.h>
#include <casper_vulkan_device.h>
#include <casper_vulkan_2d_vertex.h>
#include <casper_vulkan_command_buffer.h>
#include <casper_vulkan_semaphore.h>
#include <casper_vulkan_buffer.h>
#include <iostream>

float vertices[] = 
{ 
  // positions     // texture coords
  1.0f,  1.0f,  1.0f, 1.0f, // top right
  1.f,   0.0f,  1.0f, 0.0f, // bottom right
  0.0f,  0.0f,  0.0f, 0.0f, // bottom left
  0.0f,  1.0f,  0.0f, 1.0f  // top left
};
   
 const unsigned int indices[] = 
 {
   0, 1, 3, // first triangle
   1, 2, 3  // second triangle
 };

casper::Window                 window ;
casper::vulkan::DeviceManager  device    ;
casper::vulkan::SurfaceManager manager   ;
casper::vulkan::SwapChain      chain     ;
casper::vulkan::Pipeline       pipeline  ;
casper::vulkan::CommandBuffer  cmd_buff  ;
casper::vulkan::Buffer<float>  buffer    ;

void drawFrame() 
{
  unsigned t = 0 ;
  while( true )
  {
    window.pollEvents() ; //Deprecated 
    manager.surface( "vulkan" ).present( cmd_buff.submit( chain ) ) ;
    t++ ;
  }
}

int main( int argc, char** argv )
{
  int image ;
  // casper::vulkan::Buffer         buff     ;

  casper::casperInit() ;
  casper::library::initializeAPI() ;

  window   .initialize( "vulkan", 0, 520, 280 ) ;
  chain    .initialize( manager.surface("vulkan"), device.device( "vulkan" ) ) ;
  pipeline .initialize( "A:/wksp/sb/vulkan_test/spirv/big_shit", chain, device.device( "vulkan" ), casper::vulkan::VertexType2D() ) ;
  cmd_buff .initialize( device.device( "vulkan" ), pipeline, chain ) ;
  buffer   .intialize( device.device("vulkan"), casper::vulkan::BufferType::VERTEX, sizeof( vertices ) ) ;
  buffer   .copyToDevice( vertices ) ;
  cmd_buff.record() ;
  cmd_buff.draw( buffer.size(), buffer.buffer(), pipeline ) ;
  cmd_buff.stop() ;

  drawFrame() ;
  return 0 ;
}

