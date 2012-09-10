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
	leal -4(%ebp),%eax
	pushl %eax
	leal -8(%ebp),%eax
	pushl %eax
	leal -12(%ebp),%eax
	pushl %eax
	pushl $.LC1
	call scanf
	movl -8(%ebp),%eax
	addl $32,%esp
	cmpl %eax,-12(%ebp)
	jge .L18
	cmpl -4(%ebp),%eax
	jge .L18
	addl $-12,%esp
	pushl $.LC2
	jmp .L26
	.p2align 4,,7
.L18:
	movl -12(%ebp),%ecx
	movl -8(%ebp),%edx
	cmpl %edx,%ecx
	je .L21
	movl -4(%ebp),%eax
	cmpl %eax,%edx
	je .L21
	cmpl %eax,%ecx
	jne .L19
.L21:
	addl $-12,%esp
	pushl $.LC3
.L26:
	call printf
	addl $16,%esp
.L19:
	addl $-12,%esp
	pushl $.LC4
	call printf
	movl -12(%ebp),%eax
	addl $16,%esp
	cmpl -8(%ebp),%eax
	jne .L24
	cmpl -4(%ebp),%eax
	je .L23
.L24:
	addl $-12,%esp
	movl 12(%ebp),%eax
	pushl 4(%eax)
	call atoi
	addl $16,%esp
	testl %eax,%eax
	jge .L22
.L23:
	addl $-12,%esp
	pushl $.LC5
	call printf
	jmp .L25
	.p2align 4,,7
.L22:
	addl $-12,%esp
	pushl $.LC6
	call printf
.L25:
	leave
	ret
.Lfe1:
	.size	 main,.Lfe1-main
	.ident	"GCC: (GNU) 2.95.4  (Debian prerelease)"
