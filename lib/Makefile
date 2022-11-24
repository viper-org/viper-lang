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

VPRFLAGS=
ARFLAGS=-rcs
ASFLAGS=$(PLATFORM_ASFLAGS)

VPR_SRCS:=$(shell find $(SRCDIR) -name '*.vpr')
ASM_SRCS:=$(shell find $(SRCDIR) -name '*.asm')

OBJS:=${VPR_SRCS:.vpr=.vpr.o} ${ASM_SRCS:.asm=.o}

TARGET=libvpr.a

all: $(TARGET)

%.vpr.o: %.vpr
	$(VPR) $(VPRFLAGS) $<

%.o: %.asm
	$(AS) $(ASFLAGS) $< -o $@

$(TARGET): $(OBJS)
	$(AR) $(ARFLAGS) $@ $^

clean:
	rm -rf $(TARGET) $(OBJS)