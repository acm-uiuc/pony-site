	.file	"array-stack-char.c"
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"hello there, govna!"
	.text
	.p2align 4,,15
.globl charArray
	.type	charArray, @function
charArray:
	pushl	%ebx
	subl	$524, %esp
	pushl	$511
	pushl	$.LC0
	leal	12(%esp), %ebx
	pushl	%ebx
	call	strncpy
	movl	%ebx, (%esp)
	movb	$0, 35(%esp)
	call	printArray
	addl	$536, %esp
	popl	%ebx
	ret
	.size	charArray, .-charArray
	.ident	"GCC: (GNU) 3.3.2"
