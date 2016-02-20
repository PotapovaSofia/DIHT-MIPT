# Makefile for a C project
# v0.8 by Andrey Zakharov
# <andrey.zakharov@phystech.edu>
# Licenced unter WTFPL v 2.0
# Not guaranteed not to accidentally wipe your HDD

PATH   := $(PATH):/usr/lib64/openmpi/bin
CC      = mpic++
LINK    = mpic++
CFLAGS  = -O2 -Wall -Wextra #-Werror
CFLAGS += $(flags)
LFLAGS  = -pthread -lrt

TARGET  = life

TARGET := bin/$(TARGET)
SRC     = $(wildcard src/*.cpp)
OBJ     = $(SRC:src/%.cpp=build/%.o)
DIRS    = bin build

.PHONY: all run debug rebuild clean

all: | $(DIRS) $(TARGET)

run: all
	mpirun $(TARGET) 10 10 10

debug: clean
	make flags="-g -O0 -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC -D_FORTIFY_SOURCE=2 -fstack-protector "

rebuild: clean all

clean:
	rm -f $(OBJ) $(TARGET)

$(TARGET): $(OBJ)
	$(LINK) $(CFLAGS) $(LFLAGS) -o $@ $^
	@echo "Build succeeded"

build/%.o : src/%.cpp
	$(CC) -c $(CFLAGS) -o $@ $<

%:
	mkdir -p $@
