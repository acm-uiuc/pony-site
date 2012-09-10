	.file	"ifelseif.c"
	.version	"01.01"
gcc2_compiled.:
.section	.rodata
.LC0:
	.string	"A is less than 0\n"
.LC1:
	.string	"A is 0\n"
.LC2:
	.string	"A > 0\n"
.LC3:
	.string	"Leaving main\n"
.text
	.align 4
.globl main
	.type	 main,@function
main:
	pushl %ebp
	movl %esp,%ebp
	subl $24,%esp

        
        /* "Jump past if body if -4(%ebp) ge 0" */
        cmpl $0,-4(%ebp)
	jge .L3

        /* code executed if (a > 0) */
	addl $-12,%esp
	pushl $.LC0
	call printf
	addl $16,%esp

        /* jump past else if and else clause */
	jmp .L4
	.p2align 4,,7
.L3:
        /* else.. */
        /* jump past elseif body if -4(%ebp) ne 0 */
	cmpl $0,-4(%ebp)
	jne .L5

        /* code executed if (a == 0 */
	addl $-12,%esp
	pushl $.LC1
	call printf
	addl $16,%esp

        /* Jump past else */
	jmp .L4
	.p2align 4,,7
.L5:
        /* else */
	addl $-12,%esp
	pushl $.LC2
	call printf
	addl $16,%esp
.L6:
.L4:
	addl $-12,%esp
	pushl $.LC3
	call printf
	addl $16,%esp
.L2:
	leave
	ret
.Lfe1:
	.size	 main,.Lfe1-main
	.ident	"GCC: (GNU) 2.95.4  (Debian prerelease)"
