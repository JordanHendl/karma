#ifndef KGL_IO_EVENT_H
#define KGL_IO_EVENT_H

/* Forward decleration of SDL event used for conversion.
 */
union SDL_Event ;

/* Forward Decleration of KGL event.
 */
namespace kgl
{
  namespace io
  {
    class Event ;
  }
}

/* Conversion operator between SDL_Event and kgl Event.
 */
::kgl::io::Event& operator<<( ::kgl::io::Event& event, SDL_Event& sdl_event ) ;

namespace kgl
{
  namespace io
  {
    /** Event object to describe keyboard & window events.
     */
    class Event
    {
      public:
        
        /** The type of event that has occured.
         */
        enum class Type
        {
          None,
          Quit,
          KeyDown,
          KeyUp,
        };

        /** The code correlating to the event that occured.
         */
        enum class IOCode
        {
          None,
          A,
          B,
          C,
          D,
          E,
          F,
          G,
          H,
          I,
          J,
          K,
          L,
          M,
          N,
          O,
          P,
          Q,
          R,
          S,
          T,
          U,
          V,
          W,
          X,
          Y,
          Z,
          Zero,
          One,
          Two,
          Three,
          Four,
          Five,
          Six,
          Seven,
          Eight,
          Nine,
          Escape,
          Left,
          Right,
          Up,
          Down,
          Space,
          Enter,
          Tab
        };
        
        /** The modifier that was active when the event happened.
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

        /** Default constructor
         */
        Event() ;
        
        /** Defaulted deconstructor.
         */
        ~Event() = default ;
        
        /** Method to return the type of event that this object represents.
         * @return The type of event this object represents.
         */
        Type type() const ;
        
        /** Method to retrieve the specific event code that occured.
         * @return The event code that occuted.
         */
        IOCode code() const ;
        
        /** Method to retrieve the modifier active when the event occured.
         * @return The modifer, if any, that were active when the event occured.
         */
        Modifier mod() const ;

      private:
        
        /** Friend declaration of SDL_Event conversion operator.
         */
        friend ::kgl::io::Event& ::operator<<( ::kgl::io::Event& event, SDL_Event& sdl_event ) ;
        
        /** The type of event.
         */
        Type event_type ;
        
        /** The code of event.
         */
        IOCode key_code ;
        
        /** The modifier.
         */
        Modifier modifier ;
    };
  }
}
#endif /* EVENT_H */

