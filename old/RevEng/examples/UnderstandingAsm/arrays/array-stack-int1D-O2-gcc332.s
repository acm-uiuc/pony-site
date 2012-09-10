	.file	"array-stack-int1D.c"
	.version	"01.01"
gcc2_compiled.:
.text
	.align 4
.globl intArray
	.type	 intArray,@function
intArray:
	pushl %ebp
	movl %esp,%ebp
	subl $2056,%esp
	leal -2048(%ebp),%edx
	movl $511,%ecx
	leal -4(%ebp),%eax
	.p2align 4,,7
.L21:
	movl %ecx,(%eax)
	addl $-4,%eax
	decl %ecx
	jns .L21
	addl $-12,%esp
	pushl %edx
	call printArray
	leave
	ret
.Lfe1:
	.size	 intArray,.Lfe1-intArray
	.ident	"GCC: (GNU) 2.95.4  (Debian prerelease)"
