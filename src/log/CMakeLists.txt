SET( KARMA_LOG_SOURCES 
     Log.cpp
   )
      
SET( KARMA_LOG_HEADERS
     Log.h
   )

SET( KARMA_LOG_INCLUDES
   )

SET( KARMA_LOG_LIBRARIES
    )

ADD_LIBRARY( karma_log SHARED ${KARMA_LOG_SOURCES} ${KARMA_LOG_HEADERS} )

ADD_EXECUTABLE       ( karma_log_test test.cpp          )
TARGET_LINK_LIBRARIES( karma_log_test PRIVATE karma_log ) 

