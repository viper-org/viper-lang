CXXC=g++
LD=g++

DEBUGCXXFLAGS=-fsanitize=address,undefined -Og -ggdb3 -Iinclude -std=gnu++20 -Wall -Wextra -Wpedantic -lLLVM
RELEASECXXFLAGS=-O3 -Iinclude -std=gnu++20 -Wall -Wextra -Wpedantic -lLLVM

DEBUGLDFLAGS=-fsanitize=address,undefined -lLLVM
RELEASELDFLAGS=-lLLVM

release: CXXFLAGS=$(RELEASECXXFLAGS)
release: LDFLAGS=$(RELEASELDFLAGS)

debug: CXXFLAGS=$(DEBUGCXXFLAGS)
debug: LDFLAGS=$(DEBUGLDFLAGS)

CXXSOURCES:=$(shell find src -name '*.cc')
OBJS:=${CXXSOURCES:.cc=.o}
TARGET=qrk

all: $(TARGET)

release: $(OBJS)
	$(LD) $(LDFLAGS) $^ -o $(TARGET)

debug: $(OBJS)
	$(LD) $(LDFLAGS) $^ -o $(TARGET)

%.o: %.cc
	$(CXXC) $(CXXFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	$(LD) $(LDFLAGS) $^ -o $@

clean:
	rm -rf $(OBJS) $(TARGET)

example: $(TARGET)
	./$(TARGET) example.qrk