	.file	"array-stack-int3D.c"
	.text
.globl intArray3D
	.type	intArray3D, @function
intArray3D:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$1224, %esp
	movl	$0, -1212(%ebp)
.L2:
	cmpl	$2, -1212(%ebp)
	jle	.L5
	jmp	.L3
.L5:
	movl	$0, -1216(%ebp)
.L6:
	cmpl	$9, -1216(%ebp)
	jle	.L9
	jmp	.L4
.L9:
	movl	$0, -1220(%ebp)
.L10:
	cmpl	$9, -1220(%ebp)
	jle	.L13
	jmp	.L8
.L13:
	movl	-1216(%ebp), %eax
	cmpl	-1220(%ebp), %eax
	jne	.L14
	movl	-1216(%ebp), %ecx
	movl	-1212(%ebp), %edx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	leal	0(,%eax,4), %edx
	addl	%edx, %eax
	leal	0(,%eax,4), %edx
	movl	%ecx, %eax
	sall	$2, %eax
	addl	%ecx, %eax
	addl	%eax, %eax
	leal	(%eax,%edx), %eax
	movl	%eax, %edx
	addl	-1220(%ebp), %edx
	movl	-1212(%ebp), %eax
	movl	%eax, -1208(%ebp,%edx,4)
	jmp	.L12
.L14:
	movl	-1216(%ebp), %ecx
	movl	-1212(%ebp), %edx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	leal	0(,%eax,4), %edx
	addl	%edx, %eax
	leal	0(,%eax,4), %edx
	movl	%ecx, %eax
	sall	$2, %eax
	addl	%ecx, %eax
	addl	%eax, %eax
	leal	(%eax,%edx), %eax
	addl	-1220(%ebp), %eax
	movl	$0, -1208(%ebp,%eax,4)
.L12:
	leal	-1220(%ebp), %eax
	incl	(%eax)
	jmp	.L10
.L8:
	leal	-1216(%ebp), %eax
	incl	(%eax)
	jmp	.L6
.L4:
	leal	-1212(%ebp), %eax
	incl	(%eax)
	jmp	.L2
.L3:
	subl	$12, %esp
	leal	-1208(%ebp), %eax
	pushl	%eax
	call	printArray
	addl	$16, %esp
	leave
	ret
	.size	intArray3D, .-intArray3D
	.ident	"GCC: (GNU) 3.3.2"
