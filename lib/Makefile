VIPC=../viper
AS=as

VIPFLAGS:=-L

VIP_SRCS:=$(shell find $(SRCDIR) -name '*.vip')
AS_SRCS:=$(shell find $(SRCDIR) -name '*.s')
OBJS:=${AS_SRCS:.s=.o}

TARGET=../std.vplib

all: $(TARGET)

$(TARGET): $(VIP_SRCS) $(OBJS)
	$(VIPC) $(VIPFLAGS) $^ -o $@

%.o: %.s
	$(AS) $< -o $@