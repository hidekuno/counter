/*
 * fake glic sample program
 *
 * gcc -fPIC -shared fake_bind.c -o fake.so -ldl
 * LD_PRELOAD=./fake.so test-program
 *
 * hidekuno@gmail.com
 *
 */
#define _GNU_SOURCE
#include <dlfcn.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/select.h>

static const int TEST_DELAY = 30;
static int counter = 1;
static int (*bind_org)(int, const struct sockaddr *, socklen_t);

__attribute__((constructor)) void __get_origs()
{
    bind_org = (int (*)(int, const struct sockaddr *, socklen_t ))dlsym(RTLD_NEXT, "bind");
}

int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
    if (counter <= 1) {
        struct timeval t;
        int ret;
        t.tv_sec = TEST_DELAY; 
        t.tv_usec = 0; 
        ret = select(0,NULL,NULL,NULL,&t); 
        counter++;
    }
    return bind_org(sockfd, addr, addrlen);
}
