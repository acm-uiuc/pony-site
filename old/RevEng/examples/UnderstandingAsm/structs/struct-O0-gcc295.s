	.file	"struct.c"
	.version	"01.01"
gcc2_compiled.:
.text
	.align 16
.globl retstruct
	.type	 retstruct,@function
retstruct:
	pushl %ebp
	movl %esp,%ebp
	subl $24,%esp
	movl 8(%ebp),%eax
	movl $1,-8(%ebp)
	movb $2,-4(%ebp)
	movw $3,-2(%ebp)
	movl -8(%ebp),%edx
	movl -4(%ebp),%ecx
	movl %edx,(%eax)
	movl %ecx,4(%eax)
	jmp .L2
.L2:
	movl %eax,%eax
	movl %ebp,%esp
	popl %ebp
	ret $4
.Lfe1:
	.size	 retstruct,.Lfe1-retstruct
.section	.rodata
.LC0:
	.string	"%d, %d, %d\n"
.text
	.align 16
.globl passtruct
	.type	 passtruct,@function
passtruct:
	pushl %ebp
	movl %esp,%ebp
	subl $8,%esp
	movl $1,8(%ebp)
	movb $2,12(%ebp)
	movw $3,14(%ebp)
	movswl 14(%ebp),%eax
	pushl %eax
	movsbl 12(%ebp),%eax
	pushl %eax
	movl 8(%ebp),%eax
	pushl %eax
	pushl $.LC0
	call printf
	addl $16,%esp
.L3:
	movl %ebp,%esp
	popl %ebp
	ret
.Lfe2:
	.size	 passtruct,.Lfe2-passtruct
.section	.rodata
.LC1:
	.string	"%d %d %d\n"
.text
	.align 16
.globl main
	.type	 main,@function
main:
	pushl %ebp
	movl %esp,%ebp
	subl $24,%esp
	leal -8(%ebp),%eax
	addl $-12,%esp
	pushl %eax
	call retstruct
	addl $12,%esp
	addl $-8,%esp
	movl -8(%ebp),%eax
	movl -4(%ebp),%edx
	pushl %edx
	pushl %eax
	call passstruct
	addl $16,%esp
	movswl -2(%ebp),%eax
	pushl %eax
	movsbl -4(%ebp),%eax
	pushl %eax
	movl -8(%ebp),%eax
	pushl %eax
	pushl $.LC1
	call printf
	addl $16,%esp
.L4:
	movl %ebp,%esp
	popl %ebp
	ret
.Lfe3:
	.size	 main,.Lfe3-main
	.ident	"GCC: (GNU) 2.95.3 20010315 (SuSE)"
