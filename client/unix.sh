#!/bin/bash
set -e

echo -e "///-------------------------------------------------------"
echo -e "/// Crypto++ AES/RSA web encoder"
echo -e "///-------------------------------------------------------"
echo -e "Set the C compiler(default: gcc): "
read C_COMPILER
if [ "$C_COMPILER" == "" ]; then
    C_COMPILER='gcc'
fi

echo -e "Set the C++ compiler(default: g++): "
read CPP_COMPILER
if [ "$CPP_COMPILER" == "" ]; then
    CPP_COMPILER='g++'
fi

# remove "old" CMakeLists.txt
if [ -f "CMakeLists.txt" ]; then
    rm CMakeLists.txt
fi

#remove old building
if [ -d "build" ]; then
    rm -rf build
fi

# create the building directory
mkdir build

# create the CMakeLists.txt file 
touch CMakeLists.txt
cat >CMakeLists.txt <<EOL
set( CMAKE_C_COMPILER $C_COMPILER )
set( CMAKE_CXX_COMPILER $CPP_COMPILER )

cmake_minimum_required( VERSION 3.2 )
project( encoder )

set( CMAKE_CXX_FLAGS "\${CMAKE_CXX_FLAGS} -O3 -std=c++14" )

include_directories( "include"  )
include_directories( "contrib/nlohmann"  )
include_directories( "contrib/tinyformat"  )
include_directories( "contrib/jarro2783"  )

add_definitions( -DWEB_ENCODER )

file( GLOB SOURCES "src/*.cpp" )

set( PROJECT_LINK_LIBS dl ssl crypto crypto++ pthread )

add_executable( encoder \${SOURCES} )
target_link_libraries( encoder \${PROJECT_LINK_LIBS} )
EOL

# go to the building directory
cd build

# let cmake configure the project
cmake ..

# build the project with 2 threads
make -j2
