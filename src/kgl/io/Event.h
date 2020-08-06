/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Event.h
 * Author: jhendl
 *
 * Created on August 1, 2020, 3:36 PM
 */

#ifndef KGL_IO_EVENT_H
#define KGL_IO_EVENT_H

union SDL_Event ;
namespace kgl
{
  namespace io
  {
    class Event ;
  }
}

::kgl::io::Event& operator<<( ::kgl::io::Event& event, SDL_Event& sdl_event ) ;

namespace kgl
{
  namespace io
  {
    /**
     */
    class Event
    {
      public:
        
        /**
         */
        enum class Type
        {
          None,
          Quit,
          KeyDown,
        };

        /**
         */
        enum class IOCode
        {
          None,
          Escape,
          Left,
          Right,
          Up,
          Down,
          Z,
          Enter,
          Zero,
        };
        
        /**
         */
        enum class Modifier
        {
          None,
          LeftShift,
          RightShift,
          LeftCtrl,
          RightCtrl,
          LeftAlt,
          RightAlt,
          LeftGUI,
          RightGUI,
          NumLock,
          CapsLock,
          Mode,
        };

        /**
         */
        Event() ;
        
        /**
         */
        ~Event() = default ;
        
        /**
         * @return 
         */
        Type   type() const ;
        
        /**
         * @return 
         */
        IOCode code() const ;
        
        /**
         * @return 
         */
        Modifier mod() const ;
      private:
        
        /**
         */
        friend ::kgl::io::Event& ::operator<<( ::kgl::io::Event& event, SDL_Event& sdl_event ) ;
        
        /**
         */
        Type     event_type ;
        
        /**
         */
        IOCode   key_code   ;
        
        /**
         */
        Modifier modifier   ;
    };
  }
}
#endif /* EVENT_H */

