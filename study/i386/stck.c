/*
 * execute stack segment test program
 *
 * Requirement)
 *   1. linux 3.10 >= this
 *   2. gcc 4.8.5 >= this
 *   (this is written on CentOS{5,6,7})
 *
 * How to build)
 *   1. gcc -m32 stck.c -o stck
 *   2. execstack -s stck
 *
 * hidekuno@gmail.com
 */
#include <string.h>

char msg[] = "Hello, World\n";

void do_stack(size_t l) {

    __asm__("pushl %eax");
    __asm__("pushl %ebx");
    __asm__("pushl %ecx");
    __asm__("pushl %edx");

    __asm__("movl $4,%eax");
    __asm__("movl $1,%ebx");
    __asm__("movl $msg,%ecx");
    __asm__("movl 8(%ebp),%edx");
    __asm__("int $0x80");

    __asm__("popl %edx");
    __asm__("popl %ecx");
    __asm__("popl %ebx");
    __asm__("popl %eax");
}
int main(int argc, char** argv) {

    char stack[128];

    memcpy(stack, do_stack, (size_t)main - (size_t)do_stack);
    ((void (*)(size_t))stack)(strlen(msg));
}
