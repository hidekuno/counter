/*
   This is prototype program what linux kernel module

   hidekuno@gmail.com
*/
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

static unsigned int value = 0;
static unsigned int busy = 0;
static unsigned int eof = 0;

#define MAX_LENGTH (8)
#define MAX_VALUE (99999999)
#define VERSION "1.00"

static int counter_open(struct inode *inode, struct file *file)
{

    if (busy) {
        printk("counter open error\n");
        return -EBUSY;
    }
    busy = 1;

    eof = 0;
    return 0;
}

static int counter_release(struct inode *inode, struct file *file)
{
    busy = 0;
    return 0;
}
static ssize_t counter_read_impl(struct file *file,
                                 char __user * buf,
                                 size_t count,
                                 loff_t * ppos)
{
    char numstr[16];
    ssize_t len;

    if (MAX_VALUE < value) {
        value = 0;
    }

    sprintf(numstr, "%d\n", value);
    len = strlen(numstr) + 1;

    if (copy_to_user(buf, numstr, len)) {
        return -EFAULT;
    }
    eof = 1;
    return len;
}
static ssize_t counter_read(struct file *file,
                            char __user * buf,
                            size_t count,
                            loff_t * ppos)
{
    int ret = 0;
    if (eof) {
        return 0;
    }
    ret = counter_read_impl(file, buf, count, ppos);
    value++;
    return ret;
}
static ssize_t counter_readonly(struct file *file,
                                char __user * buf,
                                size_t count,
                                loff_t * ppos)
{
    if (eof) {
        return 0;
    }
    return counter_read_impl(file, buf, count, ppos);
}
static ssize_t counter_write(struct file *file,
                             const char __user * buf,
                             size_t count,
                             loff_t * ppos)
{
    char numstr[16];
    ssize_t len;

    if (MAX_LENGTH < count) {
        return -EINVAL;
    }
    if (copy_from_user(numstr, buf, count)) {
        return -EFAULT;
    }
    sscanf(numstr, "%d", &value);
    len = strlen(numstr) + 1;

    return len;
}
static struct file_operations counter_fops = {
    .owner = THIS_MODULE,
    .read = counter_read,
    .write = counter_write,
    .open = counter_open,
    .release = counter_release,
};

static struct file_operations counter_readonly_fops = {
    .owner = THIS_MODULE,
    .read = counter_readonly,
    .open = counter_open,
    .release = counter_release,
};

struct file_operations *get_couter_fops(void)
{
    return &counter_fops;
}
struct file_operations *get_couter_readonly_fops(void)
{
    return &counter_readonly_fops;
}
EXPORT_SYMBOL(get_couter_fops);
EXPORT_SYMBOL(get_couter_readonly_fops);

#define VERSION "1.00"

static int __init counter_init_module(void) {
    return 0;
}
static void __exit counter_cleanup_module(void) {
    return;
}
module_init(counter_init_module);
module_exit(counter_cleanup_module);

MODULE_AUTHOR("Hideki Kuno <hidekuno@gmail.com>");
MODULE_DESCRIPTION("Test Driver");
MODULE_LICENSE("GPL");
MODULE_VERSION(VERSION);
