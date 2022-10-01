SHELL=/bin/sh

PREFIX=.
EXEC_PREFIX=$(PREFIX)

INCLUDEDIR=$(EXEC_PREFIX)/include
LIBDIR=$(EXEC_PREFIX)/lib
SRCDIR=$(EXEC_PREFIX)/src

CXXC=g++
LD=g++

CXX_FLAGS=-fsanitize=address,undefined -O0 -ggdb3 -I$(INCLUDEDIR) -std=c++20 -Wall -Wextra -Wpedantic
LD_FLAGS=-fsanitize=address,undefined

CXX_SRCS:=$(shell find $(SRCDIR) -name '*.cc')
OBJS:=${CXX_SRCS:.cc=.o}

TARGET=viper

all: $(TARGET)

%.o: %.cc
	$(CXXC) $(CXX_FLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	$(LD) $(LD_FLAGS) $^ -o $@

clean:
	rm -rf $(TARGET) $(OBJS)