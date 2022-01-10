cmake_minimum_required(VERSION 2.8)
project(rgen-shortestpath)

# enable c++11 support
set (CMAKE_CXX_FLAGS "-std=c++11 -Wall ${CMAKE_CXX_FLAGS}")
set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/run)

# create the main executable
## add additional .cpp files if needed
add_executable(sp shortestpath.cpp )
add_executable(mainp mainprogram.cpp )
add_executable(rgen rgen.cpp)

# install binaries
install(PROGRAMS getgraph.py DESTINATION bin)
install(TARGETS sp mainp rgen RUNTIME DESTINATION bin)
