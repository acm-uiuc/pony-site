	.file	"functions.c"
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"%d, %d, %d\n"
	.text
	.p2align 4,,15
.globl main
	.type	main, @function
main:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$8, %esp
	andl	$-16, %esp
#APP
	nop
#NO_APP
	pushl	$3
	pushl	$2
	pushl	$1
	pushl	$.LC0
	call	printf
	addl	$16, %esp
#APP
	nop
	nop
#NO_APP
	pushl	$6
	pushl	$3
	pushl	$1
	pushl	$.LC0
	call	printf
	addl	$16, %esp
#APP
	nop
	nop
	nop
#NO_APP
	movl	%ebp, %esp
	popl	%ebp
	ret
	.size	main, .-main
	.p2align 4,,15
.globl function3args
	.type	function3args, @function
function3args:
	subl	$12, %esp
	movl	24(%esp), %edx
	pushl	%edx
	movl	24(%esp), %eax
	pushl	%eax
	movl	24(%esp), %edx
	pushl	%edx
	pushl	$.LC0
	call	printf
	addl	$28, %esp
	ret
	.size	function3args, .-function3args
	.p2align 4,,15
.globl function3argsRet
	.type	function3argsRet, @function
function3argsRet:
	movl	8(%esp), %eax
	imull	4(%esp), %eax
	imull	12(%esp), %eax
	ret
	.size	function3argsRet, .-function3argsRet
	.p2align 4,,15
.globl functionPtrArg
	.type	functionPtrArg, @function
functionPtrArg:
	subl	$12, %esp
	movl	24(%esp), %edx
	movl	(%edx), %ecx
	pushl	%ecx
	movl	24(%esp), %edx
	pushl	%edx
	movl	24(%esp), %edx
	pushl	%edx
	pushl	$.LC0
	call	printf
	addl	$28, %esp
	ret
	.size	functionPtrArg, .-functionPtrArg
	.p2align 4,,15
.globl functionPtrRet
	.type	functionPtrRet, @function
functionPtrRet:
	movl	8(%esp), %eax
	addl	4(%esp), %eax
	sall	$2, %eax
	addl	12(%esp), %eax
	ret
	.size	functionPtrRet, .-functionPtrRet
	.p2align 4,,15
.globl functionLocalVars
	.type	functionLocalVars, @function
functionLocalVars:
	pushl	%edi
	pushl	%esi
	pushl	%ebx
	movl	16(%esp), %esi
	movl	%esi, %eax
	movl	20(%esp), %ebx
	movl	%esi, %edi
	cltd
	idivl	%ebx
	xorl	%ebx, %edi
	orl	%eax, %edi
	andl	%edi, %esi
	popl	%ebx
	movl	%esi, %eax
	popl	%esi
	popl	%edi
	ret
	.size	functionLocalVars, .-functionLocalVars
	.ident	"GCC: (GNU) 3.3.2"
