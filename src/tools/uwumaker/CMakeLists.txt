set( UWUMAKER_LIBRARIES
     uwushader
     uwuwriter
     glslang
     SPIRV
     stdc++fs
    )

add_executable       ( uwumaker main.cpp ArgumentParser.cpp ArgumentParser.h )
target_link_libraries( uwumaker PRIVATE  ${UWUMAKER_LIBRARIES}               )
