	.file	"array-malloc-int2D.c"
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"Not enough space"
	.text
	.p2align 4,,15
.globl allocate2D
	.type	allocate2D, @function
allocate2D:
	pushl	%ebp
	pushl	%edi
	pushl	%esi
	pushl	%ebx
	subl	$24, %esp
	movl	44(%esp), %edi
	leal	0(,%edi,4), %edx
	movl	48(%esp), %ebp
	pushl	%edx
	call	malloc
	addl	$16, %esp
	testl	%eax, %eax
	movl	%eax, %esi
	je	.L26
	xorl	%ebx, %ebx
	cmpl	%edi, %ebx
	jl	.L23
.L25:
	addl	$12, %esp
	popl	%ebx
	movl	%esi, %eax
	popl	%esi
	popl	%edi
	popl	%ebp
	ret
	.p2align 4,,15
.L23:
	subl	$12, %esp
	leal	0(,%ebp,4), %edx
	pushl	%edx
	call	malloc
	addl	$16, %esp
	testl	%eax, %eax
	movl	%eax, (%esi,%ebx,4)
	je	.L26
	incl	%ebx
	cmpl	%edi, %ebx
	jl	.L23
	jmp	.L25
.L26:
	subl	$12, %esp
	pushl	$.LC0
	call	puts
	movl	$1, (%esp)
	call	exit
	.size	allocate2D, .-allocate2D
	.p2align 4,,15
.globl intArray2D
	.type	intArray2D, @function
intArray2D:
	pushl	%esi
	pushl	%ebx
	subl	$16, %esp
	pushl	$40
	call	malloc
	xorl	%ebx, %ebx
	addl	$16, %esp
	testl	%eax, %eax
	movl	%eax, %esi
	je	.L54
	.p2align 4,,15
.L34:
	subl	$12, %esp
	pushl	$40
	call	malloc
	addl	$16, %esp
	testl	%eax, %eax
	movl	%eax, (%esi,%ebx,4)
	je	.L54
	incl	%ebx
	cmpl	$10, %ebx
	jl	.L34
	xorl	%ecx, %ecx
.L47:
	xorl	%eax, %eax
	movl	(%esi,%ecx,4), %edx
	.p2align 4,,15
.L46:
	cmpl	%eax, %ecx
	je	.L55
	movl	$0, (%edx,%eax,4)
.L42:
	incl	%eax
	cmpl	$9, %eax
	jle	.L46
	incl	%ecx
	cmpl	$9, %ecx
	jle	.L47
	subl	$12, %esp
	pushl	%esi
	call	printArray
	addl	$20, %esp
	popl	%ebx
	popl	%esi
	ret
	.p2align 4,,15
.L55:
	movl	$1, (%edx,%ecx,4)
	jmp	.L42
.L54:
	subl	$12, %esp
	pushl	$.LC0
	call	puts
	movl	$1, (%esp)
	call	exit
	.size	intArray2D, .-intArray2D
	.ident	"GCC: (GNU) 3.3.2"
