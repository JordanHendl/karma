#include "Memory.h"
#include "Vulkan.h"
#include <vulkan/vulkan.hpp>
int main()
{
  using impl = kgl::vk::Vulkan ;
  kgl::Buffer<impl> memory ;
  
  memory.initialize<float>( 200, 0 ) ;
}

