// gcc -fPIC -shared fake.c -o fake.so -ldl
// LD_PRELOAD=./fake.so /home/quattro-exec/vm_application/bin/uidlist_update.pl
#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>

typedef int (*glibc_open_type)();
typedef int (*glibc_lseek_type)();

const char* case_1_3_4 = "/tmp/dovecot-uidlist";

static int call_count = 0;
int open64(const char *pathname, int flags, mode_t mode) {

    glibc_open_type glibc_open;
    glibc_open = (glibc_open_type)dlsym(RTLD_NEXT, "open64");

    if (!strcmp(case_1_3_4, pathname)) {
        call_count++;
        printf("call fake open = %s %d\n", pathname, call_count);
        if (call_count > 1) {
            errno = 2;
            return -1;
        }
    }
    return glibc_open(pathname,flags,mode);
}
const int MAX_FILENAME_SIZE = 1024;
inline int test_case_1_3_7(char* filename, off64_t offset, int whence) {
    return !strcmp(filename,"/tmp/dovecot-uidlist") && (offset == 0) && (whence == 0);
}
static const char* fd_to_filename(int fd, char* filename) {
    char fdlink[MAX_FILENAME_SIZE];
    sprintf(fdlink, "/proc/%d/fd/%d",getpid(),fd);
    size_t len = readlink(fdlink, filename, MAX_FILENAME_SIZE-1);
    //readlinkÇÕnull terminateÇµÇ»Ç¢ÅB
    filename[len] = '\0';
    return filename;
}
off_t lseek64(int fildes, off64_t offset, int whence) {
    char filename[MAX_FILENAME_SIZE];
    glibc_lseek_type glibc_lseek;
    glibc_lseek = (glibc_lseek_type)dlsym(RTLD_NEXT, "lseek64");
    fd_to_filename(fildes,filename);
    if (test_case_1_3_7(filename, offset, whence)) {
        call_count++;
        printf("%d %ld %d %s %d hook\n", fildes, offset, whence, filename,call_count);
        if (call_count == 2) {
            errno = EINVAL;
            return -1;
        }
    }
    return glibc_lseek(fildes, offset, whence);
}
