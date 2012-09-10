	.file	"ifcomplicated.c"
	.version	"01.01"
gcc2_compiled.:
.section	.rodata
.LC0:
	.string	"Enter some values:\n"
.LC1:
	.string	"%d\n%d\n%d\n"
.LC2:
	.string	"a < b < c\n"
.LC3:
	.string	"One pair of a,b,c is equal\n"
.LC4:
	.string	"End test 1\n"
.LC5:
	.string	"SOme stuff happened\n"
.LC6:
	.string	"Some stuff didn't happen\n"
.text
	.align 4
.globl main
	.type	 main,@function
main:
	pushl %ebp
	movl %esp,%ebp
	subl $24,%esp
	addl $-12,%esp
	pushl $.LC0
	call printf
	addl $16,%esp
	leal -12(%ebp),%eax
	pushl %eax
	leal -8(%ebp),%eax
	pushl %eax
	leal -4(%ebp),%eax
	pushl %eax
	pushl $.LC1
	call scanf
	addl $16,%esp
	movl -4(%ebp),%eax
	cmpl -8(%ebp),%eax
	jge .L3
	movl -8(%ebp),%eax
	cmpl -12(%ebp),%eax
	jge .L3
	addl $-12,%esp
	pushl $.LC2
	call printf
	addl $16,%esp
	jmp .L4
	.p2align 4,,7
.L3:
	movl -4(%ebp),%eax
	cmpl -8(%ebp),%eax
	je .L6
	movl -8(%ebp),%eax
	cmpl -12(%ebp),%eax
	je .L6
	movl -4(%ebp),%eax
	cmpl -12(%ebp),%eax
	je .L6
	jmp .L4
	.p2align 4,,7
.L6:
	addl $-12,%esp
	pushl $.LC3
	call printf
	addl $16,%esp
.L5:
.L4:
	addl $-12,%esp
	pushl $.LC4
	call printf
	addl $16,%esp
	movl -4(%ebp),%eax
	cmpl -8(%ebp),%eax
	jne .L9
	movl -8(%ebp),%eax
	cmpl -12(%ebp),%eax
	je .L8
	jmp .L9
	.p2align 4,,7
.L9:
	addl $-12,%esp
	movl 12(%ebp),%eax
	addl $4,%eax
	movl (%eax),%edx
	pushl %edx
	call atoi
	addl $16,%esp
	movl %eax,%eax
	testl %eax,%eax
	jl .L8
	jmp .L7
	.p2align 4,,7
.L8:
	addl $-12,%esp
	pushl $.LC5
	call printf
	addl $16,%esp
	jmp .L10
	.p2align 4,,7
.L7:
	addl $-12,%esp
	pushl $.LC6
	call printf
	addl $16,%esp
.L10:
.L2:
	leave
	ret
.Lfe1:
	.size	 main,.Lfe1-main
	.ident	"GCC: (GNU) 2.95.4  (Debian prerelease)"
