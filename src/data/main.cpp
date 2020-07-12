
//#include<casper_bus.h>
//#include<casper_signal.h>
#include "Bus.h"
#include "Signal.h"
#include "Pool.h"
#include "PoolData.h"
#include <stdio.h>

static data::module::Bus bus ;
static data::module::Pool pool ;

class object
{
  public:
    
    void setConfiguration( const char* val )
    {
      printf(" Recieving value ::-> %s\n", val ) ;    
    }
    
    void subscribe()
    {
      bus( "1111" ).attach<object, const char*>( this, &object::setConfiguration ) ;
    }
};

int main( int argc, char** argv ) 
{
  object obj ;
  
  obj.subscribe() ;
  bus( "1111" ).emit<const char*>( "peepee poo poo" ) ;

  float value = 1.5f ;
  pool[ "instance" ].set<float>( value ) ;

  value = 0.0f ;

  printf("value before pool: %f", value ) ;

  value = pool[ "instance" ].get<float>() ;

  printf("value after pool: %f", value ) ;

  return 0;
}

