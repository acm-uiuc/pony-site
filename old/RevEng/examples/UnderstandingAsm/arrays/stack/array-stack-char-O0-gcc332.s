	.file	"array-stack-char.c"
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
	.ident	"GCC: (GNU) 3.3.2"
