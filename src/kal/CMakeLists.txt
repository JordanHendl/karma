find_package( SDL2 REQUIRED          )
find_package( SDL2_mixer REQUIRED    )

set( KAL_SOURCES 
     KAL_Interface.cpp
   )
      
set( KAL_HEADERS
     KAL_Interface.h
   )

set( KAL_INCLUDE_DIRS
      ${SDL2_INCLUDE_DIRS}
      ${CMAKE_WORKING_DIRECTORY}/include
      ${DATA_BUS_DIR}
      ${CONFIGURATION_DIR}
   )
   
set( KAL_LIBRARIES
     ${SDL2_LIBRARIES}
     ${SDL2_MIXER_LIBRARIES}
     kal_database
     karma_configuration
     karma_data_bus
     karma_log
    )

set( KAL_DEPS
     kal_database
     karma_configuration
     karma_data_bus
     karma_log
    )

ADD_SUBDIRECTORY( database )

ADD_LIBRARY               ( kal SHARED  ${KAL_SOURCES} ${KAL_HEADERS} )
TARGET_LINK_LIBRARIES     ( kal PUBLIC ${KAL_LIBRARIES}              )
TARGET_INCLUDE_DIRECTORIES( kal PRIVATE ${KAL_INCLUDE_DIRS}           )
ADD_DEPENDENCIES          ( kal ${KAL_DEPS} ) 
set( CMAKE_EXPORT_PACKAGE_REGISTRY ON )

INSTALL( TARGETS kal EXPORT kgl_lib DESTINATION ${KARMA_LIB_INSTALL_DIR}          )
INSTALL( EXPORT kgl_lib DESTINATION ${KARMA_LIB_INSTALL_DIR} FILE KGLConfig.cmake )
INSTALL( FILES KAL_Interface.h DESTINATION ${KARMA_HEADER_INSTALL_DIR}            )
