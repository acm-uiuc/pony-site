	.file	"array-stack-int2D.c"
	.text
.globl intArray2D
	.type	intArray2D, @function
intArray2D:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$424, %esp
	movl	$0, -412(%ebp)
.L2:
	cmpl	$9, -412(%ebp)
	jle	.L5
	jmp	.L3
.L5:
	movl	$0, -416(%ebp)
.L6:
	cmpl	$9, -416(%ebp)
	jle	.L9
	jmp	.L4
.L9:
	movl	-412(%ebp), %eax
	cmpl	-416(%ebp), %eax
	jne	.L10
	movl	-412(%ebp), %edx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	addl	%eax, %eax
	addl	-416(%ebp), %eax
	movl	$1, -408(%ebp,%eax,4)
	jmp	.L8
.L10:
	movl	-412(%ebp), %edx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	addl	%eax, %eax
	addl	-416(%ebp), %eax
	movl	$0, -408(%ebp,%eax,4)
.L8:
	leal	-416(%ebp), %eax
	incl	(%eax)
	jmp	.L6
.L4:
	leal	-412(%ebp), %eax
	incl	(%eax)
	jmp	.L2
.L3:
	subl	$12, %esp
	leal	-408(%ebp), %eax
	pushl	%eax
	call	printArray
	addl	$16, %esp
	leave
	ret
	.size	intArray2D, .-intArray2D
	.ident	"GCC: (GNU) 3.3.2"
