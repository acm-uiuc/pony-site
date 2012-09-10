	.file	"array-stack.c"
	.text
	.p2align 4,,15
.globl main
	.type	main, @function
main:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$8, %esp
	andl	$-16, %esp
	movl	%ebp, %esp
	popl	%ebp
	ret
	.size	main, .-main
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
	.p2align 4,,15
.globl intArray
	.type	intArray, @function
intArray:
	subl	$2060, %esp
	xorl	%eax, %eax
	.p2align 4,,15
.L23:
	movl	%eax, (%esp,%eax,4)
	incl	%eax
	cmpl	$511, %eax
	jle	.L23
	subl	$12, %esp
	leal	12(%esp), %edx
	pushl	%edx
	call	printArray
	addl	$2076, %esp
	ret
	.size	intArray, .-intArray
	.p2align 4,,15
.globl intArray2D
	.type	intArray2D, @function
intArray2D:
	pushl	%esi
	pushl	%ebx
	subl	$404, %esp
	xorl	%ecx, %ecx
	xorl	%ebx, %ebx
.L38:
	xorl	%edx, %edx
	leal	(%ecx,%ebx), %esi
	.p2align 4,,15
.L37:
	cmpl	%edx, %ecx
	je	.L43
	leal	(%edx,%ebx), %eax
	movl	$0, (%esp,%eax,4)
.L33:
	incl	%edx
	cmpl	$9, %edx
	jle	.L37
	incl	%ecx
	addl	$10, %ebx
	cmpl	$9, %ecx
	jle	.L38
	subl	$12, %esp
	leal	12(%esp), %edx
	pushl	%edx
	call	printArray
	addl	$420, %esp
	popl	%ebx
	popl	%esi
	ret
	.p2align 4,,15
.L43:
	movl	$1, (%esp,%esi,4)
	jmp	.L33
	.size	intArray2D, .-intArray2D
	.p2align 4,,15
.globl intArray3D
	.type	intArray3D, @function
intArray3D:
	pushl	%ebp
	pushl	%edi
	pushl	%esi
	pushl	%ebx
	subl	$1228, %esp
	xorl	%ebp, %ebp
	movl	$0, 12(%esp)
.L61:
	xorl	%ecx, %ecx
	movl	12(%esp), %esi
.L60:
	xorl	%edx, %edx
	movl	%esi, %ebx
	leal	(%ecx,%esi), %edi
	.p2align 4,,15
.L59:
	cmpl	%edx, %ecx
	je	.L68
	leal	(%edx,%ebx), %eax
	movl	$0, 16(%esp,%eax,4)
.L55:
	incl	%edx
	cmpl	$9, %edx
	jle	.L59
	incl	%ecx
	addl	$10, %esi
	cmpl	$9, %ecx
	jle	.L60
	incl	%ebp
	addl	$100, 12(%esp)
	cmpl	$2, %ebp
	jle	.L61
	subl	$12, %esp
	leal	28(%esp), %edx
	pushl	%edx
	call	printArray
	addl	$1244, %esp
	popl	%ebx
	popl	%esi
	popl	%edi
	popl	%ebp
	ret
	.p2align 4,,15
.L68:
	movl	%ebp, 16(%esp,%edi,4)
	jmp	.L55
	.size	intArray3D, .-intArray3D
	.ident	"GCC: (GNU) 3.3.2"
