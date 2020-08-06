#include "Validation.h"
#include <vulkan/vulkan.hpp>
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

    struct ValidationData
    {
      std::vector<::vk::LayerProperties> available_layers ; ///< JHTODO
      std::vector<const char*          > layers           ; ///< JHTODO
      unsigned                           layer_count      ; ///< JHTODO
      bool                               layer_found      ; ///< JHTODO
      
      ValidationData()
      {
        this->layer_count = 0     ;
        this->layer_found = false ;
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
      data().layer_count = 0 ;
      
      data().available_layers.clear() ;
      data().available_layers = ::vk::enumerateInstanceLayerProperties() ;

      for ( const auto& prop : data().available_layers ) 
      {
        data().layers.push_back( prop.layerName ) ;
        std::cout << " Enabling layer:: " << prop.layerName << std::endl ;
      }
      return true;
    }
    
    const char** Validation::names()
    {
      supported() ;
      return validation_layers.data() ;
    }
    
    unsigned Validation::count()
    {
      supported() ;
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
