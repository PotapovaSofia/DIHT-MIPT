project(book)

set(LIB_SOURCES
  book.h
  book.cpp main.cpp)

add_library(book STATIC ${LIB_SOURCES})

add_executable(Book STATIC ${LIB_SOURCES})

