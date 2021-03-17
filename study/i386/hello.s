/*
 * 32bit assembly learning program
 *
 * how to build)
 *   1. as -g -32 hello.s -o hello.o
 *   2. ld -m elf_i386 --entry=start hello.o -o hello
 *
 * hidekuno@gmail.com
 *
 */
	.global start,length,print,env,itos,stoi,bit
	.local msg,buffer,LF,ONE,ZERO,ivalue

	//------------------------------------------------------------------------
	// return sub routine
	//------------------------------------------------------------------------
	.macro entry ssize

	pushl %ebp
	movl %esp,%ebp
	subl \ssize, %esp
	pushl %ebx
	pushl %ecx
	pushl %edx

	.endm

	//------------------------------------------------------------------------
	// return sub routine
	//------------------------------------------------------------------------
	.macro return

	popl %edx
	popl %ecx
	popl %ebx
	leave
	ret

	.endm

	//------------------------------------------------------------------------
	// system call( write )
	//------------------------------------------------------------------------
	.macro write s,l

	pushl %eax
	pushl %ebx
	pushl %ecx
	pushl %edx

	movl $4,%eax
	movl $1,%ebx
	movl \s,%ecx
	movl \l,%edx
	int $0x80

	popl %edx
	popl %ecx
	popl %ebx
	popl %eax

	.endm

	//------------------------------------------------------------------------
	// call sub routine
	//------------------------------------------------------------------------
	.macro execute proc,prm

	pushl \prm
	call  \proc
	addl $4,%esp

	.endm

	//------------------------------------------------------------------------
	// call sub routine
	//------------------------------------------------------------------------
	.macro execute2 proc,prm1,prm2

	pushl \prm1
	pushl \prm2
	call  \proc
	addl $8,%esp

	.endm

	.data
msg:	.string "Hello,World"
LF:	.byte 0x0a
ONE:	.byte 0x31
ZERO:	.byte 0x30
ivalue:	.int 1234567890

	.bss
buffer: .space 1024

	.text
	//------------------------------------------------------------------------
	// length of string
	//------------------------------------------------------------------------
length:
	entry $4
	movl 8(%ebp), %eax
loop1:
	cmpb $0, (%eax)
	je break1
	inc %eax
	jmp loop1
break1:
	subl 8(%ebp), %eax
	return

	//------------------------------------------------------------------------
	// stdout
	//------------------------------------------------------------------------
print:
	entry $4
	execute length, 8(%ebp)
	movl %eax,-4(%ebp)
	write 8(%ebp), -4(%ebp)
	write $LF,$1
	return

	//------------------------------------------------------------------------
	// print environment variable
	//------------------------------------------------------------------------
env:
	entry $4
	movl 8(%ebp), %ebx
	movl (%ebx),%ecx
	addl $2,%ecx
	imul $4,%ecx
	addl %ecx,%ebx
loop2:
	cmpl $0,(%ebx)
	je break2
	execute print (%ebx)
	addl $4,%ebx
	jmp loop2
break2:
	return

	//------------------------------------------------------------------------
	// integer -> string
	//------------------------------------------------------------------------
itos:
	entry $32
	leal -1(%ebp), %esi
	movb $0,(%esi)
	dec %esi

	movl 12(%ebp),%eax
	movl $10,%ebx
loop3:
	cltd
	idiv %ebx
	orl $0x30,%edx
	movb %dl, (%esi)
	cmpl $0,%eax
	je break3
	dec %esi
	jmp loop3
break3:
	movl 8(%ebp),%edi
	leal -1(%ebp), %ecx
	subl %esi,%ecx
	rep movsb
	return

	//------------------------------------------------------------------------
	// string -> integer
	//------------------------------------------------------------------------
stoi:
	entry $4
	movl 12(%ebp), %esi
	movl 8(%ebp),%ecx
	addl %ecx, %esi
	dec %esi

	movl $0,%eax
	movl $1,%ebx
	movl $0,%edx
loop4:
	cmpl %esi,12(%ebp)
	jg break4
	movzbl (%esi),%edx
	andl $0x0F,%edx
	imul %ebx,%edx
	addl %edx,%eax
	dec %esi
	imul $10,%ebx
	jmp loop4
break4:
	return

	//------------------------------------------------------------------------
	// print bit
	//------------------------------------------------------------------------
bit:
	entry $4
	movl 8(%ebp), %eax
	movl $32,%ecx
loop5:
	cmpl $0,%ecx
	je break5
	shll %eax
	jc next5_1
	write $ZERO, $1
	jmp next5_2
next5_1:
	write $ONE, $1
next5_2:
	dec %ecx
	jmp loop5
break5:
	write $LF, $1
	return

	//------------------------------------------------------------------------
	// main
	//------------------------------------------------------------------------
start:
	execute env %esp
	execute print $msg
	execute2 itos ivalue, $buffer
	movb $0, buffer + 5
	execute2 stoi $buffer, $5
	execute bit $10

	movl $1, %eax
	movl $0, %ebx
	int $0x80
