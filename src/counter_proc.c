/*
   This is prototype program what linux kernel module

   Usage
     sudo insmod sequence.ko
     cat /proc/sequence

   hidekuno@gmail.com
*/

#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>

#define VERSION "0.03"
#define PROC_FILE_NAME "sequence"

static struct proc_dir_entry *proc;
extern struct file_operations* get_couter_fops(void);

static int proc_init_module(void) {
	proc = proc_create_data(PROC_FILE_NAME, 0666, NULL, get_couter_fops(),
				"Hello,World");
	return 0;
}

static void proc_cleanup_module(void) {
	remove_proc_entry(PROC_FILE_NAME, (struct proc_dir_entry *) 0);
}

module_init(proc_init_module);
module_exit(proc_cleanup_module);

MODULE_AUTHOR("Hideki Kuno <hidekuno@gmail.com>");
MODULE_DESCRIPTION("Test Driver");
MODULE_LICENSE("GPL");
MODULE_VERSION(VERSION);
