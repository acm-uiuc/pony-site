	.file	"array-stack-int1D.c"
	.version	"01.01"
gcc2_compiled.:
.text
	.align 4
.globl intArray
	.type	 intArray,@function
intArray:
	subl $2060,%esp
	movl %esp,%edx
	movl $511,%ecx
	leal 2044(%esp),%eax
	.p2align 4,,7
.L21:
	movl %ecx,(%eax)
	addl $-4,%eax
	decl %ecx
	jns .L21
	addl $-12,%esp
	pushl %edx
	call printArray
	addl $16,%esp
	addl $2060,%esp
	ret
.Lfe1:
	.size	 intArray,.Lfe1-intArray
	.ident	"GCC: (GNU) 2.95.4  (Debian prerelease)"
