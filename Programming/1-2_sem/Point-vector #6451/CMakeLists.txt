project(Point-vector)

set(LIB_SOURCES
 point.h
 segment.h vector.h point.cpp segment.cpp vector.cpp)

add_library(Point-vector STATIC ${LIB_SOURCES})

add_executable(Point-vector STATIC ${LIB_SOURCES})

