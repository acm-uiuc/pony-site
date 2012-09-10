	.file	"dowhile.c"
	.section	.rodata
.LC0:
	.string	"%d\n"
	.text
.globl main
	.type	main, @function
main:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$8, %esp
	andl	$-16, %esp
	movl	$0, %eax
	subl	%eax, %esp
	movl	$0, -4(%ebp)
.L2:
	subl	$8, %esp
	pushl	-4(%ebp)
	pushl	$.LC0
	call	printf
	addl	$16, %esp
	leal	-4(%ebp), %eax
	incl	(%eax)
	cmpl	$9, -4(%ebp)
	jle	.L2
	leave
	ret
	.size	main, .-main
	.ident	"GCC: (GNU) 3.3.2"
