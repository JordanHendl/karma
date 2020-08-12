set( KGL_CMD_SOURCES 
     DrawCommand.cpp
   )
      
set( KGL_CMD_HEADERS
     DrawCommand.h
   )

set( KGL_CMD_INCLUDE_DIRS

   )

set( KGL_CMD_LIBRARIES
    )

ADD_LIBRARY( kgl_command SHARED ${KGL_CMD_SOURCES} ${KGL_CMD_HEADERS} )
