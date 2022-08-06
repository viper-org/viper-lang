CXXC=/usr/bin/g++
LD=/usr/bin/g++
CXXFLAGS=-fsanitize=address,undefined -Og -ggdb3 -Iinclude -std=c++17 -Wall -Wextra -Wpedantic -lLLVM
LDFLAGS=-fsanitize=address,undefined -lLLVM

CXXSOURCES:=$(shell find src -name '*.cc')
OBJS:=${CXXSOURCES:.cc=.o}
TARGET=qrk

all: $(TARGET)

%.o: %.cc
	$(CXXC) $(CXXFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	$(LD) $(LDFLAGS) $^ -o $@

clean:
	rm -rf $(OBJS) $(TARGET)

test: $(TARGET)
	./$(TARGET) example.qrk

debug: $(TARGET)
	gdb --args ./${TARGET} example.qrk