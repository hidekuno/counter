/*
   This is prototype program what linux kernel module

   Usage:
     sudo insmod counter_core.ko
     sudo insmod counter_state.ko
     cat /proc/counter

   hidekuno@gmail.com
*/

#include <linux/module.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>

#define VERSION "0.03"
#define PROC_FILE_NAME "counter"

static struct proc_dir_entry *proc;
extern struct file_operations* get_couter_readonly_fops(void);

static int __init proc_init_module(void) {

#ifdef KERNEL_51X
    static struct proc_ops readonly_ops;
    const struct file_operations *fs_ops = get_couter_readonly_fops();
    readonly_ops.proc_read = fs_ops->read;
    readonly_ops.proc_open = fs_ops->open;
    readonly_ops.proc_release = fs_ops->release;
    proc = proc_create_data(PROC_FILE_NAME, 0444, NULL, &readonly_ops,
                            "Hello,World");
#else
    proc = proc_create_data(PROC_FILE_NAME, 0444, NULL, get_couter_readonly_fops(),
                            "Hello,World");
#endif
    return 0;
}

static void __exit proc_cleanup_module(void) {
    remove_proc_entry(PROC_FILE_NAME, (struct proc_dir_entry *) 0);
}

module_init(proc_init_module);
module_exit(proc_cleanup_module);

MODULE_AUTHOR("Hideki Kuno <hidekuno@gmail.com>");
MODULE_DESCRIPTION("Test Driver");
MODULE_LICENSE("GPL");
MODULE_VERSION(VERSION);
