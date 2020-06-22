
set( CASPER_IO_SOURCES 
casper_object_loader.cpp
)

set( CASPER_IO_HEADERS
casper_object_loader.h
)

set( CASPER_IO_INCLUDE_DIRS
)

set( CASPER_IO_LIBRARIES
)

add_library( casper_io SHARED ${CASPER_IO_SOURCES} ${CASPER_IO_HEADERS} )


