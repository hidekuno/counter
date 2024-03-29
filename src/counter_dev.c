/*
   This is prototype program what linux kernel module

   Usage:
     sudo insmod counter_core.ko
     sudo insmod counter.ko
     sudo mknod /dev/counter c 90 1
     cat /dev/counter

   hidekuno@gmail.com
*/
#include <linux/module.h>
#include <linux/fs.h>

#define VERSION "1.00"

static int devmajor = 90;
static char *devname = "counter";

module_param(devmajor, int, 0644);
module_param(devname, charp, 0644);

extern struct file_operations* get_couter_fops(void);

static int __init counter_init_module(void) {

    int ret = register_chrdev(devmajor, devname, get_couter_fops());

    if (ret != 0) {
        printk("counter registration error\n");
        return -EIO;
    }
    printk("counter is loaded\n");
    return 0;
}

static void __exit counter_cleanup_module(void) {

    unregister_chrdev(devmajor, devname);

    printk("counter is unloaded\n");
}

module_init(counter_init_module);
module_exit(counter_cleanup_module);

MODULE_AUTHOR("Hideki Kuno <hidekuno@gmail.com>");
MODULE_DESCRIPTION("Test Driver");
MODULE_LICENSE("GPL");
MODULE_VERSION(VERSION);
