//========================================================================
// Usage:
//  sudo insmod counter.ko
//  sudo rmmod counter
//========================================================================
#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/proc_fs.h>

#define PROC_FILE_NAME "counter"
#define VERSION "0.0.1"

static unsigned int value = 0;
static unsigned int busy = 0;
static unsigned int eof = 0;

static int counter_open(struct inode *inode, struct file *file){

	if (busy) { 
		printk("counter open error\n");
		return -EBUSY;
	}
	busy = 1;

	eof = 0;
	return 0;
}
static int counter_release(struct inode *inode,struct file *file) {
	busy = 0;
	return 0;
}
static ssize_t counter_read(struct file *file, char __user *buf, size_t count, loff_t *ppos) {
	char numstr[ 16 ];
	ssize_t len;

	if ( eof ) return 0;

	sprintf (numstr, "%d\n", value++);
	len = strlen(numstr) + 1;

	if (10 <= len ) return -EINVAL;
	if ( copy_to_user( buf, numstr, len)) return -EFAULT;

	eof = 1;
	return len;
}
static ssize_t counter_write(struct file *file, const char __user *buf,  size_t count, loff_t *ppos) {

	char numstr[ 16 ];
	ssize_t len;

	if (10 <= count ) return -EINVAL;
	if(copy_from_user(numstr, buf, count)) return -EFAULT;

	sscanf(numstr, "%3d", &value);
	len = strlen(numstr) + 1;

	return len;
}
static struct file_operations proc_ops = {
    .owner   = THIS_MODULE,
    .open    = counter_open,
    .write   = counter_write,
    .read    = counter_read,
    .release = counter_release
};

static struct proc_dir_entry *proc;
static int proc_init_module(void)
{
    proc = proc_create_data(PROC_FILE_NAME, 0666, NULL, &proc_ops,  "Hello,World");
    return 0;
}

static void proc_cleanup_module(void)
{
    remove_proc_entry(PROC_FILE_NAME, (struct proc_dir_entry *) 0);
}

module_init(proc_init_module);
module_exit(proc_cleanup_module);

MODULE_AUTHOR("Hideki Kuno <hidekuno@gmail.com>");
MODULE_DESCRIPTION("Test Driver");
MODULE_LICENSE("GPL");
MODULE_VERSION(VERSION);
