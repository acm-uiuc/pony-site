	.file	"for.c"
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"%d\n"
	.text
	.p2align 4,,15
.globl main
	.type	main, @function
main:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	pushl	%eax
	andl	$-16, %esp
	xorl	%ebx, %ebx
	.p2align 4,,15
.L6:
	subl	$8, %esp
	pushl	%ebx
	pushl	$.LC0
	incl	%ebx
	call	printf
	addl	$16, %esp
	cmpl	$9, %ebx
	jle	.L6
	movl	-4(%ebp), %ebx
	leave
	ret
	.size	main, .-main
	.ident	"GCC: (GNU) 3.3.2"
