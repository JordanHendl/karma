set( UWUMAKER_LIBRARIES
     karma_uwushader
     karma_uwuwriter
     glslang
     SPIRV
     stdc++fs
    )

add_executable       ( karma_uwumaker main.cpp ArgumentParser.cpp ArgumentParser.h )
target_link_libraries( karma_uwumaker PUBLIC  ${UWUMAKER_LIBRARIES}               )
