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
	subl $16,%esp
	pushl %esi
	pushl %ebx
	movl 12(%ebp),%esi
	xorl %ebx,%ebx
	jmp .L18
	.p2align 4,,7
.L21:
	addl $-8,%esp
	pushl %ebx
	pushl $.LC0
	call printf
	addl $16,%esp
	incl %ebx
.L18:
	addl $-12,%esp
	pushl 4(%esi)
	call atoi
	addl $16,%esp
	cmpl %eax,%ebx
	jl .L21
	leal -24(%ebp),%esp
	popl %ebx
	popl %esi
	leave
	ret
.Lfe1:
	.size	 main,.Lfe1-main
	.ident	"GCC: (GNU) 2.95.4  (Debian prerelease)"
