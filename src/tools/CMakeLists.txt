
set( CASPER_SHADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/shader)

if( ${BUILD_UWUMAKER} )
  add_subdirectory( glslang   ) 
  add_subdirectory( uwuwriter ) 
  add_subdirectory( uwumaker  ) 
endif()

add_subdirectory( shader    ) 

