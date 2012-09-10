	.file	"array-stack-int2D.c"
	.text
	.p2align 4,,15
.globl intArray2D
	.type	intArray2D, @function
intArray2D:
	pushl	%esi
	pushl	%ebx
	subl	$404, %esp
	xorl	%ecx, %ecx
	xorl	%ebx, %ebx
.L28:
	xorl	%edx, %edx
	leal	(%ecx,%ebx), %esi
	.p2align 4,,15
.L27:
	cmpl	%edx, %ecx
	je	.L33
	leal	(%edx,%ebx), %eax
	movl	$0, (%esp,%eax,4)
.L23:
	incl	%edx
	cmpl	$9, %edx
	jle	.L27
	incl	%ecx
	addl	$10, %ebx
	cmpl	$9, %ecx
	jle	.L28
	subl	$12, %esp
	leal	12(%esp), %edx
	pushl	%edx
	call	printArray
	addl	$420, %esp
	popl	%ebx
	popl	%esi
	ret
	.p2align 4,,15
.L33:
	movl	$1, (%esp,%esi,4)
	jmp	.L23
	.size	intArray2D, .-intArray2D
	.ident	"GCC: (GNU) 3.3.2"
