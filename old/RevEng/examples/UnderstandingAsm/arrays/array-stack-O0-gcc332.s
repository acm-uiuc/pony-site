	.file	"array-stack.c"
	.section	.rodata
.LC0:
	.string	"hello there, govna!"
	.text
.globl charArray
	.type	charArray, @function
charArray:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$520, %esp
	subl	$4, %esp
	pushl	$511
	pushl	$.LC0
	leal	-520(%ebp), %eax
	pushl	%eax
	call	strncpy
	addl	$16, %esp
	movb	$0, -501(%ebp)
	subl	$12, %esp
	leal	-520(%ebp), %eax
	pushl	%eax
	call	printArray
	addl	$16, %esp
	leave
	ret
	.size	charArray, .-charArray
.globl intArray
	.type	intArray, @function
intArray:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$2072, %esp
	movl	$0, -2060(%ebp)
.L3:
	cmpl	$511, -2060(%ebp)
	jle	.L6
	jmp	.L4
.L6:
	movl	-2060(%ebp), %edx
	movl	-2060(%ebp), %eax
	movl	%eax, -2056(%ebp,%edx,4)
	leal	-2060(%ebp), %eax
	incl	(%eax)
	jmp	.L3
.L4:
	subl	$12, %esp
	leal	-2056(%ebp), %eax
	pushl	%eax
	call	printArray
	addl	$16, %esp
	leave
	ret
	.size	intArray, .-intArray
.globl intArray2D
	.type	intArray2D, @function
intArray2D:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$424, %esp
	movl	$0, -412(%ebp)
.L8:
	cmpl	$9, -412(%ebp)
	jle	.L11
	jmp	.L9
.L11:
	movl	$0, -416(%ebp)
.L12:
	cmpl	$9, -416(%ebp)
	jle	.L15
	jmp	.L10
.L15:
	movl	-412(%ebp), %eax
	cmpl	-416(%ebp), %eax
	jne	.L16
	movl	-412(%ebp), %edx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	addl	%eax, %eax
	addl	-416(%ebp), %eax
	movl	$1, -408(%ebp,%eax,4)
	jmp	.L14
.L16:
	movl	-412(%ebp), %edx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	addl	%eax, %eax
	addl	-416(%ebp), %eax
	movl	$0, -408(%ebp,%eax,4)
.L14:
	leal	-416(%ebp), %eax
	incl	(%eax)
	jmp	.L12
.L10:
	leal	-412(%ebp), %eax
	incl	(%eax)
	jmp	.L8
.L9:
	subl	$12, %esp
	leal	-408(%ebp), %eax
	pushl	%eax
	call	printArray
	addl	$16, %esp
	leave
	ret
	.size	intArray2D, .-intArray2D
.globl intArray3D
	.type	intArray3D, @function
intArray3D:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$1224, %esp
	movl	$0, -1212(%ebp)
.L19:
	cmpl	$2, -1212(%ebp)
	jle	.L22
	jmp	.L20
.L22:
	movl	$0, -1216(%ebp)
.L23:
	cmpl	$9, -1216(%ebp)
	jle	.L26
	jmp	.L21
.L26:
	movl	$0, -1220(%ebp)
.L27:
	cmpl	$9, -1220(%ebp)
	jle	.L30
	jmp	.L25
.L30:
	movl	-1216(%ebp), %eax
	cmpl	-1220(%ebp), %eax
	jne	.L31
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
	jmp	.L29
.L31:
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
.L29:
	leal	-1220(%ebp), %eax
	incl	(%eax)
	jmp	.L27
.L25:
	leal	-1216(%ebp), %eax
	incl	(%eax)
	jmp	.L23
.L21:
	leal	-1212(%ebp), %eax
	incl	(%eax)
	jmp	.L19
.L20:
	subl	$12, %esp
	leal	-1208(%ebp), %eax
	pushl	%eax
	call	printArray
	addl	$16, %esp
	leave
	ret
	.size	intArray3D, .-intArray3D
.globl main
	.type	main, @function
main:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$8, %esp
	andl	$-16, %esp
	movl	$0, %eax
	subl	%eax, %esp
	leave
	ret
	.size	main, .-main
	.ident	"GCC: (GNU) 3.3.2"
