#include "casper_vulkan_validation_layer.h"
#include <vulkan/vulkan.h>
#include <stdio.h>
#include <vector>
#include <string>
#include <cstring>

namespace casper
{
  namespace vulkan
  {

    std::vector<const char*> validation_layers = 
    {
      "VK_LAYER_KHRONOS_validation",
      "VK_LAYER_LUNARG_standard_validation",
//      "VK_LAYER_NV_optimus"
    };

    struct LayerData
    {
      std::vector<VkLayerProperties> available_layers ; ///< JHTODO
      std::vector<const char*      > layers           ; ///< JHTODO
      unsigned                       layer_count      ; ///< JHTODO
      bool                           layer_found      ; ///< JHTODO
      
      LayerData()
      {
        this->layer_count = 0     ;
        this->layer_found = false ;
      }
    };

    ValidationLayer::ValidationLayer()
    {
      this->layer_data = new LayerData() ;
    }
    
    ValidationLayer::~ValidationLayer( ) 
    {
      delete( this->layer_data ) ;
    }

    bool ValidationLayer::supported()
    {
      data().layer_count = 0     ;
      
      data().available_layers.clear() ;
      
      vkEnumerateInstanceLayerProperties( &data().layer_count, NULL ) ;
      data().available_layers.resize( data().layer_count ) ;
      vkEnumerateInstanceLayerProperties( &data().layer_count, data().available_layers.data() ) ;
      
      for ( const char* layerName : validation_layers )
      {
        data().layer_found = false ;
        
        for ( const auto& layerProperties : data().available_layers ) 
        {
          if ( strcmp(layerName, layerProperties.layerName) == 0 )
          {
            data().layers.push_back( layerProperties.layerName ) ;
            data().layer_found = true;
            break;
          }
        }
  
        if (!data().layer_found) 
        {
          printf(" Failed to find validation layer: %s\n", layerName ) ;
          return false;
        }
      }
      
    return true;
    }
    
    const char** ValidationLayer::names()
    {
      supported() ;
      return validation_layers.data() ;
    }
    
    unsigned ValidationLayer::count()
    {
      supported() ;
      return validation_layers.size() ;
    }
    
    LayerData& ValidationLayer::data()
    {
      return *this->layer_data ;
    }
    
    const LayerData& ValidationLayer::data() const
    {
      return *this->layer_data ;
    }
  }
}
