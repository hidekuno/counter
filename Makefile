# centos
# yum install kernel-devel
#
KERNEL_SRC_DIR = /usr/src/kernels/2.6.18-410.el5-i686/
DEV_SRC_DIR = `pwd`
TARGET:= counter.ko

SRCS=counter.c
OBJS=counter.o
KOS=$(TARGET)

all: ${TARGET}

$(KOS): $(SRCS)
	make -C $(KERNEL_SRC_DIR) M=$(DEV_SRC_DIR) V=1 modules

clean:
	make -C $(KERNEL_SRC_DIR) M=$(DEV_SRC_DIR) V=1 clean

obj-m:= $(OBJS)

#counter-objs := counter.o

clean-files := *.o *.ko *.mod.[co]

