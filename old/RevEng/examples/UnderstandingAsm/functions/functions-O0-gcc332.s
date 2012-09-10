	.file	"functions.c"
	.section	.rodata
.LC0:
	.string	"%d, %d, %d\n"
	.text
.globl function3args
	.type	function3args, @function
function3args:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$8, %esp
	pushl	16(%ebp)
	pushl	12(%ebp)
	pushl	8(%ebp)
	pushl	$.LC0
	call	printf
	addl	$16, %esp
	leave
	ret
	.size	function3args, .-function3args
.globl function3argsRet
	.type	function3argsRet, @function
function3argsRet:
	pushl	%ebp
	movl	%esp, %ebp
	movl	8(%ebp), %eax
	imull	12(%ebp), %eax
	imull	16(%ebp), %eax
	popl	%ebp
	ret
	.size	function3argsRet, .-function3argsRet
.globl functionPtrArg
	.type	functionPtrArg, @function
functionPtrArg:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$8, %esp
	movl	16(%ebp), %eax
	pushl	(%eax)
	pushl	12(%ebp)
	pushl	8(%ebp)
	pushl	$.LC0
	call	printf
	addl	$16, %esp
	leave
	ret
	.size	functionPtrArg, .-functionPtrArg
.globl functionPtrRet
	.type	functionPtrRet, @function
functionPtrRet:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$4, %esp
	movl	12(%ebp), %eax
	movl	8(%ebp), %edx
	addl	%eax, %edx
	movl	%edx, -4(%ebp)
	movl	-4(%ebp), %eax
	sall	$2, %eax
	movl	16(%ebp), %edx
	addl	%eax, %edx
	movl	%edx, -4(%ebp)
	movl	-4(%ebp), %eax
	leave
	ret
	.size	functionPtrRet, .-functionPtrRet
.globl functionLocalVars
	.type	functionLocalVars, @function
functionLocalVars:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$16, %esp
	movl	12(%ebp), %eax
	xorl	8(%ebp), %eax
	movl	%eax, -4(%ebp)
	movl	8(%ebp), %edx
	leal	12(%ebp), %eax
	movl	%eax, -16(%ebp)
	movl	%edx, %eax
	movl	-16(%ebp), %ecx
	cltd
	idivl	(%ecx)
	movl	%eax, -16(%ebp)
	movl	-16(%ebp), %eax
	movl	%eax, -8(%ebp)
	movl	8(%ebp), %edx
	leal	12(%ebp), %ecx
	movl	%ecx, -16(%ebp)
	movl	%edx, %eax
	movl	-16(%ebp), %ecx
	cltd
	idivl	(%ecx)
	movl	%edx, -12(%ebp)
	movl	-8(%ebp), %eax
	orl	-4(%ebp), %eax
	movl	%eax, -12(%ebp)
	movl	-12(%ebp), %eax
	andl	8(%ebp), %eax
	leave
	ret
	.size	functionLocalVars, .-functionLocalVars
.globl main
	.type	main, @function
main:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$8, %esp
	andl	$-16, %esp
	movl	$0, %eax
	subl	%eax, %esp
#APP
	nop
#NO_APP
	subl	$4, %esp
	pushl	$3
	pushl	$2
	pushl	$1
	call	function3args
	addl	$16, %esp
#APP
	nop
#NO_APP
	subl	$4, %esp
	pushl	$3
	pushl	$2
	pushl	$1
	call	function3argsRet
	addl	$16, %esp
	movl	%eax, -4(%ebp)
#APP
	nop
#NO_APP
	subl	$4, %esp
	leal	-4(%ebp), %eax
	pushl	%eax
	pushl	$3
	pushl	$1
	call	functionPtrArg
	addl	$16, %esp
#APP
	nop
#NO_APP
	subl	$4, %esp
	leal	-4(%ebp), %eax
	pushl	%eax
	pushl	$3
	pushl	$1
	call	functionPtrRet
	addl	$16, %esp
	movl	%eax, -8(%ebp)
#APP
	nop
#NO_APP
	subl	$8, %esp
	pushl	$2
	pushl	$1
	call	functionLocalVars
	addl	$16, %esp
	movl	%eax, -4(%ebp)
#APP
	nop
#NO_APP
	leave
	ret
	.size	main, .-main
	.ident	"GCC: (GNU) 3.3.2"
