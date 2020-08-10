#include "BufferedStack.h"
#include <stack>
#include <vector>
#include <mutex>

namespace kgl
{
  namespace containers
  {
    struct StackEntry
    {
      void* data ;
    };

    struct BufferedStackImplData
    {
      typedef void*                 DataEntry     ;
      typedef std::stack<DataEntry> Stack         ;
      typedef std::vector<Stack>    LayeredStacks ;
      
      std::mutex    mutex        ; ///< TODO
      LayeredStacks stacks       ; ///< TODO
      Stack         *curr        ; ///< TODO
      unsigned      current_swap ; ///< TODO
      unsigned      element_size ; ///< TODO
      
      /**
       */
      BufferedStackImplData() ;
    };

    BufferedStackImplData::BufferedStackImplData()
    {
      this->current_swap = 0 ;
      this->element_size = 0 ;
    }

    BufferedStackImpl::BufferedStackImpl()
    {
      this->bstack_data = new BufferedStackImplData() ;
    }

    BufferedStackImpl::~BufferedStackImpl()
    {
      delete this->bstack_data ;
    }

    bool BufferedStackImpl::empty()
    {
      return data().curr->empty() ;
    }

    void BufferedStackImpl::swap( int layer )
    {
      data().mutex.lock() ;
      if( layer < 0 )
      {
        data().current_swap++ ;
        data().current_swap = data().current_swap < data().stacks.size() ? data().current_swap : 0 ;

        data().curr = &data().stacks[ data().current_swap ] ;
      }
      else if( static_cast<unsigned>( layer ) < data().stacks.size() )
      {
        data().current_swap = static_cast<unsigned>( layer ) ;
        data().curr = &data().stacks[ data().current_swap ]  ;
      }
      else
      {
        data().current_swap = 0                              ;
        data().curr = &data().stacks[ data().current_swap ]  ;        
      }
      data().mutex.unlock() ;
    }
    
    void BufferedStackImpl::clear()
    {
      while( !data().curr->empty() ) data().curr->pop() ;
    }
    
    unsigned BufferedStackImpl::size( int layer ) const
    {
      if     ( static_cast<unsigned>( layer ) < 0                    ) return data().curr->size()           ;
      else if( static_cast<unsigned>( layer ) < data().stacks.size() ) return data().stacks[ layer ].size() ;
      else                                                             return 0                             ;
    }

    void BufferedStackImpl::initialize( unsigned layers, unsigned element_size )
    {
      data().mutex.lock() ;
      if( layers > 0 )
      {
        data().stacks.resize( layers ) ;

        data().curr         = &data().stacks[ 0 ] ;
        data().element_size = element_size        ;
      }
      data().mutex.unlock() ;
    }

    void BufferedStackImpl::insertBase( void* val, unsigned layer )
    {
      data().mutex.lock() ;
      if( layer < data().stacks.size() )
      {
        data().stacks[ layer ].emplace( val ) ;
      }
      data().mutex.unlock() ;
    }

    void* BufferedStackImpl::popBase()
    {
      data().mutex.lock() ;
      void* val = data().curr->top() ;
      
      data().curr->pop() ;

      data().mutex.unlock() ;
      return val ;
    }

    unsigned BufferedStackImpl::currentSwap() const
    {
      return data().current_swap ;
    }
    
    unsigned BufferedStackImpl::nextSwap() const
    {
      return data().current_swap + 1 < data().stacks.size() ? data().current_swap + 1 : 0 ;
    }

    BufferedStackImplData& BufferedStackImpl::data()
    {
      return *this->bstack_data ;
    }

    const BufferedStackImplData& BufferedStackImpl::data() const
    {
      return *this->bstack_data ;
    }
  }
}
