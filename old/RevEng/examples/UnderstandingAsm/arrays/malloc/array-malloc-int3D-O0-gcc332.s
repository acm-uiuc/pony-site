	.file	"array-malloc-int3D.c"
	.section	.rodata
.LC0:
	.string	"Not enough space\n"
	.text
.globl allocate3D
	.type	allocate3D, @function
allocate3D:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%esi
	pushl	%ebx
	subl	$16, %esp
	subl	$12, %esp
	movl	8(%ebp), %eax
	sall	$2, %eax
	pushl	%eax
	call	malloc
	addl	$16, %esp
	movl	%eax, -12(%ebp)
	movl	-12(%ebp), %eax
	testl	%eax, %eax
	jne	.L2
	subl	$12, %esp
	pushl	$.LC0
	call	printf
	addl	$16, %esp
	subl	$12, %esp
	pushl	$1
	call	exit
.L2:
	movl	$0, -16(%ebp)
.L3:
	movl	-16(%ebp), %eax
	cmpl	8(%ebp), %eax
	jl	.L6
	jmp	.L4
.L6:
	movl	-16(%ebp), %eax
	leal	0(,%eax,4), %esi
	movl	-12(%ebp), %ebx
	subl	$12, %esp
	movl	12(%ebp), %eax
	sall	$2, %eax
	pushl	%eax
	call	malloc
	addl	$16, %esp
	movl	%eax, (%ebx,%esi)
	movl	(%ebx,%esi), %eax
	testl	%eax, %eax
	jne	.L7
	subl	$12, %esp
	pushl	$.LC0
	call	printf
	addl	$16, %esp
	subl	$12, %esp
	pushl	$1
	call	exit
.L7:
	movl	$0, -20(%ebp)
.L8:
	movl	-20(%ebp), %eax
	cmpl	12(%ebp), %eax
	jl	.L11
	jmp	.L5
.L11:
	movl	-16(%ebp), %eax
	leal	0(,%eax,4), %ecx
	movl	-12(%ebp), %edx
	movl	-20(%ebp), %eax
	leal	0(,%eax,4), %esi
	movl	(%edx,%ecx), %ebx
	subl	$12, %esp
	movl	16(%ebp), %eax
	sall	$2, %eax
	pushl	%eax
	call	malloc
	addl	$16, %esp
	movl	%eax, (%ebx,%esi)
	movl	(%ebx,%esi), %eax
	testl	%eax, %eax
	jne	.L10
	subl	$12, %esp
	pushl	$.LC0
	call	printf
	addl	$16, %esp
	subl	$12, %esp
	pushl	$1
	call	exit
.L10:
	leal	-20(%ebp), %eax
	incl	(%eax)
	jmp	.L8
.L5:
	leal	-16(%ebp), %eax
	incl	(%eax)
	jmp	.L3
.L4:
	movl	-12(%ebp), %eax
	leal	-8(%ebp), %esp
	popl	%ebx
	popl	%esi
	popl	%ebp
	ret
	.size	allocate3D, .-allocate3D
.globl intArray3D
	.type	intArray3D, @function
intArray3D:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	subl	$20, %esp
	subl	$4, %esp
	pushl	$10
	pushl	$10
	pushl	$3
	call	allocate3D
	addl	$16, %esp
	movl	%eax, -8(%ebp)
	movl	$0, -12(%ebp)
.L14:
	cmpl	$2, -12(%ebp)
	jle	.L17
	jmp	.L15
.L17:
	movl	$0, -16(%ebp)
.L18:
	cmpl	$9, -16(%ebp)
	jle	.L21
	jmp	.L16
.L21:
	movl	$0, -20(%ebp)
.L22:
	cmpl	$9, -20(%ebp)
	jle	.L25
	jmp	.L20
.L25:
	movl	-16(%ebp), %eax
	cmpl	-20(%ebp), %eax
	jne	.L26
	movl	-12(%ebp), %eax
	leal	0(,%eax,4), %ecx
	movl	-8(%ebp), %edx
	movl	-16(%ebp), %eax
	leal	0(,%eax,4), %ebx
	movl	(%edx,%ecx), %edx
	movl	-20(%ebp), %eax
	leal	0(,%eax,4), %ecx
	movl	(%edx,%ebx), %edx
	movl	-12(%ebp), %eax
	movl	%eax, (%edx,%ecx)
	jmp	.L24
.L26:
	movl	-12(%ebp), %eax
	leal	0(,%eax,4), %ecx
	movl	-8(%ebp), %edx
	movl	-16(%ebp), %eax
	leal	0(,%eax,4), %ebx
	movl	(%edx,%ecx), %ecx
	movl	-20(%ebp), %eax
	leal	0(,%eax,4), %edx
	movl	(%ecx,%ebx), %eax
	movl	$0, (%eax,%edx)
.L24:
	leal	-20(%ebp), %eax
	incl	(%eax)
	jmp	.L22
.L20:
	leal	-16(%ebp), %eax
	incl	(%eax)
	jmp	.L18
.L16:
	leal	-12(%ebp), %eax
	incl	(%eax)
	jmp	.L14
.L15:
	subl	$12, %esp
	pushl	-8(%ebp)
	call	printArray
	addl	$16, %esp
	movl	-4(%ebp), %ebx
	leave
	ret
	.size	intArray3D, .-intArray3D
	.ident	"GCC: (GNU) 3.3.2"
