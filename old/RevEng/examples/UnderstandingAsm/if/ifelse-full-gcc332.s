	.file	"ifelse.c"
	.section	.rodata.str1.32,"aMS",@progbits,1
	.align 32
.LC1:
	.string	"A is greater than or equal to 0"
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC2:
	.string	"Leaving main"
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
	js	.L5
	subl	$12, %esp
	pushl	$.LC1
.L4:
	call	puts
	movl	$.LC2, (%esp)
	call	puts
	movl	%ebp, %esp
	popl	%ebp
	ret
	.p2align 4,,15
.L5:
	subl	$12, %esp
	pushl	$.LC0
	jmp	.L4
	.size	main, .-main
	.ident	"GCC: (GNU) 3.3.2"
