	.file	"for.c"
	.version	"01.01"
gcc2_compiled.:
.section	.rodata
.LC0:
	.string	"%d\n"
.text
	.align 4
.globl main
	.type	 main,@function
main:
	pushl %ebp
	movl %esp,%ebp
	subl $20,%esp
	pushl %ebx
	xorl %ebx,%ebx
	.p2align 4,,7
.L21:
	addl $-8,%esp
	pushl %ebx
	pushl $.LC0
	call printf
	addl $16,%esp
	incl %ebx
	cmpl $9,%ebx
	jle .L21
	movl -24(%ebp),%ebx
	leave
	ret
.Lfe1:
	.size	 main,.Lfe1-main
	.ident	"GCC: (GNU) 2.95.4  (Debian prerelease)"
