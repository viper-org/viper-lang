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
AR=ar
AS=nasm

VPRFLAGS=-L
ARFLAGS=-rcs
ASFLAGS=$(PLATFORM_ASFLAGS)

VPR_SRCS:=$(shell find $(SRCDIR) -name '*.vpr')
ASM_SRCS:=$(shell find $(SRCDIR) -name '*.asm')

OBJS:=${ASM_SRCS:.asm=.o}

TARGET=libvpr.a

all: $(TARGET)

%.o: %.asm
	$(AS) $(ASFLAGS) $< -o $@

$(TARGET): $(OBJS) $(VPR_SRCS)
	$(AR) $(ARFLAGS) $@ $^
	$(VPR) -L $^ -o std.vplib

clean:
	rm -rf $(TARGET) $(OBJS)