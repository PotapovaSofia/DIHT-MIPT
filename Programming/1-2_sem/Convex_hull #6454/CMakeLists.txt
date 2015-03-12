project(Convex_hull)

set(LIB_SOURCES
  polygon.h
  convex_hull.cpp main.cpp)

add_library(Convex_hull STATIC ${LIB_SOURCES})

add_executable(Convex_hull STATIC ${LIB_SOURCES})

