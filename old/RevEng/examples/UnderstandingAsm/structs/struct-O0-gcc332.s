	.file	"struct.c"
	.text
.globl retstruct
	.type	retstruct, @function
retstruct:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$8, %esp
	movl	8(%ebp), %ecx
	movl	$1, -8(%ebp)
	movb	$2, -4(%ebp)
	movw	$3, -2(%ebp)
	movl	-8(%ebp), %eax
	movl	-4(%ebp), %edx
	movl	%eax, (%ecx)
	movl	%edx, 4(%ecx)
	movl	%ecx, %eax
	leave
	ret	$4
	.size	retstruct, .-retstruct
	.section	.rodata
.LC0:
	.string	"%d, %d, %d\n"
	.text
.globl passtruct
	.type	passtruct, @function
passtruct:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$8, %esp
	movl	8(%ebp), %eax
	movl	12(%ebp), %edx
	movl	%eax, -8(%ebp)
	movl	%edx, -4(%ebp)
	movl	$1, -8(%ebp)
	movb	$2, -4(%ebp)
	movw	$3, -2(%ebp)
	movswl	-2(%ebp),%eax
	pushl	%eax
	movsbl	-4(%ebp),%eax
	pushl	%eax
	pushl	-8(%ebp)
	pushl	$.LC0
	call	printf
	addl	$16, %esp
	leave
	ret
	.size	passtruct, .-passtruct
	.section	.rodata
.LC1:
	.string	"%d %d %d\n"
	.text
.globl main
	.type	main, @function
main:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$8, %esp
	andl	$-16, %esp
	movl	$0, %eax
	subl	%eax, %esp
	leal	-8(%ebp), %eax
	subl	$12, %esp
	pushl	%eax
	call	retstruct
	addl	$12, %esp
	subl	$8, %esp
	pushl	-4(%ebp)
	pushl	-8(%ebp)
	call	passstruct
	addl	$16, %esp
	movswl	-2(%ebp),%eax
	pushl	%eax
	movsbl	-4(%ebp),%eax
	pushl	%eax
	pushl	-8(%ebp)
	pushl	$.LC1
	call	printf
	addl	$16, %esp
	leave
	ret
	.size	main, .-main
	.ident	"GCC: (GNU) 3.3.2"
