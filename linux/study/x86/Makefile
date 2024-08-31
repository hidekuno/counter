#
# Makefile
#
# hidekuno@gmail.com
#
TARGET = hello hello64 stck of

all: $(TARGET)

hello: hello.o
	ld -m elf_i386 --entry=start hello.o -o hello

hello.o: hello.s
	as -g -32 hello.s -o hello.o

hello64: hello64.o
	ld --entry=_main hello64.o -o hello64

hello64.o: hello64.s
	as --defsym Linux=1 -g hello64.s -o hello64.o

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
