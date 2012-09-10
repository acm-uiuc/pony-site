	.file	"if.c"
	.section	.rodata
.LC0:
	.string	"A is less than 0\n"
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
	cmpl	$0, -4(%ebp)
	jns	.L2
	subl	$12, %esp
	pushl	$.LC0
	call	printf
	addl	$16, %esp
.L2:
	leave
	ret
	.size	main, .-main
	.ident	"GCC: (GNU) 3.3.2"
