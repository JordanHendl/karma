#include "List.h"
#include "BufferedStack.h"
#include <iostream>

bool checkBasicListFunctionality()
{
  kgl::List<float> list ;
  bool             pass ;
  
  list.initialize( 100  ) ;
  list.fill      ( 2.0f ) ;
  
  pass = true ;
  for( unsigned i = 0; i < list.size(); i++ )
  {
    if( list[ i ] != 2.0f ) pass = false ;
  }
  
  return pass ;
}

bool checkListCopy()
{
  kgl::List<unsigned> list1 ;
  kgl::List<unsigned> list2 ;
  bool             pass  ;
  
  list1.initialize( 5 ) ;
  list1.fill      ( 5 ) ;
  
  list2.copy( list1 ) ;
  
  pass = true ;
  for( unsigned i = 0; i < list2.size(); i++ )
  {
    if( list2[ i ] != 5 ) pass = false ;
  }
  
  return pass ;
}

bool checkListIterator()
{
  kgl::List<unsigned> list  ;
  bool                pass  ;
  unsigned            index ;
  
  list.initialize( 5 ) ;
  list.fill      ( 5 ) ;
  
  pass  = true ;
  index = 0    ;
  for( auto iter = list.begin(); iter != list.end(); ++iter )
  {
    if( *iter != 5 ) pass = false ;
    index++ ;
  }
  
  if( index != 5 ) pass = false ;
  index = 0 ;

  for( auto val : list )
  {
    if( val != 5 ) pass = false ;
    index++ ;
  }
  
  if( index != 5 ) pass = false ;
  return pass ;
}

bool checkStackFunctionality()
{
  struct SimpleStruct
  {
    float    a = 0.f ;
    unsigned b = 0   ;
  };
  
  kgl::BufferedStack<SimpleStruct, 2> stack ;
  bool                                pass  ;
  SimpleStruct                        s     ;
  
  pass = true ;
  
  s.a = 0.0f ;
  s.b = 0    ;
  stack.insert( s, 0 ) ;
  
  s.a = 5.0f ;
  s.b = 5    ;
  stack.insert( s, 1 ) ;

  s.a = 10.0f ;
  s.b = 10    ;
  stack.insert( s, 2 ) ;
  
  while( !stack.empty() )
  {
    auto val = stack.pop() ;
    
    if( val.a != 0.0f && val.b != 0 ) pass = false ;
  }
  
  stack.swap() ;
  
  while( !stack.empty() )
  {
    auto val = stack.pop() ;
    
    if( val.a != 5.0f && val.b != 5 ) pass = false ;
  }
  
  stack.swap() ;
  
  while( !stack.empty() )
  {
    auto val = stack.pop() ;
    
    if( val.a != 10.0f && val.b != 10 ) pass = false ;
  }

  return pass ;
}
  
int main() 
{
  bool pass ;
  
  std::cout << "Checking List functionality.." << std::endl ;
  pass = checkBasicListFunctionality() ;
  if( pass ) std::cout << "Passed!" << std::endl ;
  else       std::cout << "Failed!" << std::endl << std::endl;
  
  std::cout << "Checking List copy.." << std::endl ;
  pass = checkListCopy() ;
  if( pass ) std::cout << "Passed!" << std::endl ;
  else       std::cout << "Failed!" << std::endl << std::endl ;
  
  std::cout << "Checking List iterator.." << std::endl ;
  pass = checkListIterator() ;
  if( pass ) std::cout << "Passed!" << std::endl ;
  else       std::cout << "Failed!" << std::endl << std::endl ;
  
  std::cout << "Checking Buffered Stack functionality.." << std::endl ;
  pass = checkStackFunctionality() ;
  if( pass ) std::cout << "Passed!" << std::endl ;
  else       std::cout << "Failed!" << std::endl << std::endl ;
  
  return 0;
}

