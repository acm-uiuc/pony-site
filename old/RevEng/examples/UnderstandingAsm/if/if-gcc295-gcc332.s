	.file	"if.c"
	.version	"01.01"
gcc2_compiled.:
.section	.rodata
.LC0:
	.string	"A is less than 0\n"
.text
	.align 4
.globl main
	.type	 main,@function
main:
	pushl %ebp
	movl %esp,%ebp
	subl $8,%esp
	movl 12(%ebp),%eax
	addl $-12,%esp
	pushl 4(%eax)
	call atoi
	addl $16,%esp
	testl %eax,%eax
	jge .L18
	addl $-12,%esp
	pushl $.LC0
	call printf
.L18:
	leave
	ret
.Lfe1:
	.size	 main,.Lfe1-main
	.ident	"GCC: (GNU) 2.95.4  (Debian prerelease)"
