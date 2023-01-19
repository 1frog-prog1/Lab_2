cmake_minimum_required(VERSION 3.23)
project(Deque C CXX)

set(CMAKE_C_STANDARD 11)

set("CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -std=c++20")

add_executable(Deque source.cpp)