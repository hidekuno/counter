/*
 * fake glic sample program
 *
 * gcc -fPIC -shared fake_mkdir.c -o fake.so  -ldl
 * LD_PRELOAD=./fake.so test-program
 *
 * hidekuno@gmail.com
 *
 */
#define _GNU_SOURCE
#include <dlfcn.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/select.h>
#include <string.h>

static const int TEST_DELAY = 10;
static int counter = 1;
static int (*mkdir_org)(const char *pathname, mode_t mode);

__attribute__((constructor)) void __get_origs()
{
    mkdir_org = (int (*)(const char*, mode_t))dlsym(RTLD_NEXT, "mkdir");
}

int mkdir(const char *pathname, mode_t mode) {

	if (!strcmp("pg_log",pathname)) {
		struct timeval t;
		int ret;
		t.tv_sec = TEST_DELAY; 
		t.tv_usec = 0; 
		ret = select(0,NULL,NULL,NULL,&t); 
	}
	return mkdir_org(pathname, mode);
}
