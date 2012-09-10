	.file	"array-malloc-int3D.c"
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"Not enough space"
	.text
	.p2align 4,,15
.globl allocate3D
	.type	allocate3D, @function
allocate3D:
	pushl	%ebp
	pushl	%edi
	pushl	%esi
	pushl	%ebx
	subl	$24, %esp
	movl	44(%esp), %edx
	sall	$2, %edx
	pushl	%edx
	call	malloc
	addl	$16, %esp
	testl	%eax, %eax
	movl	%eax, %edi
	je	.L19
	xorl	%ebp, %ebp
	cmpl	32(%esp), %ebp
	jl	.L14
.L16:
	addl	$12, %esp
	popl	%ebx
	popl	%esi
	movl	%edi, %eax
	popl	%edi
	popl	%ebp
	ret
	.p2align 4,,15
.L14:
	subl	$12, %esp
	movl	48(%esp), %edx
	sall	$2, %edx
	pushl	%edx
	call	malloc
	addl	$16, %esp
	testl	%eax, %eax
	movl	%eax, (%edi,%ebp,4)
	je	.L19
	xorl	%esi, %esi
	cmpl	36(%esp), %esi
	jl	.L13
.L18:
	incl	%ebp
	cmpl	32(%esp), %ebp
	jl	.L14
	jmp	.L16
	.p2align 4,,15
.L13:
	subl	$12, %esp
	movl	52(%esp), %ecx
	sall	$2, %ecx
	pushl	%ecx
	movl	(%edi,%ebp,4), %ebx
	call	malloc
	addl	$16, %esp
	testl	%eax, %eax
	movl	%eax, (%ebx,%esi,4)
	je	.L19
	incl	%esi
	cmpl	36(%esp), %esi
	jl	.L13
	jmp	.L18
.L19:
	subl	$12, %esp
	pushl	$.LC0
	call	puts
	movl	$1, (%esp)
	call	exit
	.size	allocate3D, .-allocate3D
	.p2align 4,,15
.globl intArray3D
	.type	intArray3D, @function
intArray3D:
	pushl	%edi
	pushl	%esi
	pushl	%ebx
	pushl	%eax
	pushl	$10
	pushl	$10
	pushl	$3
	call	allocate3D
	movl	%eax, %edi
	xorl	%esi, %esi
	addl	$16, %esp
.L52:
	xorl	%ecx, %ecx
	movl	(%edi,%esi,4), %ebx
.L51:
	xorl	%edx, %edx
	.p2align 4,,15
.L50:
	cmpl	%edx, %ecx
	je	.L59
	movl	(%ebx,%ecx,4), %eax
	movl	$0, (%eax,%edx,4)
.L46:
	incl	%edx
	cmpl	$9, %edx
	jle	.L50
	incl	%ecx
	cmpl	$9, %ecx
	jle	.L51
	incl	%esi
	cmpl	$2, %esi
	jle	.L52
	subl	$12, %esp
	pushl	%edi
	call	printArray
	addl	$16, %esp
	popl	%ebx
	popl	%esi
	popl	%edi
	ret
	.p2align 4,,15
.L59:
	movl	(%ebx,%ecx,4), %eax
	movl	%esi, (%eax,%ecx,4)
	jmp	.L46
	.size	intArray3D, .-intArray3D
	.ident	"GCC: (GNU) 3.3.2"
