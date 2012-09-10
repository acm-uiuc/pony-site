	.file	"while.c"
	.version	"01.01"
gcc2_compiled.:
.section	.rodata
.LC0:
	.string	"%d\n"
.text
	.align 4
.globl main
	.type	 main,@function
main:
	pushl %ebp
	movl %esp,%ebp
	subl $24,%esp
	movl $0,-4(%ebp)
	.p2align 4,,7
.L3:
	cmpl $9,-4(%ebp)
	jle .L5
	jmp .L4
	.p2align 4,,7
.L5:
	addl $-8,%esp
	movl -4(%ebp),%eax
	pushl %eax
	pushl $.LC0
	call printf
	addl $16,%esp
	incl -4(%ebp)
	jmp .L3
	.p2align 4,,7
.L4:
.L2:
	leave
	ret
.Lfe1:
	.size	 main,.Lfe1-main
	.ident	"GCC: (GNU) 2.95.4  (Debian prerelease)"
