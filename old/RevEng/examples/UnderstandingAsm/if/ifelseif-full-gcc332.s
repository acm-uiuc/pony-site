	.file	"ifelseif.c"
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC1:
	.string	"A is 0"
.LC3:
	.string	"Leaving main"
.LC2:
	.string	"A > 0"
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
	js	.L7
	testl	%eax, %eax
	jne	.L4
	subl	$12, %esp
	pushl	$.LC1
.L6:
	call	puts
	movl	$.LC3, (%esp)
	call	puts
	movl	%ebp, %esp
	popl	%ebp
	ret
	.p2align 4,,15
.L4:
	subl	$12, %esp
	pushl	$.LC2
	jmp	.L6
	.p2align 4,,15
.L7:
	subl	$12, %esp
	pushl	$.LC0
	jmp	.L6
	.size	main, .-main
	.ident	"GCC: (GNU) 3.3.2"
