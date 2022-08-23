cmake_minimum_required(VERSION 3.22)
project(TA_Algorithms)

set(CMAKE_CXX_STANDARD 14)

#IF(NOT CMAKE_BUILD_TYPE)
#    SET(CMAKE_BUILD_TYPE "RELEASE")
#ENDIF()

add_executable(TA_Algorithms main.cpp Edge.h Vertex.h Graph.h Dijkstra.h TripMatrix.h ShortestPath.h timer.h timer.cpp)
