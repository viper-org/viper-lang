SHELL=/bin/sh

PREFIX=.
EXEC_PREFIX=$(PREFIX)

INCLUDEDIR=$(EXEC_PREFIX)/include
LIBDIR=$(EXEC_PREFIX)/lib
SRCDIR=$(EXEC_PREFIX)/src
TESTDIR=$(EXEC_PREFIX)/tests

CXXC=g++
LD=g++

CXX_FLAGS=-O0 -ggdb3 -I$(INCLUDEDIR) -std=c++17 -Wall -Wextra -Wpedantic
LD_FLAGS=

CXX_SRCS:=$(shell find $(SRCDIR) -name '*.cc')
OBJS:=${CXX_SRCS:.cc=.o}
TESTS:=$(shell find $(TESTDIR) -name '*.vpr')
TEST_OBJS:=${TESTS:.vpr=.asm}

TARGET=viper

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