//========================================================================
// Usage:
//   insmod counter.ko
//   mknod /dev/counter c 90 1
//   cat /dev/counter
//========================================================================
#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/mm.h>
#include <asm/uaccess.h>

#define VERSION "0.01"

static unsigned int value = 0;
static unsigned int busy = 0;
static unsigned int eof = 0;
static int   devmajor = 90;
static char *devname = "counter";

module_param(devmajor, int, 0644);
module_param(devname, charp,0644);

static int counter_open (struct inode *inode, struct file *file){

	if (busy) { 
		printk("counter open error\n");
		return -EBUSY;
	}
	busy = 1;

	eof = 0;
	return 0;
}

static int counter_release (struct inode *inode,struct file *file) {
	busy = 0;
	return 0;
}
static ssize_t counter_read (struct file *file, char __user *buf, size_t count, loff_t *ppos) {
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

static ssize_t counter_write (struct file *file, const char __user *buf,  size_t count, loff_t *ppos) {
	char numstr[ 16 ];
	ssize_t len;

	if (10 <= count ) return -EINVAL;
	if(copy_from_user(numstr, buf, count)) return -EFAULT;


	sscanf(numstr, "%3d", &value);
	len = strlen(numstr) + 1;

	return len;
}
static struct file_operations counter_fops = {
	.owner     = THIS_MODULE,
	.read      = counter_read,
	.write     = counter_write,
	.open      = counter_open,
	.release   = counter_release,
};

int counter_init_module (void) {

	int ret = register_chrdev (devmajor, devname, &counter_fops);

	if ( ret ) {
		printk("counter registration error\n");
		return -EIO;
	}
	return 0;
}

void counter_cleanup_module (void) {

#if 0
/* Kernel 2.6.19 */
	int ret = unregister_chrdev (devmajor, devname );

	if ( ret ) {
		printk("counter unregister_chrdev error\n");
	}
#else
/* Kernel 2.6.23 */
	unregister_chrdev(devmajor, devname );
#endif
}

module_init(counter_init_module);
module_exit(counter_cleanup_module);

MODULE_AUTHOR("Hideki Kuno <hidekuno@gmail.com>");
MODULE_DESCRIPTION("Test Driver");
MODULE_LICENSE("GPL");
MODULE_VERSION(VERSION);
