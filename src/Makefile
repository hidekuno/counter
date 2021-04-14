#
# KERN_SRC=${WHERE} make
#
# hidekuno@gmail.com
#

KERNEL_SRC_DIR = /usr/src/$(KERN_SRC)
DEV_SRC_DIR = `pwd`

TARGET:= counter.ko sequence.ko counter2.ko

SRCS=counter_core.c counter_dev.c counter_proc.c counter_dev_sysfs.c

all: ${TARGET}

$(TARGET): $(SRCS)
	make -C $(KERNEL_SRC_DIR) M=$(DEV_SRC_DIR) modules

clean:
	make -C $(KERNEL_SRC_DIR) M=$(DEV_SRC_DIR) clean

obj-m := counter.o sequence.o counter2.o

counter-objs := counter_core.o counter_dev.o
sequence-objs := counter_core.o counter_proc.o
counter2-objs := counter_core.o counter_dev_sysfs.o

clean-files := *.o *.ko *.mod.[co]

