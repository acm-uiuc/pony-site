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
	subl $12,%esp
	pushl 24(%esp)
	pushl 24(%esp)
	pushl 24(%esp)
	pushl $.LC0
	call printf
	addl $16,%esp
	addl $12,%esp
	ret
.Lfe1:
	.size	 function3args,.Lfe1-function3args
	.align 4
.globl function3argsRet
	.type	 function3argsRet,@function
function3argsRet:
	movl 4(%esp),%eax
	imull 8(%esp),%eax
	imull 12(%esp),%eax
	ret
.Lfe2:
	.size	 function3argsRet,.Lfe2-function3argsRet
	.align 4
.globl functionPtrArg
	.type	 functionPtrArg,@function
functionPtrArg:
	subl $12,%esp
	movl 24(%esp),%eax
	pushl (%eax)
	pushl 24(%esp)
	pushl 24(%esp)
	pushl $.LC0
	call printf
	addl $16,%esp
	addl $12,%esp
	ret
.Lfe3:
	.size	 functionPtrArg,.Lfe3-functionPtrArg
	.align 4
.globl functionPtrRet
	.type	 functionPtrRet,@function
functionPtrRet:
	movl 4(%esp),%eax
	sall $2,%eax
	addl 12(%esp),%eax
	movl 8(%esp),%edx
	sall $2,%edx
	addl %edx,%eax
	ret
.Lfe4:
	.size	 functionPtrRet,.Lfe4-functionPtrRet
	.align 4
.globl main
	.type	 main,@function
main:
	subl $12,%esp
#APP
	nop
#NO_APP
	pushl $3
	pushl $2
	pushl $1
	pushl $.LC0
	call printf
	addl $16,%esp
#APP
	nop
	nop
#NO_APP
	pushl $6
	pushl $3
	pushl $1
	pushl $.LC0
	call printf
	addl $16,%esp
#APP
	nop
	nop
#NO_APP
	addl $12,%esp
	ret
.Lfe5:
	.size	 main,.Lfe5-main
	.ident	"GCC: (GNU) 2.95.4  (Debian prerelease)"
