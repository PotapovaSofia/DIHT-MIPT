cmake_minimum_required(VERSION 3.1)
project(lock_free_stack)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp)
add_executable(lock_free_stack ${SOURCE_FILES} lock_free_stack.h)