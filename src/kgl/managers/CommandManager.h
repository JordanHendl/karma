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
    class CommandManager ;

    class CommandCreator
    {
      public:
        
        enum Type
        {
          MODEL,
          IMAGE,
          PARTICLE,
          COMPUTE
        };

        CommandCreator() ;
        ~CommandCreator() ;
        
        void setNameOfAsset( const char* name ) ;
        void addGraph( const char* graph ) ;
        void setType( Type type ) ;
        
      private:
        struct CommandCreatorData* crt_data ;
        CommandCreatorData& data() ;
        const CommandCreatorData& data() const ;

        friend class CommandManager ;
    };

    class CommandManager
    {
      public:
        CommandManager() ;
        ~CommandManager() ;
        void insertCommand( CommandCreator& command ) ;
        void publish() ;
        void clear() ;
    };
  }
}

#endif /* COMMANDMANAGER_H */

