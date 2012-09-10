	.file	"ifelse.c"
	.version	"01.01"
gcc2_compiled.:
.section	.rodata
.LC0:
	.string	"A is less than 0\n"
	.align 32
.LC1:
	.string	"A is greater than or equal to 0\n"
.LC2:
	.string	"Leaving main\n"
.text
	.align 4
.globl main
	.type	 main,@function
main:
        /* function prolog */
	pushl %ebp
	movl %esp,%ebp
	subl $24,%esp

        /* "Jump if -4(%ebp) ge 0" -> jump if (a >= 0) */
	cmpl $0,-4(%ebp)
	jge .L3

        /* This code executed if (a < 0) */
	addl $-12,%esp
	pushl $.LC0
	call printf
	addl $16,%esp

        /* Jump past the else clause to the unconditionally executed code */
	jmp .L4
	.p2align 4,,7
.L3:
        /* else { */
	addl $-12,%esp
	pushl $.LC1
	call printf
	addl $16,%esp
.L4:
        /* Unconditionally executed printf */
	addl $-12,%esp
	pushl $.LC2
	call printf
	addl $16,%esp
.L2:
	leave
	ret
.Lfe1:
	.size	 main,.Lfe1-main
	.ident	"GCC: (GNU) 2.95.4  (Debian prerelease)"
