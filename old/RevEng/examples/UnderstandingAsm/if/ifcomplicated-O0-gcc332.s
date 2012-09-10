	.file	"ifcomplicated.c"
	.section	.rodata
.LC0:
	.string	"Enter some values:\n"
.LC1:
	.string	"%d\n%d\n%d\n"
.LC2:
	.string	"a < b < c\n"
.LC3:
	.string	"One pair of a,b,c is equal\n"
.LC4:
	.string	"End test 1\n"
.LC5:
	.string	"SOme stuff happened\n"
.LC6:
	.string	"Some stuff didn't happen\n"
	.text
.globl main
	.type	main, @function
main:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$24, %esp
	andl	$-16, %esp
	movl	$0, %eax
	subl	%eax, %esp
	subl	$12, %esp
	pushl	$.LC0
	call	printf
	addl	$16, %esp
	leal	-12(%ebp), %eax
	pushl	%eax
	leal	-8(%ebp), %eax
	pushl	%eax
	leal	-4(%ebp), %eax
	pushl	%eax
	pushl	$.LC1
	call	scanf
	addl	$16, %esp
	movl	-4(%ebp), %eax
	cmpl	-8(%ebp), %eax
	jge	.L2
	movl	-8(%ebp), %eax
	cmpl	-12(%ebp), %eax
	jge	.L2
	subl	$12, %esp
	pushl	$.LC2
	call	printf
	addl	$16, %esp
	jmp	.L3
.L2:
	movl	-4(%ebp), %eax
	cmpl	-8(%ebp), %eax
	je	.L5
	movl	-8(%ebp), %eax
	cmpl	-12(%ebp), %eax
	je	.L5
	movl	-4(%ebp), %eax
	cmpl	-12(%ebp), %eax
	je	.L5
	jmp	.L3
.L5:
	subl	$12, %esp
	pushl	$.LC3
	call	printf
	addl	$16, %esp
.L3:
	subl	$12, %esp
	pushl	$.LC4
	call	printf
	addl	$16, %esp
	movl	-4(%ebp), %eax
	cmpl	-8(%ebp), %eax
	jne	.L8
	movl	-8(%ebp), %eax
	cmpl	-12(%ebp), %eax
	je	.L7
.L8:
	subl	$12, %esp
	movl	12(%ebp), %eax
	addl	$4, %eax
	pushl	(%eax)
	call	atoi
	addl	$16, %esp
	testl	%eax, %eax
	js	.L7
	jmp	.L6
.L7:
	subl	$12, %esp
	pushl	$.LC5
	call	printf
	addl	$16, %esp
	jmp	.L9
.L6:
	subl	$12, %esp
	pushl	$.LC6
	call	printf
	addl	$16, %esp
.L9:
	leave
	ret
	.size	main, .-main
	.ident	"GCC: (GNU) 3.3.2"
