/*
   This is prototype program what linux kernel module

   Usage:
     sudo insmod counter2.ko
     cat /dev/counter

   hidekuno@gmail.com
*/
#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>

#define VERSION "0.01"

#define DEV_MINOR (0)
#define MINOR_NUM (1)
#define PERMISSION (0644)

static char *devname = "counter";
static unsigned int counter_major;
static struct cdev counter_cdev;
static struct class *counter_class = NULL;

extern struct file_operations* get_couter_fops(void);

static char *devnode(struct device *dev, umode_t *mode) {
    if (mode && PERMISSION) {
        *mode = PERMISSION;
    }
    return NULL;
}
static int __init counter_init_module(void) {
    int ret = 0;
    dev_t dev = -1;
    counter_cdev.dev = -1;
    
    ret = alloc_chrdev_region(&dev, DEV_MINOR, MINOR_NUM, devname);
    if (ret != 0) {
        printk("counter alloc_chrdev_region error\n");
        return -1;
    }

    counter_major = MAJOR(dev);
    dev = MKDEV(counter_major, DEV_MINOR);

    cdev_init(&counter_cdev, get_couter_fops());
    counter_cdev.owner = THIS_MODULE;

    ret = cdev_add(&counter_cdev, dev, MINOR_NUM);
    if (ret != 0) {
        printk("counter cdev_add error\n");
        goto ERROR;
    }
    counter_class = class_create(THIS_MODULE, devname);
    if (IS_ERR(counter_class)) {
        printk("counter class_create error\n");
        goto ERROR;
    }
    counter_class->devnode = devnode;

    device_create(counter_class, NULL, MKDEV(counter_major, DEV_MINOR), NULL, devname);

    return ret;

ERROR:
    if (counter_cdev.dev != -1) {
        cdev_del(&counter_cdev);
    }
    if (dev != -1) {
        unregister_chrdev_region(dev, MINOR_NUM);
    }
    return ret;
}
static void __exit counter_cleanup_module(void) {

    dev_t dev = MKDEV(counter_major, DEV_MINOR);
    
    device_destroy(counter_class, MKDEV(counter_major, DEV_MINOR));

    class_destroy(counter_class);

    cdev_del(&counter_cdev);

    unregister_chrdev_region(dev, MINOR_NUM);
}

module_init(counter_init_module);
module_exit(counter_cleanup_module);

MODULE_AUTHOR("Hideki Kuno <hidekuno@gmail.com>");
MODULE_DESCRIPTION("Test Driver");
MODULE_LICENSE("GPL");
MODULE_VERSION(VERSION);
