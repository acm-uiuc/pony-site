	.file	"array-malloc-int2D.c"
	.section	.rodata
.LC0:
	.string	"Not enough space\n"
	.text
.globl allocate2D
	.type	allocate2D, @function
allocate2D:
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
	jne	.L5
	subl	$12, %esp
	pushl	$.LC0
	call	printf
	addl	$16, %esp
	subl	$12, %esp
	pushl	$1
	call	exit
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
	.size	allocate2D, .-allocate2D
.globl intArray2D
	.type	intArray2D, @function
intArray2D:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	subl	$20, %esp
	subl	$8, %esp
	pushl	$10
	pushl	$10
	call	allocate2D
	addl	$16, %esp
	movl	%eax, -8(%ebp)
	movl	$0, -12(%ebp)
.L9:
	cmpl	$9, -12(%ebp)
	jle	.L12
	jmp	.L10
.L12:
	movl	$0, -16(%ebp)
.L13:
	cmpl	$9, -16(%ebp)
	jle	.L16
	jmp	.L11
.L16:
	movl	-12(%ebp), %eax
	cmpl	-16(%ebp), %eax
	jne	.L17
	movl	-12(%ebp), %eax
	leal	0(,%eax,4), %ebx
	movl	-8(%ebp), %ecx
	movl	-16(%ebp), %eax
	leal	0(,%eax,4), %edx
	movl	(%ecx,%ebx), %eax
	movl	$1, (%eax,%edx)
	jmp	.L15
.L17:
	movl	-12(%ebp), %eax
	leal	0(,%eax,4), %ebx
	movl	-8(%ebp), %ecx
	movl	-16(%ebp), %eax
	leal	0(,%eax,4), %edx
	movl	(%ecx,%ebx), %eax
	movl	$0, (%eax,%edx)
.L15:
	leal	-16(%ebp), %eax
	incl	(%eax)
	jmp	.L13
.L11:
	leal	-12(%ebp), %eax
	incl	(%eax)
	jmp	.L9
.L10:
	subl	$12, %esp
	pushl	-8(%ebp)
	call	printArray
	addl	$16, %esp
	movl	-4(%ebp), %ebx
	leave
	ret
	.size	intArray2D, .-intArray2D
	.ident	"GCC: (GNU) 3.3.2"
