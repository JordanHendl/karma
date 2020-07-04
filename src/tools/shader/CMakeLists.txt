
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
     glslang
     SPIRV
     stdc++fs
    )

add_library               ( glsl_compiler SHARED   ${TOOLS_SHADER_SOURCES} ${TOOLS_SHADER_HEADERS} )
target_include_directories( glsl_compiler PRIVATE  ${TOOLS_SHADER_INCLUDE_DIRS}                    )
target_link_libraries     ( glsl_compiler PRIVATE  ${TOOLS_SHADER_LIBRARIES}                       )

add_executable            ( glslToUWU main.cpp ArgumentParser.cpp ${TOOLS_SHADER_SOURCES} ${TOOLS_SHADER_HEADERS} ArgumentParser.h )
target_include_directories( glslToUWU PRIVATE  ${TOOLS_SHADER_INCLUDE_DIRS}                                                        )
target_link_libraries     ( glslToUWU PRIVATE  ${TOOLS_SHADER_LIBRARIES}                                                           )
