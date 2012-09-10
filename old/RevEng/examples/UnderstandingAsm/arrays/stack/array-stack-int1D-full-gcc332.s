	.file	"array-stack-int1D.c"
	.text
	.p2align 4,,15
.globl intArray
	.type	intArray, @function
intArray:
	subl	$2060, %esp
	xorl	%eax, %eax
	.p2align 4,,15
.L21:
	movl	%eax, (%esp,%eax,4)
	incl	%eax
	cmpl	$511, %eax
	jle	.L21
	subl	$12, %esp
	leal	12(%esp), %edx
	pushl	%edx
	call	printArray
	addl	$2076, %esp
	ret
	.size	intArray, .-intArray
	.ident	"GCC: (GNU) 3.3.2"
