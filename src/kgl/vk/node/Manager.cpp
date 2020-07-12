/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Manager.cpp
 * Author: jhendl
 * 
 * Created on July 10, 2020, 6:04 AM
 */

#include "Manager.h"

namespace kgl
{
  namespace vk
  {
    struct ManagerData
    {
      
    };

    Manager::Manager()
    {
    
    }

    Manager::~Manager()
    {
    
    }

    void Manager::initialize( const char* configuration_path )
    {
    
    }

    void Manager::start()
    {
    
    }

    ManagerData& Manager::data()
    {
      return *this->man_data ;
    }

    const ManagerData& Manager::data() const
    {
      return *this->man_data ;
    }
  }
}