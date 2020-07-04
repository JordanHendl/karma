set( CASPER_VULKAN_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vulkan )


find_package( X11                )
find_package( PNG                )
find_package( Threads            )
find_package( Vulkan REQUIRED    )
find_package( SDL2 REQUIRED      )

set( CASPER_SOURCES 
      casper.cpp 
      window/casper_library.cpp
   )
      
set( CASPER_HEADERS
      casper.h
      window/casper_library.h
   )

set( CASPER_INCLUDE_DIRS
      ${SDL2_INCLUDE_DIRS}
      ${Vulkan_INCLUDE_DIRS}
      ${CMAKE_WORKING_DIRECTORY}/include
      ${DATA_BUS_DIR}
      ${CASPER_VULKAN_DIR}
      ${CASPER_SHADER_DIR}
   )

set( CASPER_LIBRARIES
     ${SDL2_LIBRARIES}
     ${Vulkan_LIBRARIES}
     data_bus
     casper_vulkan
    )

add_subdirectory( io     )
add_subdirectory( vulkan )

add_library               ( casper SHARED ${CASPER_SOURCES} ${CASPER_HEADERS} )
target_include_directories( casper PRIVATE  ${CASPER_INCLUDE_DIRS}            )
target_link_libraries     ( casper PRIVATE  ${CASPER_LIBRARIES}               )

add_executable            ( casper_test main.cpp                                          )
target_include_directories( casper_test PRIVATE  ${CASPER_INCLUDE_DIRS}                   )
target_link_libraries     ( casper_test PRIVATE  ${CASPER_LIBRARIES} casper casper_vulkan )

