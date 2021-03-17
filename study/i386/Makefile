#
# Makefile
#
# hidekuno@gmail.com
#
TARGET = hello stck

all: $(TARGET)

hello: hello.o
	ld -m elf_i386 --entry=start hello.o -o hello

hello.o: hello.s
	as -g -32 hello.s -o hello.o

stck: stck.o
	gcc -m32 stck.o -o stck
	execstack -s stck

stck.o: stck.c
	gcc -c -m32 stck.c

clean:
	rm -f stck hello *.o
