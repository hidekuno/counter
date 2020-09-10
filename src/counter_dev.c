/*
   This is prototype program what linux kernel module

   Usage:
     sudo insmod counter.ko
     sudo mknod /dev/counter c 90 1
     sudo cat /dev/counter

   hidekuno@gmail.com
*/
#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/fs.h>

#define VERSION "0.03"

static int devmajor = 90;
static char *devname = "counter";

module_param(devmajor, int, 0644);
module_param(devname, charp, 0644);

extern struct file_operations* get_couter_fops(void);

int counter_init_module(void) {

	int ret = register_chrdev(devmajor, devname, get_couter_fops());

	if (ret) {
		printk("counter registration error\n");
		return -EIO;
	}
	return 0;
}

void counter_cleanup_module(void) {

	unregister_chrdev(devmajor, devname);
}

module_init(counter_init_module);
module_exit(counter_cleanup_module);

MODULE_AUTHOR("Hideki Kuno <hidekuno@gmail.com>");
MODULE_DESCRIPTION("Test Driver");
MODULE_LICENSE("GPL");
MODULE_VERSION(VERSION);
