#include "Validation.h"
#include <vulkan/vulkan.hpp>
#include <log/Log.h>
#include <stdio.h>
#include <vector>
#include <string>
#include <cstring>
#include <iostream>

namespace kgl
{
  namespace vk
  {

    std::vector<const char*> validation_layers = 
    {
      "VK_LAYER_KHRONOS_validation",
      "VK_LAYER_LUNARG_standard_validation",
    };

    static std::vector<::vk::LayerProperties> available_layers         ; ///< JHTODO
    static std::vector<const char*          > layers                   ; ///< JHTODO
    static unsigned                           layer_count      = 0     ; ///< JHTODO
    static bool                               initted          = false ; ///< JHTODO

    struct ValidationData
    {
      
      ValidationData()
      {
      }
    };

    Validation::Validation()
    {
      this->layer_data = new ValidationData() ;
    }
    
    Validation::~Validation( ) 
    {
      delete( this->layer_data ) ;
    }

    bool Validation::supported()
    {
      layer_count = 0 ;
      if( !initted )
      {
        available_layers.clear() ;
        available_layers = ::vk::enumerateInstanceLayerProperties() ;
        initted = true ;
  
        for ( const auto& prop : available_layers ) 
        {
          for( auto requested : validation_layers )
          {
            if( std::string( prop.layerName ) == std::string( requested ) )
            {
              layers.push_back( prop.layerName ) ;
              
              karma::log::Log::output( "Using validation layer: ", prop.layerName ) ;
            }
          }
        }
      }
      return true;
    }
    
    const char** Validation::names()
    {
      if( !initted )
      {
        supported()    ;
        initted = true ;
      }

      return validation_layers.data() ;
    }
    
    unsigned Validation::count()
    {
      if( !initted )
      {
        supported()    ;
        initted = true ;
      }
      return validation_layers.size() ;
    }
    
    ValidationData& Validation::data()
    {
      return *this->layer_data ;
    }
    
    const ValidationData& Validation::data() const
    {
      return *this->layer_data ;
    }
  }
}
