/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CommandManager.h
 * Author: jhendl
 *
 * Created on July 10, 2020, 3:33 PM
 */

#ifndef KGL_VK_COMMANDMANAGER_H
#define KGL_VK_COMMANDMANAGER_H

namespace kgl
{
  namespace man
  {
    class ImageSubmition ;
    class CommandManager
    {
      public:
        CommandManager() ;
        ~CommandManager() ;
        void initialize() ;
        void insertCommand( const ImageSubmition& command ) ;
        void publish() ;
        void clear() ;
      private:
        struct CommandManagerData *man_data ;
        CommandManagerData& data() ;
        const CommandManagerData& data() const ;
    };
  }
}

#endif /* COMMANDMANAGER_H */

