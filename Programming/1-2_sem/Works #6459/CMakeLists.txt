project(Book)

set(LIB_SOURCES
  book.h works.h
  book.cpp works.cpp)

add_library(Book STATIC ${LIB_SOURCES})

add_executable(Book STATIC ${LIB_SOURCES})

