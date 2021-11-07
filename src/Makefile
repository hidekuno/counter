# ex) 4.19.x, 5.4.x
# KERN_SRC=${WHERE} make
#
# ex) 5.10.x
# KERN_SRC=${WHERE} make CFLAGS=-DKERNEL_510
#
# hidekuno@gmail.com
#
KERNEL_SRC_DIR = /usr/src/$(KERN_SRC)
DEV_SRC_DIR = `pwd`

TARGET:= counter_core.ko counter.ko counter2.ko counter_state.ko

SRCS=counter_core.c counter_dev.c counter_proc.c counter_dev_sysfs.c

all: ${TARGET}

$(TARGET): $(SRCS)
	make -C $(KERNEL_SRC_DIR) M=$(DEV_SRC_DIR) EXTRA_CFLAGS=$(CFLAGS) modules

clean:
	make -C $(KERNEL_SRC_DIR) M=$(DEV_SRC_DIR) clean

obj-m := counter_core.o counter.o counter_state.o counter2.o

counter-objs := counter_dev.o
counter2-objs := counter_dev_sysfs.o
counter_state-objs := counter_proc.o

clean-files := *.o *.ko *.mod.[co] *.mod
