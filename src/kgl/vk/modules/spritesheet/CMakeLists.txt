
set( KGL_VK_SPRITESHEET_MODULE_MODULE_SOURCES 
     SpriteSheet.cpp
   )
     
set( KGL_VK_SPRITESHEET_MODULE_MODULE_HEADERS
     SpriteSheet.h
   )

set( KGL_VK_SPRITESHEET_MODULE_MODULE_INCLUDE_DIRS
      ${DATA_BUS_DIR}
      ${KGL_NODE_DIR}
    )

set( KGL_VK_SPRITESHEET_MODULE_MODULE_LIBRARIES
     data_bus
     kgl_vk_module
     kgl
   )

add_library               ( spritesheet SHARED   ${KGL_VK_SPRITESHEET_MODULE_MODULE_SOURCES} ${KGL_VK_SPRITESHEET_MODULE_MODULE_HEADERS} )
target_include_directories( spritesheet PRIVATE  ${KGL_VK_SPRITESHEET_MODULE_MODULE_INCLUDE_DIRS}                                        )
target_link_libraries     ( spritesheet PRIVATE  ${KGL_VK_SPRITESHEET_MODULE_MODULE_LIBRARIES}                                           )

find_package(Doxygen)
if ( ${DOXYGEN_FOUND} AND ${BUILD_DOC} )
    # set input and output files
    set(DOXYGEN_IN  ${DOXYGEN_CONFIG_DIR}/doxygen.cfg                      )
    set(DOXYGEN_OUT ${DOCUMENTATION_OUTPUT_DIRECTORY}/render2d/doxygen.cfg )
    
    set( DOC_INPUT ${CMAKE_CURRENT_SOURCE_DIR} )

    # request to configure the file
    configure_file( ${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY )
    message("Doxygen build started")

    # note the option ALL which allows to build the docs together with the application
    add_custom_target( doc_doxygen ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM )
else (DOXYGEN_FOUND)
  message("Doxygen need to be installed to generate the doxygen documentation")
endif (DOXYGEN_FOUND)