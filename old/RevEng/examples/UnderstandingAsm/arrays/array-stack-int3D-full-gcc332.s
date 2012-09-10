	.file	"array-stack-int3D.c"
	.text
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
.L33:
	xorl	%ecx, %ecx
	movl	12(%esp), %esi
.L32:
	xorl	%edx, %edx
	movl	%esi, %ebx
	leal	(%ecx,%esi), %edi
	.p2align 4,,15
.L31:
	cmpl	%edx, %ecx
	je	.L40
	leal	(%edx,%ebx), %eax
	movl	$0, 16(%esp,%eax,4)
.L27:
	incl	%edx
	cmpl	$9, %edx
	jle	.L31
	incl	%ecx
	addl	$10, %esi
	cmpl	$9, %ecx
	jle	.L32
	incl	%ebp
	addl	$100, 12(%esp)
	cmpl	$2, %ebp
	jle	.L33
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
.L40:
	movl	%ebp, 16(%esp,%edi,4)
	jmp	.L27
	.size	intArray3D, .-intArray3D
	.ident	"GCC: (GNU) 3.3.2"
