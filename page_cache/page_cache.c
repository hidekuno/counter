#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>
#include <linux/fdtable.h>
#include <linux/pagemap.h>

#define PROC_FILE_NAME "test_cache"
#define VERSION "0.0.1"



static void do_page_cache(struct task_struct *process)
{
   int i, hit = 0;
   struct  files_struct *files = process->files;
   
   for (i = 0; i < files->fdt->max_fds; i++) {
       if (files->fdt->fd[i]) {
           if (!strcmp(files->fdt->fd[i]->f_path.dentry->d_name.name, "test_file")) {
               hit = 1;
               break;
           }
       }
   }
   if (hit) {
       struct address_space *mapping = files->fdt->fd[i]->f_mapping;
       struct radix_tree_node *node  = mapping->page_tree.rnode;
       struct page            *page  = node;
       printk("%s", kmap(page));
   }
   return 0;
}

static size_t proc_write( struct file *filp, const char __user *buff,
                                   unsigned long len, void *data )
{
   char    _buff[64];

   if (copy_from_user(_buff, buff, len )) {
       return -EFAULT;
   }
   pid_t pid = simple_strtol(_buff, NULL, 0);

   struct task_struct *process = pid_task(find_vpid(pid), PIDTYPE_PID);
   if (process) { 
       do_page_cache(process);
   }
   else {
       printk("pid error: %d\n", pid);
       return -EFAULT;
   }
   return len;
}
static struct file_operations proc_ops = {
   .write = proc_write
};

static struct proc_dir_entry *proc;
static int proc_init_module(void)
{
   proc = create_proc_entry(PROC_FILE_NAME, 0666, (struct proc_dir_entry *) 0);
   if (proc) {
       proc->proc_fops = &proc_ops;
   }
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
