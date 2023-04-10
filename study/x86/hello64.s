        /*
         * 64bit assembly learning program
         *
         * How to build
         * Linux)
         *   1. as --defsym Linux=1 -g hello64.s -o hello64.o
         *   2. ld --entry=_main hello64.o -o hello64
         *
         *  MacOS)
         *   clang -Wa,-defsym,Linux=0 hello64.s -o hello64
         *
         * hidekuno@gmail.com
         *
         */
        .globl _main

        //------------------------------------------------------------------------
        // entry macro
        //------------------------------------------------------------------------
        .macro entry ssize
        pushq %rbp
        movq %rsp,%rbp
        subq \ssize,%rsp
        pushq %rbx
        pushq %rcx
        pushq %rdx
        .endm

        //------------------------------------------------------------------------
        // return macro
        //------------------------------------------------------------------------
        .macro return
        popq %rdx
        popq %rcx
        popq %rbx
        movq %rbp,%rsp
        popq %rbp
        ret
        .endm

        //------------------------------------------------------------------------
        // system call( write )
        //------------------------------------------------------------------------
        .macro write s, l
        pushq %rax
        pushq %rdi
        pushq %rsi
        pushq %rdx

        movq SYSCALL_WRITE(%rip), %rax
        movq $1, %rdi
        movq \s, %rsi
        movq \l, %rdx
        syscall

        popq %rdx
        popq %rsi
        popq %rdi
        popq %rax
        .endm

        //------------------------------------------------------------------------
        // call sub routine
        //------------------------------------------------------------------------
        .macro execute proc, prm
        pushq \prm
        call \proc
        addq $8,%rsp
        .endm

        //------------------------------------------------------------------------
        // call sub routine
        //------------------------------------------------------------------------
        .macro execute2 proc,prm1,prm2
        pushq \prm1
        pushq \prm2
        call \proc
        addq $16,%rsp
        .endm

        .data
msg:    .string "Hello,World"
LF:     .byte 0x0A

.if Linux == 1
SYSCALL_WRITE: .int 1
SYSCALL_EXIT:  .int 60
.else
SYSCALL_WRITE: .int 0x2000004
SYSCALL_EXIT:  .int 0x2000001
.endif

        .text
        //------------------------------------------------------------------------
        // length of string
        //------------------------------------------------------------------------
len:
        entry $8
        movq 16(%rbp),%rax
len_loop1:
        cmpb $0,(%rax)
        je len_break1
        inc %rax
        jmp len_loop1
len_break1:
        subq 16(%rbp),%rax
        return

        //------------------------------------------------------------------------
        // stdout
        //------------------------------------------------------------------------
print:
        entry $8
        movq 16(%rbp),%rbx
        execute len,%rbx
        movq %rax,-8(%rbp)
        write %rbx,-8(%rbp)

        leaq LF(%rip),%rbx
        write %rbx,$1
        return

_main:
        leaq msg(%rip),%rbx
        execute print, %rbx

        movq SYSCALL_EXIT(%rip), %rax
        movq $0, %rdi
        syscall
