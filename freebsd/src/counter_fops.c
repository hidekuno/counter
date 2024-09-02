/*
   This is prototype program what FreeBSD kernel module

   hidekuno@gmail.com
*/
#include <sys/param.h>
#include <sys/systm.h>
#include <sys/conf.h>
#include <sys/uio.h>
#include <sys/ctype.h>

static unsigned int value = 0;
static unsigned int busy = 0;
static unsigned int eof = 0;

#define MAX_LENGTH (10)
#define MAX_VALUE (99999999)
#define SUCCESS (0)

struct cdevsw *get_counter_cdevsw(void);

static int counter_open(struct cdev *dev, int oflags, int devtype, struct thread *td) {
    if (busy) {
        printf("counter open error\n");
        return EBUSY;
    }
    busy = 1;
    eof = 0;
    return SUCCESS;
}

static int counter_close(struct cdev *dev, int fflag, int devtype, struct thread *td) {
    busy = 0;
    return SUCCESS;
}

static int counter_read(struct cdev *dev, struct uio *uio, int ioflag) {
    char numstr[16];
    size_t len;

    /* uprintf("read counter start\n"); */
    if (eof) {
        return 0;
    }
    if (MAX_VALUE < value) {
        value = 0;
    }

    sprintf(numstr, "%d\n", value++);
    len = strlen(numstr) + 1;

    int error = uiomove(numstr, len, uio);
    if (error != 0) {
        return error;
    }
    eof = 1;
    /* uprintf("read counter end %d\n", value); */
    return SUCCESS;
}

static int counter_write(struct cdev *dev, struct uio *uio, int ioflag) {
    char numstr[16];

    if (MAX_LENGTH <= uio->uio_resid) {
        return EINVAL;
    }

    int error = uiomove(numstr, uio->uio_resid, uio);
    if (error != 0) {
        return error;
    }

    /* Not support whitespace('\n','\r'...) */
    for (int i = 0; i < uio->uio_offset; ++i) {
        if (0 == isdigit(numstr[i])) {
            return EINVAL;
        }
    }

    sscanf(numstr, "%10d", &value);
    return SUCCESS;
}

static struct cdevsw counter_cdevsw = {
    .d_version = D_VERSION,
    .d_open = counter_open,
    .d_close = counter_close,
    .d_read = counter_read,
    .d_write = counter_write,
    .d_name = "counter",
};

struct cdevsw *get_counter_cdevsw(void) {
    return &counter_cdevsw;
}
