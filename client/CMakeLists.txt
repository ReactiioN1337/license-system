set( CMAKE_C_COMPILER gcc )
set( CMAKE_CXX_COMPILER g++ )

cmake_minimum_required( VERSION 3.2 )
project( encoder )

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -std=c++14" )

include_directories( "include"  )
include_directories( "contrib/nlohmann"  )
include_directories( "contrib/tinyformat"  )

add_definitions( -DWEB_ENCODER )

file( GLOB SOURCES "src/*.cpp" )

set( PROJECT_LINK_LIBS dl ssl crypto crypto++ pthread )

add_executable( encoder ${SOURCES} )
target_link_libraries( encoder ${PROJECT_LINK_LIBS} )

