cmake_minimum_required(VERSION 3.0)
project(Deque CXX)

set("CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -std=c++20")

add_executable(Deque deque.h source.cpp)