	.file	"array-malloc-int1D.c"
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"Out of memory"
	.text
	.p2align 4,,15
.globl allocate1D
	.type	allocate1D, @function
allocate1D:
	subl	$24, %esp
	movl	28(%esp), %edx
	sall	$2, %edx
	pushl	%edx
	call	malloc
	addl	$16, %esp
	testl	%eax, %eax
	je	.L18
	addl	$12, %esp
	ret
.L18:
	subl	$12, %esp
	pushl	$.LC0
	call	puts
	movl	$1, (%esp)
	call	exit
	.size	allocate1D, .-allocate1D
	.p2align 4,,15
.globl intArray
	.type	intArray, @function
intArray:
	subl	$24, %esp
	pushl	$2048
	call	malloc
	addl	$16, %esp
	xorl	%edx, %edx
	testl	%eax, %eax
	je	.L29
	.p2align 4,,15
.L26:
	movl	%edx, (%eax,%edx,4)
	incl	%edx
	cmpl	$511, %edx
	jle	.L26
	subl	$12, %esp
	pushl	%eax
	call	printArray
	addl	$28, %esp
	ret
.L29:
	subl	$12, %esp
	pushl	$.LC0
	call	puts
	movl	$1, (%esp)
	call	exit
	.size	intArray, .-intArray
	.ident	"GCC: (GNU) 3.3.2"
