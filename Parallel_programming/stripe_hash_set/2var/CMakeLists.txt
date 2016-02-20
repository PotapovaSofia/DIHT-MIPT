cmake_minimum_required(VERSION 3.1)
project(striped_hash_set_3)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp)
add_executable(striped_hash_set_3 ${SOURCE_FILES})