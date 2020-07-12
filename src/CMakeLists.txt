
set( KGL_DIR           ${CMAKE_CURRENT_SOURCE_DIR}/kgl    )
set( DATA_BUS_DIR      ${CMAKE_CURRENT_SOURCE_DIR}/data   )
set( CONFIGURATION_DIR ${CMAKE_CURRENT_SOURCE_DIR}/config )
set( TOOLS_DIR         ${CMAKE_CURRENT_SOURCE_DIR}/tools  )

add_subdirectory( tools  )
add_subdirectory( data   )
add_subdirectory( config )
add_subdirectory( kgl    ) 
add_subdirectory( game   )
