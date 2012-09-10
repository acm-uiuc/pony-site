	.file	"array-malloc-int1D.c"
	.version	"01.01"
gcc2_compiled.:
.section	.rodata
.LC0:
	.string	"Out of memory\n"
.text
	.align 4
.globl allocate1D
	.type	 allocate1D,@function
allocate1D:
	subl $12,%esp
	addl $-12,%esp
	movl 28(%esp),%eax
	sall $2,%eax
	pushl %eax
	call malloc
	addl $16,%esp
	testl %eax,%eax
	jne .L18
	addl $-12,%esp
	pushl $.LC0
	call printf
	addl $-12,%esp
	pushl $1
	call exit
	.p2align 4,,7
.L18:
	addl $12,%esp
	ret
.Lfe1:
	.size	 allocate1D,.Lfe1-allocate1D
	.align 4
.globl intArray
	.type	 intArray,@function
intArray:
	subl $12,%esp
	addl $-12,%esp
	pushl $2048
	call malloc
	addl $16,%esp
	testl %eax,%eax
	jne .L20
	addl $-12,%esp
	pushl $.LC0
	call printf
	addl $-12,%esp
	pushl $1
	call exit
	.p2align 4,,7
.L20:
	movl $511,%ecx
	leal 2044(%eax),%edx
	.p2align 4,,7
.L25:
	movl %ecx,(%edx)
	addl $-4,%edx
	decl %ecx
	jns .L25
	addl $-12,%esp
	pushl %eax
	call printArray
	addl $16,%esp
	addl $12,%esp
	ret
.Lfe2:
	.size	 intArray,.Lfe2-intArray
	.ident	"GCC: (GNU) 2.95.4  (Debian prerelease)"
