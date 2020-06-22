#include "casper_shader.h"
#include <string>
#include <iostream>

int main( int argc, const char** argv )
{
  int                   spot   ;
  casper::tools::Shader shader ;
  std::string           input  ;

  spot = -1 ;

  for( unsigned i = 1; i < argc; i++ )
  {
    input = std::string( argv[ i ] ) ;
    if( input == "|") 
    {
      spot = i ;
      break    ;
    }

    shader.addShader( argv[i] ) ;
  }

  if( spot != -1 && spot + 1 < argc )
  {
    shader.initialize() ;
    shader.write( argv[ spot + 1 ] ) ;

    return 0 ;
  }

  std::cout << "Usage: <shader>.vert <shader>.frag ... | <output_dir>" << std::endl ;
  return 0 ;
}