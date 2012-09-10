	.file	"array-malloc-int1D.c"
	.section	.rodata
.LC0:
	.string	"Out of memory\n"
	.text
.globl allocate1D
	.type	allocate1D, @function
allocate1D:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$8, %esp
	subl	$12, %esp
	movl	8(%ebp), %eax
	sall	$2, %eax
	pushl	%eax
	call	malloc
	addl	$16, %esp
	movl	%eax, -4(%ebp)
	movl	-4(%ebp), %eax
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
	movl	-4(%ebp), %eax
	leave
	ret
	.size	allocate1D, .-allocate1D
.globl intArray
	.type	intArray, @function
intArray:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$8, %esp
	subl	$12, %esp
	pushl	$512
	call	allocate1D
	addl	$16, %esp
	movl	%eax, -4(%ebp)
	movl	$0, -8(%ebp)
.L4:
	cmpl	$511, -8(%ebp)
	jle	.L7
	jmp	.L5
.L7:
	movl	-8(%ebp), %eax
	leal	0(,%eax,4), %ecx
	movl	-4(%ebp), %edx
	movl	-8(%ebp), %eax
	movl	%eax, (%edx,%ecx)
	leal	-8(%ebp), %eax
	incl	(%eax)
	jmp	.L4
.L5:
	subl	$12, %esp
	pushl	-4(%ebp)
	call	printArray
	addl	$16, %esp
	leave
	ret
	.size	intArray, .-intArray
	.ident	"GCC: (GNU) 3.3.2"
