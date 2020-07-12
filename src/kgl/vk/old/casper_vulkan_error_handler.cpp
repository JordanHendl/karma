#include "casper_vulkan_error_handler.h"
#include <stdlib.h> 
#include <stdio.h>
#include <vulkan/vulkan.h>
static const char* vulkanStringFromError( VkResult res )
{
  switch( res )
  {
    // VK_SUCCESS = 0,
    case VK_NOT_READY                   : return "A fence or query has not yet completed"                                                  ;
    case VK_TIMEOUT                     : return "A wait operation has not completed in the specified time"                                ;
    case VK_EVENT_SET                   : return "An event is signaled"                                                                    ;
    case VK_EVENT_RESET                 : return "An event is unsignaled"                                                                  ;
    case VK_INCOMPLETE                  : return "A return array was too small for the result"                                             ;
    case VK_ERROR_OUT_OF_HOST_MEMORY    : return "A host memory allocation has failed."                                                    ; 
    case VK_ERROR_OUT_OF_DEVICE_MEMORY  : return "A device memory allocation has failed."                                                  ; 
    case VK_ERROR_INITIALIZATION_FAILED : return "Initialization of an object could not be completed for implementation-specific reasons." ;
    case VK_ERROR_DEVICE_LOST           : return "The logical or physical device has been lost."                                           ;  
    // VK_ERROR_MEMORY_MAP_FAILED = -5,
    // VK_ERROR_LAYER_NOT_PRESENT = -6,
    // VK_ERROR_EXTENSION_NOT_PRESENT = -7,
    // VK_ERROR_FEATURE_NOT_PRESENT = -8,
    // VK_ERROR_INCOMPATIBLE_DRIVER = -9,
    // VK_ERROR_TOO_MANY_OBJECTS = -10,
    // VK_ERROR_FORMAT_NOT_SUPPORTED = -11,
    // VK_ERROR_FRAGMENTED_POOL = -12,
    // VK_ERROR_UNKNOWN = -13,
    // VK_ERROR_OUT_OF_POOL_MEMORY = -1000069000,
    // VK_ERROR_INVALID_EXTERNAL_HANDLE = -1000072003,
    // VK_ERROR_FRAGMENTATION = -1000161000,
    // VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS = -1000257000,
    // VK_ERROR_SURFACE_LOST_KHR = -1000000000,
    // VK_ERROR_NATIVE_WINDOW_IN_USE_KHR = -1000000001,
    // VK_SUBOPTIMAL_KHR = 1000001003,
    // VK_ERROR_OUT_OF_DATE_KHR = -1000001004,
    // VK_ERROR_INCOMPATIBLE_DISPLAY_KHR = -1000003001,
    // VK_ERROR_VALIDATION_FAILED_EXT = -1000011001,
    // VK_ERROR_INVALID_SHADER_NV = -1000012000,
    // VK_ERROR_INCOMPATIBLE_VERSION_KHR = -1000150000,
    // VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT = -1000158000,
    // VK_ERROR_NOT_PERMITTED_EXT = -1000174001,
    // VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT = -1000255000,
    // VK_THREAD_IDLE_KHR = 1000268000,
    // VK_THREAD_DONE_KHR = 1000268001,
    // VK_OPERATION_DEFERRED_KHR = 1000268002,
    // VK_OPERATION_NOT_DEFERRED_KHR = 1000268003,
    // VK_PIPELINE_COMPILE_REQUIRED_EXT = 1000297000,
    // VK_ERROR_OUT_OF_POOL_MEMORY_KHR = VK_ERROR_OUT_OF_POOL_MEMORY,
    // VK_ERROR_INVALID_EXTERNAL_HANDLE_KHR = VK_ERROR_INVALID_EXTERNAL_HANDLE,
    // VK_ERROR_FRAGMENTATION_EXT = VK_ERROR_FRAGMENTATION,
    // VK_ERROR_INVALID_DEVICE_ADDRESS_EXT = VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS,
    // VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS_KHR = VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS,
    // VK_ERROR_PIPELINE_COMPILE_REQUIRED_EXT = VK_PIPELINE_COMPILE_REQUIRED_EXT,
    default : return "Unknown error" ;
  }
}

namespace casper
{
  namespace vulkan
  {
    void ErrorHandler::HandleError( int err, const char *file, int line )
    {
      if ( err != VK_SUCCESS ) 
      {
        printf( "%s in %s at line %d\n", vulkanStringFromError( static_cast<VkResult>( err ) ), file, line );
        exit( EXIT_FAILURE );
      }
    }
  }
}
