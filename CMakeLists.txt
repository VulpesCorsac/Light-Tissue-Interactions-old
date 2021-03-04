cmake_minimum_required(VERSION 3.5)
project(MCML CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -O2")

add_subdirectory(googletest/)

enable_testing()

add_subdirectory(mcml/)
