project(book)

set(LIB_SOURCES
 kvater.h
  kvater.cpp main.cpp)

add_library(book STATIC ${LIB_SOURCES})

add_executable(Book STATIC ${LIB_SOURCES})

