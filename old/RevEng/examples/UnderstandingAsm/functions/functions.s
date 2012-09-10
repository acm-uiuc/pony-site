	.file	"functions.c"
	.version	"01.01"
gcc2_compiled.:
		.section	.rodata
.LC0:
	.string	"%d, %d, %d\n"
.text
	.align 4
.globl function3args
	.type	 function3args,@function
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
.Lfe1:
	.size	 function3args,.Lfe1-function3args
	.align 4
.globl function3argsRet
	.type	 function3argsRet,@function
function3argsRet:
	pushl	%ebp
	movl	%esp, %ebp
	movl	8(%ebp), %eax
	imull	12(%ebp), %eax
	imull	16(%ebp), %eax
	movl	%eax, %eax
	popl	%ebp
	ret
.Lfe2:
	.size	 function3argsRet,.Lfe2-function3argsRet
	.align 4
.globl functionPtrArg
	.type	 functionPtrArg,@function
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
.Lfe3:
	.size	 functionPtrArg,.Lfe3-functionPtrArg
	.align 4
.globl functionPtrRet
	.type	 functionPtrRet,@function
functionPtrRet:
	pushl	%ebp
	movl	%esp, %ebp
	movl	12(%ebp), %eax
	addl	8(%ebp), %eax
	imull	$4, %eax, %eax
	addl	16(%ebp), %eax
	movl	%eax, %eax
	popl	%ebp
	ret
.Lfe4:
	.size	 functionPtrRet,.Lfe4-functionPtrRet
	.align 4
.globl functionLocalVars
	.type	 functionLocalVars,@function
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
	movl	%eax, -8(%ebp)
	movl	8(%ebp), %edx
	leal	12(%ebp), %eax
	movl	%eax, -16(%ebp)
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
	movl	%eax, %eax
	leave
	ret
.Lfe5:
	.size	 functionLocalVars,.Lfe5-functionLocalVars
	.align 4
.globl main
	.type	 main,@function
main:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$8, %esp
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
	movl	%eax, %eax
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
	movl	%eax, %eax
	movl	%eax, -8(%ebp)
#APP
	nop
#NO_APP
	subl	$8, %esp
	pushl	$2
	pushl	$1
	call	functionLocalVars
	addl	$16, %esp
	movl	%eax, %eax
	movl	%eax, -4(%ebp)
#APP
	nop
#NO_APP
	leave
	ret
.Lfe6:
	.size	 main,.Lfe6-main
	.ident	"GCC: (GNU) 2.96 20000731 (Red Hat Linux 7.1 2.96-81)"
