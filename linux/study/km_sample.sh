#!/bin/bash 

set -e
set -o pipefail

sudo apt update
sudo apt install -y build-essential flex bison dwarves libssl-dev libelf-dev bc
cd /usr/src
sudo git clone https://github.com/microsoft/WSL2-Linux-Kernel.git
cd WSL2-Linux-Kernel
sudo git checkout linux-msft-wsl-5.15.y
sudo bash -c "cat /proc/config.gz  | gzip -dc > /usr/src/WSL2-Linux-Kernel/.config"
sudo make modules_prepare
sudo make scripts

cd $HOME
mkdir hello
cd hello

cat <<EOF>hello.c
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("John Doe");
MODULE_DESCRIPTION("A sample Linux kernel module");
MODULE_VERSION("0.1");

static int __init hello_init(void)
{
    printk(KERN_INFO "Hello, kernel\n");
    return 0;
}
static void __exit hello_exit(void)
{
    printk(KERN_INFO "Goodbye, kernel\n");
}
module_init(hello_init);
module_exit(hello_exit);
EOF
echo 'obj-m := hello.o' > Makefile
make -C /usr/src/WSL2-Linux-Kernel M=`pwd` modules
sudo insmod hello.ko
