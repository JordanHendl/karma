
SET( KGL_DIR           ${CMAKE_CURRENT_SOURCE_DIR}/kgl    )
SET( DATA_BUS_DIR      ${CMAKE_CURRENT_SOURCE_DIR}/data   )
SET( CONFIGURATION_DIR ${CMAKE_CURRENT_SOURCE_DIR}/config )
SET( TOOLS_DIR         ${CMAKE_CURRENT_SOURCE_DIR}/tools  )


INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR} )
INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR}/glm/glm ) 

ADD_SUBDIRECTORY( config    )
ADD_SUBDIRECTORY( data      )
ADD_SUBDIRECTORY( game      )
ADD_SUBDIRECTORY( glm       )
ADD_SUBDIRECTORY( kgl       ) 
ADD_SUBDIRECTORY( log       )
ADD_SUBDIRECTORY( profiling )
ADD_SUBDIRECTORY( tools     )

if ( ${DOXYGEN_FOUND} AND ${BUILD_DOC} )
    # set input and output files
    set(DOXYGEN_IN  ${DOXYGEN_CONFIG_DIR}/doxygen.cfg                 )
    set(DOXYGEN_OUT ${DOCUMENTATION_OUTPUT_DIRECTORY}/doc/doxygen.cfg )
    
    set( DOC_INPUT ${CMAKE_CURRENT_SOURCE_DIR}/ )

    # request to configure the file
    configure_file( ${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY )

    message("Generating KGL documentation")
    # note the option ALL which allows to build the docs together with the application
    add_custom_target( doc_doxygen ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM )

elseif( ${BUILD_DOC} )
  message("Doxygen need to be installed to generate documentation")

endif( ${DOXYGEN_FOUND} AND ${BUILD_DOC} )
