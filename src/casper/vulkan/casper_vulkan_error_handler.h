#ifndef CASPER_VULKAN_ERROR_HANDLER_H
#define CASPER_VULKAN_ERROR_HANDLER_H

namespace casper
{
  namespace vulkan
  {
    class ErrorHandler
    {
      public:
      static void HandleError( int err, const char *file, int line ) ;
    };
  }
}

#define HANDLE_ERROR( err ) (casper::vulkan::ErrorHandler::HandleError( err, __FILE__, __LINE__ ) )

#endif