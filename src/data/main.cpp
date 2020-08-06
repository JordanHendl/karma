
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
      printf("Recieving value ::-> %s\n", val ) ;    
    }
    
    void subscribe()
    {
      bus( "1111"   ).attach<object, const char*>( this, &object::setConfiguration ) ;
      bus( "this a" ).attach<object, float      >( this, &object::setA             ) ;
      bus( "this b" ).attach<object, float      >( this, &object::setB             ) ;
      
      bus( "a" ).onCompletion( "printab", this, &object::printAB ) ;
      bus( "this a" ).addDependancy( this, &object::setA, "printab" ) ;
      bus( "this b" ).addDependancy( this, &object::setB, "printab" ) ;
    }
    
    void setA( float a )
    {
      this->a = a ;
      
      printf(" Set A: %f\n", a ) ;
    }

    void setB( float b )
    {
      this->b = b ;
      printf(" Set B: %f\n", b ) ;
    }


    void printAB()
    {
      printf(" DEPENDENCY MET: A-> %f, B-> %f \n", this->a, this->b ) ;
    }
    
  private:
    float a = 0.0f ;
    float b = 0.0f ;
};

int main( int argc, char** argv ) 
{
  float  value ;
  object obj   ;
  
  value = 15.0f ;
  
  obj.subscribe() ;
  
  printf("Testing Simple emitting of data..\n" ) ;
  printf("Emitting \"TEST\" ...\n" ) ;
  bus( "1111" ).emit<const char*>( "TEST" ) ;
  
  
  printf("\nTesting Dependancies..\n" ) ;

  printf("\nTesting Multiple Emits for Dependancies..\n" ) ;
  bus( "this a" ).emit<float>( 15.5f      ) ;
  bus( "this a" ).emit<float>( 280.2f     ) ;
  bus( "this a" ).emit<float>( 12.1f      ) ;
  bus( "this a" ).emit<float>( 789.102f   ) ;
  bus( "this b" ).emit<float>( 27705.f    ) ;

  printf("\nTesting Depandancy auto-reset..\n" ) ;
  bus( "this b" ).emit<float>( 27705.f    ) ;
  bus( "this b" ).emit<float>( 12.1f      ) ;
  bus( "this b" ).emit<float>( 425.245f   ) ;
  bus( "this b" ).emit<float>( 789.102f   ) ;
  bus( "this a" ).emit<float>( 100.0f     ) ;
  
  printf("\nTesting pool setting & grabbing..\n" ) ;
  printf("Settings value %f in pool...\n", value ) ;
  pool[ "instance" ].set<float>( value ) ;

  value = 0.0f ;

  printf("Value before pool: %f\n", value ) ;

  value = pool[ "instance" ].get<float>() ;

  printf("Value after pool: %f\n", value ) ;

  return 0;
}

