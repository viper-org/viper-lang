SHELL=/bin/sh

PREFIX=.
EXEC_PREFIX=$(PREFIX)

INCLUDEDIR=$(EXEC_PREFIX)/include
LIBDIR=$(EXEC_PREFIX)/lib
SRCDIR=$(EXEC_PREFIX)/src

CXXC=g++
LD=g++

CXX_FLAGS=-fsanitize=address,undefined -Og -ggdb3 -I$(INCLUDEDIR) -std=c++17 -Wall -Wextra -Wpedantic -lLLVM
LD_FLAGS=-fsanitize=address,undefined -lLLVM

CXX_SRCS:=$(shell find $(SRCDIR) -name '*.cxx')
OBJS:=${CXX_SRCS:.cxx=.o}

TARGET=quark

all: $(TARGET)

%.o: %.cxx
	$(CXXC) $(CXX_FLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	$(LD) $(LD_FLAGS) $^ -o $@

clean:
	rm -rf $(TARGET) $(OBJS)