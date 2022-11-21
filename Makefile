SHELL=/bin/sh

PREFIX=.
EXEC_PREFIX=$(PREFIX)

INCLUDEDIR=$(EXEC_PREFIX)/include
LIBDIR=$(EXEC_PREFIX)/lib
SRCDIR=$(EXEC_PREFIX)/src
TESTDIR=$(EXEC_PREFIX)/tests

PLATFORM_CXXFLAGS=
PLATFORM_LDFLAGS=

ifneq ($(OS),Windows_NT)
	PLATFORM_CXXFLAGS= -fsanitize=address,undefined 
	PLATFORM_LDFLAGS= -fsanitize=address,undefined 
endif

CXXC=g++
LD=g++

CXX_FLAGS=$(PLATFORM_CXXFLAGS) -O0 -ggdb3 -Lllvm -I$(INCLUDEDIR) -std=c++17 -Wall -Wextra -Wpedantic
LD_FLAGS=-Lllvm $(PLATFORM_LDFLAGS)

CXX_SRCS:=$(shell find $(SRCDIR) -name '*.cc')
OBJS:=${CXX_SRCS:.cc=.o}
TESTS:=$(shell find $(TESTDIR) -name '*.vpr')
TEST_OBJS:=${TESTS:.vpr=.asm}

TARGET=vprc

.PHONY: all test clean

all: $(TARGET)

%.o: %.cc
	$(CXXC) $(CXX_FLAGS) -c $< -o $@

%.asm: %.vpr
	./$(TARGET) $< > /dev/null

$(TARGET): $(OBJS)
	$(LD) $(LD_FLAGS) $^ -o $@

test: $(TARGET) $(TEST_OBJS)

clean:
	rm -rf $(TARGET) $(OBJS)