find_package( X11                )
find_package( PNG                )
find_package( Threads            )
find_package( Vulkan REQUIRED    )
find_package( SDL2 REQUIRED      )

set( CASPER_SHADER_SOURCES 
     casper_shader.cpp
   )
      
set( CASPER_SHADER_HEADERS
     casper_shader.h
   )

set( CASPER_SHADER_INCLUDE_DIRS
      ${SDL2_INCLUDE_DIRS}
      ${Vulkan_INCLUDE_DIRS}
      ${CASPER_TOOLS_GLSLANG_DIR}
   )

set( CASPER_SHADER_LIBRARIES
     ${SDL2_LIBRARIES}
     ${Vulkan_LIBRARIES}
     glslang
     SPIRV
    )

add_library               ( casper_shader SHARED ${CASPER_SHADER_SOURCES} ${CASPER_SHADER_HEADERS} )
target_include_directories( casper_shader PRIVATE  ${CASPER_SHADER_INCLUDE_DIRS}                   )
target_link_libraries     ( casper_shader PRIVATE  ${CASPER_SHADER_LIBRARIES}                      )

add_executable            ( casper_shader_exec main.cpp ${CASPER_SHADER_SOURCES} ${CASPER_SHADER_HEADERS} )
target_include_directories( casper_shader_exec PRIVATE  ${CASPER_SHADER_INCLUDE_DIRS}                     )
target_link_libraries     ( casper_shader_exec PRIVATE  ${CASPER_SHADER_LIBRARIES}                        )
