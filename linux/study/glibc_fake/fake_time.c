/*
 * fake glic sample program
 *
 * gcc -fPIC -shared fake_time.c -o fake.so  -ldl
 * LD_PRELOAD=./fake.so test-program
 *
 * hidekuno@gmail.com
 *
 */
#define _GNU_SOURCE
#include <dlfcn.h>
#include <time.h>

typedef time_t (*glibc_time_type)();
time_t time(time_t *t) {

    glibc_time_type glibc_time;
    glibc_time = (glibc_time_type)dlsym(RTLD_NEXT, "time");

    return glibc_time(t) + (3600 * 24 * 30);
}
