/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Present.cpp
 * Author: jhendl
 * 
 * Created on July 19, 2020, 8:37 PM
 */

#include "Present.h"
#include "Synchronization.h"
#include "../../context/Context.h"
#include "../../context/Pipeline.h"
#include "../../context/CommandBuffer.h"
#include "../../context/Buffer.h"
#include "../../context/RenderPass.h"
#include <Bus.h>
#include <Signal.h>
#include <vulkan/vulkan.hpp>
#include <string>

namespace kgl
{
  namespace vk
  {
    struct PresentData
    {
      ::kgl::vk::render::CommandBuffer buffer       ; ///< TODO
      ::kgl::vk::render::Context       context      ; ///< TODO
      ::kgl::vk::Synchronization       sync         ; ///< TODO
      ::kgl::vk::render::Pipeline      pipeline     ; ///< TODO
      ::kgl::vk::RenderPass            pass         ; ///< TODO
      ::kgl::vk::Buffer                vertices     ; ///< TODO
      ::kgl::vk::Buffer                indices      ; ///< TODO
      ::data::module::Bus              bus          ; ///< TODO
      unsigned                         gpu          ; ///< TODO
      ::vk::Device                     device       ; ///< TODO
      std::string                      window_name  ; ///< TODO
      std::string                      output_name  ;
      std::string                      install_path ; ///< TODO
    };

    Present::Present()
    {
      this->data_2d = new PresentData() ;
    }

    Present::~Present()
    {
      delete this->data_2d ;
    }

    void Present::initialize()
    {
    
    }

    void Present::shutdown()
    {
    
    }

    void Present::subscribe( const char* pipeline, unsigned id )
    {
    
    }

    void Present::execute()
    {
    
    }

    PresentData& Present::data()
    {
      return *this->data_2d ;
    }

    const PresentData& Present::data() const
    {
      return *this->data_2d ;
    }
  }
}
