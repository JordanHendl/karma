#include "CommandManager.h"
#include <Bus.h>
#include <Signal.h>
namespace kgl
{
  namespace man
  {
    struct CommandManagerData
    {
      
    };

    CommandManager::CommandManager()
    {
      this->man_data = new CommandManagerData() ;
    }

    CommandManager::~CommandManager()
    {
      delete this->man_data ;
    }
    
    void CommandManager::initialize() 
    {
      
    }

    void CommandManager::insertCommand( const ImageSubmition& command )
    {
    
    }

    void CommandManager::publish()
    {
    
    }

    void CommandManager::clear()
    {
    
    }
    
    CommandManagerData& CommandManager::data()
    {
      return *this->man_data ;
    }

    const CommandManagerData& CommandManager::data() const
    {
      return *this->man_data ;
    }
  }
}
