	.file	"array-stack-int1D.c"
	.text
.globl intArray
	.type	intArray, @function
intArray:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$2072, %esp
	movl	$0, -2060(%ebp)
.L2:
	cmpl	$511, -2060(%ebp)
	jle	.L5
	jmp	.L3
.L5:
	movl	-2060(%ebp), %edx
	movl	-2060(%ebp), %eax
	movl	%eax, -2056(%ebp,%edx,4)
	leal	-2060(%ebp), %eax
	incl	(%eax)
	jmp	.L2
.L3:
	subl	$12, %esp
	leal	-2056(%ebp), %eax
	pushl	%eax
	call	printArray
	addl	$16, %esp
	leave
	ret
	.size	intArray, .-intArray
	.ident	"GCC: (GNU) 3.3.2"
