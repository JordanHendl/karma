
set( KGL_DIR           ${CMAKE_CURRENT_SOURCE_DIR}/kgl    )
set( DATA_BUS_DIR      ${CMAKE_CURRENT_SOURCE_DIR}/data   )
set( CONFIGURATION_DIR ${CMAKE_CURRENT_SOURCE_DIR}/config )
set( TOOLS_DIR         ${CMAKE_CURRENT_SOURCE_DIR}/tools  )


include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/glm/glm ) 
add_subdirectory( tools  )
add_subdirectory( data   )
add_subdirectory( config )
add_subdirectory( kgl    ) 

add_subdirectory( game   )
add_subdirectory( glm    )

if ( ${DOXYGEN_FOUND} AND ${BUILD_DOC} )
    # set input and output files
    set(DOXYGEN_IN  ${DOXYGEN_CONFIG_DIR}/doxygen.cfg                 )
    set(DOXYGEN_OUT ${DOCUMENTATION_OUTPUT_DIRECTORY}/doc/doxygen.cfg )
    
    set( DOC_INPUT ${CMAKE_CURRENT_SOURCE_DIR} )

    # request to configure the file
    configure_file( ${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY )

    message("Generating ${MODULE_NAME} documentation")
    # note the option ALL which allows to build the docs together with the application
    add_custom_target( doc_doxygen ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM )

elseif( ${BUILD_DOC} )
  message("Doxygen need to be installed to generate documentation")

endif( ${DOXYGEN_FOUND} AND ${BUILD_DOC} )
