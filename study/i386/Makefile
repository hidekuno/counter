#
# Makefile
#
# hidekuno@gmail.com
#
TARGET = hello stck of

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

of: of.o
	gcc -m32 of.o -o of

of.o: of.c
	gcc -c -m32 -fno-stack-protector of.c

clean:
	rm -f $(TARGET) *.o
