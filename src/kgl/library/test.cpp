#include "Memory.h"
#include "Vulkan.h"

int main()
{
  using impl = kgl::vk::Vulkan ;
  kgl::Buffer<impl> memory ;
  
  memory.initialize<float>( 200, 0 ) ;
}

