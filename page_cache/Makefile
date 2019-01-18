# centos
# yum install kernel-devel
#
KERNEL_SRC_DIR = /usr/src/kernels/3.10.0-957.1.3.el7.x86_64/
DEV_SRC_DIR = `pwd`
TARGET:= page_cache.ko

SRCS=page_cache.c
OBJS=page_cache.o
KOS=$(TARGET)

all: ${TARGET}

$(KOS): $(SRCS)
	make -C $(KERNEL_SRC_DIR) M=$(DEV_SRC_DIR) V=1 modules

clean:
	make -C $(KERNEL_SRC_DIR) M=$(DEV_SRC_DIR) V=1 clean

obj-m:= $(OBJS)

#page_cache-objs := page_cache.o

clean-files := *.o *.ko *.mod.[co]

