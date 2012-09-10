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
	movl	12(%ebp), %eax
	imull	8(%ebp), %eax
	imull	16(%ebp), %eax
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
	sall	$2, %eax
	addl	16(%ebp), %eax
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
	pushl	%ebx
	pushl	%eax
	movl	8(%ebp), %ebx
	movl	%ebx, %eax
	movl	12(%ebp), %ecx
	cltd
	movl	%ebx, -8(%ebp)
	idivl	%ecx
	xorl	%ecx, -8(%ebp)
	movl	%eax, %ecx
	orl	%ecx, -8(%ebp)
	andl	-8(%ebp), %ebx
	movl	%ebx, %eax
	movl	-4(%ebp), %ebx
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
	pushl	%ebx
	subl	$8, %esp
#APP
	nop
#NO_APP
	pushl	$3
	pushl	$2
	pushl	$1
	call	function3args
#APP
	nop
#NO_APP
	addl	$12, %esp
	pushl	$3
	pushl	$2
	pushl	$1
	call	function3argsRet
	movl	%eax, -8(%ebp)
#APP
	nop
#NO_APP
	addl	$12, %esp
	leal	-8(%ebp), %ebx
	pushl	%ebx
	pushl	$3
	pushl	$1
	call	functionPtrArg
#APP
	nop
#NO_APP
	addl	$12, %esp
	pushl	%ebx
	pushl	$3
	pushl	$1
	call	functionPtrRet
#APP
	nop
#NO_APP
	popl	%edx
	popl	%ecx
	pushl	$2
	pushl	$1
	call	functionLocalVars
	movl	%eax, -8(%ebp)
#APP
	nop
#NO_APP
	movl	-4(%ebp), %ebx
	leave
	ret
.Lfe6:
	.size	 main,.Lfe6-main
	.ident	"GCC: (GNU) 2.96 20000731 (Red Hat Linux 7.1 2.96-81)"
