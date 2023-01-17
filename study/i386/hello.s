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
        .global start,len,print,env,itos,stoi,bit
        .local msg,buffer,LF,ONE,ZERO,ivalue

        //------------------------------------------------------------------------
        // return sub routine
        //------------------------------------------------------------------------
        .macro entry ssize
        pushl %ebp
        movl %esp,%ebp
        subl \ssize,%esp
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
        movl %ebp,%esp
        popl %ebp
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
        call \proc
        addl $4,%esp
        .endm

        //------------------------------------------------------------------------
        // call sub routine
        //------------------------------------------------------------------------
        .macro execute2 proc,prm1,prm2
        pushl \prm1
        pushl \prm2
        call \proc
        addl $8,%esp
        .endm

        .data
msg:    .string "Hello,World"
LF:     .byte 0x0a
ONE:    .byte 0x31
ZERO:   .byte 0x30
SPACE:  .byte 0x20
        .align 4
ivalue: .int 1234567890

        .bss
buffer: .space 1024

        .text
        //------------------------------------------------------------------------
        // len of string
        //------------------------------------------------------------------------
len:
        entry $4
        movl 8(%ebp),%eax
len_loop1:
        cmpb $0,(%eax)
        je len_break1
        inc %eax
        jmp len_loop1
len_break1:
        subl 8(%ebp),%eax
        return

        //------------------------------------------------------------------------
        // stdout
        //------------------------------------------------------------------------
print:
        entry $4
        execute len,8(%ebp)
        movl %eax,-4(%ebp)
        write 8(%ebp),-4(%ebp)
        write $LF,$1
        return

        //------------------------------------------------------------------------
        // print environment variable
        //------------------------------------------------------------------------
env:
        entry $4
        movl 8(%ebp),%ebx
        movl (%ebx),%ecx
        addl $2,%ecx
        imul $4,%ecx
        addl %ecx,%ebx
env_loop1:
        cmpl $0,(%ebx)
        je env_break1
        execute print (%ebx)
        addl $4,%ebx
        jmp env_loop1
env_break1:
        return

        //------------------------------------------------------------------------
        // integer -> string
        //------------------------------------------------------------------------
itos:
        entry $32
        leal -1(%ebp),%esi
        movb $0,(%esi)
        dec %esi

        movl 12(%ebp),%eax
        movl $10,%ebx
itos_loop1:
        cltd
        idiv %ebx
        orl $0x30,%edx
        movb %dl,(%esi)
        cmpl $0,%eax
        je itos_break1
        dec %esi
        jmp itos_loop1
itos_break1:
        movl 8(%ebp),%edi
        leal -1(%ebp),%ecx
        subl %esi,%ecx
        rep movsb
        return

        //------------------------------------------------------------------------
        // string -> integer
        //------------------------------------------------------------------------
stoi:
        entry $4
        movl 8(%ebp),%esi
        execute len %esi
        movl %eax,%ecx
        addl %ecx,%esi

        xorl %eax,%eax
        movl $1,%ebx
stoi_loop1:
        dec %esi
        movb (%esi),%dl
        andl $0x0f, %edx
        imul %ebx,%edx
        addl %edx,%eax

        dec %ecx
        jz stoi_break1

        imul $10,%ebx
        jmp stoi_loop1
stoi_break1:
        return

        //------------------------------------------------------------------------
        // print bit
        //------------------------------------------------------------------------
bit:
        entry $4
        movl 8(%ebp),%eax
        movl $32,%ecx
        movl $4,%ebx
bit_loop1:
        cmpl $0,%ecx
        jz bit_break1

        cmpl $0,%ebx
        jnz bit_next0
        write $SPACE,$1
        movl $4,%ebx
bit_next0:
        dec %ebx

        shll %eax
        jc bit_next1
        write $ZERO,$1
        jmp bit_next2
bit_next1:
        write $ONE,$1
bit_next2:
        dec %ecx
        jmp bit_loop1
bit_break1:
        write $LF,$1
        return

        //------------------------------------------------------------------------
        // main
        //------------------------------------------------------------------------
start:
        execute env %esp
        execute print $msg
        execute2 itos ivalue,$buffer
        movb $0,buffer+5
        execute stoi $buffer
        execute bit $10

        movl $1,%eax
        movl $0,%ebx
        int $0x80
