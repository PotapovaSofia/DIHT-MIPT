project(matrix)

set(LIB_SOURCES
  matrix.h
  matrix.cpp)

add_library(matrix STATIC ${LIB_SOURCES})

add_executable(Matrix matrix.cpp)

