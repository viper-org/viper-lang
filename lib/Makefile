SHELL=/bin/sh

PREFIX=.
EXEC_PREFIX=$(PREFIX)

SRCDIR=$(EXEC_PREFIX)

ifeq ($(OS),Windows_NT)
	SRCDIR = $(EXEC_PREFIX)/win64/
else
	SRCDIR = $(EXEC_PREFIX)/linux64/
endif

VPR=../viper
AR=ar
AS=nasm

VPRFLAGS=
ARFLAGS=-rcs
ASFLAGS=-fwin64

VPR_SRCS:=$(shell find $(SRCDIR) -name '*.vpr')
ASM_SRCS:=$(shell find $(SRCDIR) -name '*.asm')

OBJS:=${VPR_SRCS:.vpr=.vpr.o} ${ASM_SRCS:.asm=.o}

TARGET=libvpr.a

all: $(TARGET)

%.vpr.o: %.vpr
	$(VPR) $(VPRFLAGS) $< > $<.asm
	$(AS) $(ASFLAGS) $<.asm -o $@
#	rm $<.asm

%.o: %.asm
	$(AS) $(ASFLAGS) $< -o $@

$(TARGET): $(OBJS)
	$(AR) $(ARFLAGS) $@ $^

clean:
	rm -rf $(TARGET) $(OBJS)