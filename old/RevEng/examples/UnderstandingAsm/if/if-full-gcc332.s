	.file	"if.c"
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"A is less than 0"
	.text
	.p2align 4,,15
.globl main
	.type	main, @function
main:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$8, %esp
	andl	$-16, %esp
	testl	%eax, %eax
	js	.L3
.L2:
	movl	%ebp, %esp
	popl	%ebp
	ret
	.p2align 4,,15
.L3:
	subl	$12, %esp
	pushl	$.LC0
	call	puts
	addl	$16, %esp
	jmp	.L2
	.size	main, .-main
	.ident	"GCC: (GNU) 3.3.2"
