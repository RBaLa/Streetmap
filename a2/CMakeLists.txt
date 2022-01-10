cmake_minimum_required(VERSION 2.8)
project(MST-2)

# enable c++11 support
set (CMAKE_CXX_FLAGS "-std=c++11 -Wall ${CMAKE_CXX_FLAGS}")

# create the main executable
add_executable(ShortestPath ShortestPath.cpp )
