/*
   This is prototype program what FreeBSD kernel module

   hidekuno@gmail.com
*/
#include <sys/types.h>
#include <sys/systm.h>
#include <sys/param.h>
#include <sys/module.h>
#include <sys/kernel.h>
#include <sys/conf.h>

#define VERSION "0.01"
#define DEV_MAJOR 90

extern struct cdevsw* get_counter_cdevsw(void);

static struct cdev *counter_cdev;

static int counter_loader(struct module *m, int what, void *arg) {
    int error = 0;

    switch (what) {
    case MOD_LOAD:
        counter_cdev = make_dev(get_counter_cdevsw(), DEV_MAJOR, UID_ROOT, GID_WHEEL, 0644, "counter");
        if (counter_cdev == NULL) {
            error = ENXIO;
        }
        break;
    case MOD_UNLOAD:
        destroy_dev(counter_cdev);
        break;
    default:
        error = EOPNOTSUPP;
        break;
    }
    return error;
}

DEV_MODULE(counter, counter_loader, NULL);
