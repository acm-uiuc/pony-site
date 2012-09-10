	.file	"struct.c"
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"%d %d %d\n"
	.text
	.p2align 4,,15
.globl main
	.type	main, @function
main:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%edi
	pushl	%esi
	pushl	%ebx
	subl	$12, %esp
	andl	$-16, %esp
	subl	$8, %esp
	movl	$196610, %edi
	pushl	%edi
	movl	$1, %ebx
	pushl	%ebx
	movl	%edi, %esi
	call	passstruct
	shrl	$16, %edi
	movl	%esi, %eax
	pushl	%edi
	movsbl	%al,%ecx
	pushl	%ecx
	pushl	%ebx
	pushl	$.LC0
	call	printf
	leal	-12(%ebp), %esp
	popl	%ebx
	popl	%esi
	popl	%edi
	popl	%ebp
	ret
	.size	main, .-main
	.p2align 4,,15
.globl retstruct
	.type	retstruct, @function
retstruct:
	movl	4(%esp), %eax
	movl	$1, (%eax)
	movl	$196610, 4(%eax)
	ret	$4
	.size	retstruct, .-retstruct
	.section	.rodata.str1.1
.LC1:
	.string	"%d, %d, %d\n"
	.text
	.p2align 4,,15
.globl passtruct
	.type	passtruct, @function
passtruct:
	subl	$12, %esp
	movl	20(%esp), %edx
	movb	$2, %dl
	movzwl	%dx,%ecx
	orl	$196608, %ecx
	movl	%ecx, %edx
	shrl	$16, %ecx
	pushl	%ecx
	movsbl	%dl,%ecx
	pushl	%ecx
	movl	$1, %eax
	pushl	%eax
	pushl	$.LC1
	call	printf
	addl	$28, %esp
	ret
	.size	passtruct, .-passtruct
	.ident	"GCC: (GNU) 3.3.2"
