/*
 * This is a vulnerability demo program. 
 *
 * gcc -fno-stack-protector -m32 of.c -o of
 *
 * hidekuno@gmail.com
 */
#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>

jmp_buf env;

void destroy_stack(void)
{
	puts("Buffer over flow.");
	longjmp(env, 1);
}
void do_test(void)
{
	int v[1];
	v[2] = (int)destroy_stack;
}
int main(int argc, char** argv)
{
	if (setjmp(env) == 0) {
		do_test();
	} else {
		puts("Non-Local exits.");
	}
	return 0;
}
