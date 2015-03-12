project(Big_int)

set(LIB_SOURCES
  big_int.h
  big_int.cpp)

add_library(Big_int STATIC ${LIB_SOURCES})

add_executable(Big_int STATIC ${LIB_SOURCES})

