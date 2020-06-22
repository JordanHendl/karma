#include "Bus.h"
#include "Signal.h"
#include <map>
#include <string>

namespace data
{
  namespace module
  {
    typedef std::map<std::string, Signal> SignalMap ;
    typedef std::map<unsigned, SignalMap> BusPool   ;
    
    static BusPool pool ;
    
    Bus::Bus( unsigned id )
    {
      setId( id ) ;
    }

    Signal& Bus::operator[]( const char* key )
    {
      return pool[ this->identifier ][ key ] ;
    }

    const Signal& Bus::operator[]( const char* key ) const
    {
      return pool[ this->identifier ][ key ] ;
    }

    Signal& Bus::get( const char* key )
    {
      return pool[ this->identifier ][ key ] ;
    }

    const Signal& Bus::get( const char* key ) const
    {
      return pool[ this->identifier ][ key ] ;
    }

    unsigned Bus::id()
    {
      return this->identifier ;
    }

    void Bus::setId( unsigned id )
    {
      this->identifier = id ;
    }

  }
}

