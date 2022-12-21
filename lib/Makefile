SHELL=/bin/sh

PREFIX=.
EXEC_PREFIX=$(PREFIX)

SRCDIR=$(EXEC_PREFIX)

PLATFORM_ASFLAGS=

ifeq ($(OS),Windows_NT)
	SRCDIR = $(EXEC_PREFIX)/win64/
	PLATFORM_ASFLAGS=-fwin64
else
	SRCDIR = $(EXEC_PREFIX)/linux64/
	PLATFORM_ASFLAGS=-felf64
endif

VPR=../vprc
AS=nasm

VPRFLAGS=-L
ASFLAGS=$(PLATFORM_ASFLAGS)

VPR_SRCS:=$(shell find $(SRCDIR) -name '*.vpr')
ASM_SRCS:=$(shell find $(SRCDIR) -name '*.asm')

OBJS:=${ASM_SRCS:.asm=.o}

TARGET=../std.vlib

all: $(TARGET)

%.o: %.asm
	$(AS) $(ASFLAGS) $< -o $@

$(TARGET): $(OBJS) $(VPR_SRCS)
	$(VPR) $(VPRFLAGS) $^ -o $(TARGET)

clean:
	rm -rf $(TARGET) $(OBJS)