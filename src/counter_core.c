/*
   This is prototype program what linux kernel module

   hidekuno@gmail.com
*/
#include <linux/fs.h>
#include <linux/uaccess.h>

static unsigned int value = 0;
static unsigned int busy = 0;
static unsigned int eof = 0;

#define MAX_LENGTH 10

static int counter_open(struct inode *inode, struct file *file) {

	if (busy) {
		printk("counter open error\n");
		return -EBUSY;
	}
	busy = 1;

	eof = 0;
	return 0;
}

static int counter_release(struct inode *inode, struct file *file) {
	busy = 0;
	return 0;
}

static ssize_t counter_read(struct file *file,
                            char __user * buf,
                            size_t count,
                            loff_t * ppos) {
	char numstr[16];
	ssize_t len;

	if (eof) {
		return 0;
    }

	sprintf(numstr, "%d\n", value++);
	len = strlen(numstr) + 1;

	if (MAX_LENGTH <= len) {
		return -EINVAL;
    }
	if (copy_to_user(buf, numstr, len)) {
        return -EFAULT;
    }
	eof = 1;
	return len;
}

static ssize_t counter_write(struct file *file, const char __user * buf,
			     size_t count, loff_t * ppos)
{
	char numstr[16];
	ssize_t len;

	if (MAX_LENGTH <= count) {
		return -EINVAL;
    }
	if (copy_from_user(numstr, buf, count)) {
		return -EFAULT;
    }
	sscanf(numstr, "%3d", &value);
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

struct file_operations* get_couter_fops(void) {
    return &counter_fops;
}