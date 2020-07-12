/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   casper_vulkan_device_pool.cpp
 * Author: jhendl
 * 
 * Created on July 3, 2020, 1:07 PM
 */

#include "casper_vulkan_device_pool.h"
#include "Device.h"
#include <map>
namespace casper
{
  namespace vulkan
  {
    static std::map<unsigned, ::casper::vulkan::Device> device_map ;
    
    const Device& DevicePool::device( unsigned gpu ) const 
    {
      static ::casper::vulkan::Device dummy ;

      auto device = device_map.find( gpu ) ;
      
      if( device != device_map.end() ) return device->second ;
      
      return dummy ;
    }
    
    void DevicePool::setDevice( unsigned gpu, const Device& device ) const 
    {
      auto it = device_map.find( gpu ) ;
      
      if( it == device_map.end() ) device_map.insert( { gpu, device } ) ;
    }
  }
}