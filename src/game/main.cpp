#include "manager/Manager.h"

int main( int argc, char** argv )
{
  karma::man::GameManager manager ;

  manager.initialize( argc, argv ) ;

  return manager.start() ;
}