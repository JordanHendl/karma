set( TOOLS_SHADER_SOURCES 
     UWUShader.cpp
   )
     
set( TOOLS_SHADER_HEADERS
     UWUShader.h
   )

set( TOOLS_SHADER_INCLUDE_DIRS
     ${TOOLS_DIR}
   )

set( TOOLS_SHADER_LIBRARIES
     stdc++fs
    )

add_library               ( karma_uwushader SHARED   ${TOOLS_SHADER_SOURCES} ${TOOLS_SHADER_HEADERS} )
target_include_directories( karma_uwushader PRIVATE  ${TOOLS_SHADER_INCLUDE_DIRS}                    )
target_link_libraries     ( karma_uwushader PUBLIC  ${TOOLS_SHADER_LIBRARIES}                       )

INSTALL( TARGETS karma_uwushader EXPORT kgl_lib DESTINATION ${KARMA_LIB_INSTALL_DIR} )
INSTALL( TARGETS karma_uwushader DESTINATION ${KARMA_LIB_INSTALL_DIR} )