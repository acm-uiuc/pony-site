	.file	"ifcomplicated.c"
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"Enter some values:"
.LC1:
	.string	"%d\n%d\n%d\n"
.LC4:
	.string	"End test 1"
.LC6:
	.string	"Some stuff didn't happen"
.LC5:
	.string	"SOme stuff happened"
.LC3:
	.string	"One pair of a,b,c is equal"
.LC2:
	.string	"a < b < c"
	.text
	.p2align 4,,15
.globl main
	.type	main, @function
main:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$24, %esp
	andl	$-16, %esp
	subl	$12, %esp
	pushl	$.LC0
	call	puts
	leal	-4(%ebp), %ecx
	pushl	%ecx
	leal	-8(%ebp), %edx
	pushl	%edx
	leal	-12(%ebp), %edx
	pushl	%edx
	pushl	$.LC1
	call	scanf
	movl	-12(%ebp), %edx
	movl	-8(%ebp), %eax
	addl	$32, %esp
	cmpl	%eax, %edx
	jge	.L2
	cmpl	-4(%ebp), %eax
	jl	.L12
.L2:
	cmpl	%eax, %edx
	je	.L5
	movl	-4(%ebp), %ecx
	cmpl	%ecx, %eax
	je	.L5
	cmpl	%ecx, %edx
	je	.L5
.L3:
	subl	$12, %esp
	pushl	$.LC4
	call	puts
	movl	-12(%ebp), %eax
	addl	$16, %esp
	cmpl	-8(%ebp), %eax
	je	.L13
.L8:
	movl	12(%ebp), %ecx
	subl	$12, %esp
	movl	4(%ecx), %edx
	pushl	%edx
	call	atoi
	addl	$16, %esp
	testl	%eax, %eax
	js	.L7
	subl	$12, %esp
	pushl	$.LC6
.L11:
	call	puts
	addl	$16, %esp
	leave
	ret
	.p2align 4,,15
.L7:
	subl	$12, %esp
	pushl	$.LC5
	jmp	.L11
	.p2align 4,,15
.L13:
	cmpl	-4(%ebp), %eax
	jne	.L8
	jmp	.L7
	.p2align 4,,15
.L5:
	subl	$12, %esp
	pushl	$.LC3
.L10:
	call	puts
	addl	$16, %esp
	jmp	.L3
	.p2align 4,,15
.L12:
	subl	$12, %esp
	pushl	$.LC2
	jmp	.L10
	.size	main, .-main
	.ident	"GCC: (GNU) 3.3.2"
