/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Command.h
 * Author: jhendl
 *
 * Created on July 10, 2020, 2:40 PM
 */

#ifndef KGL_VK_COMMAND_H
#define KGL_VK_COMMAND_H

namespace vk
{
  class Buffer ;
  class CommandBuffer ;
}

namespace kgl
{
  namespace vk
  {
    namespace gpu
    {
      class Command
      {
        public:
          Command() ;
          virtual ~Command() ;
          virtual void push( const ::vk::CommandBuffer buffer ) const = 0 ;
        private:
      };
    }
  }
}


#endif /* COMMAND_H */

