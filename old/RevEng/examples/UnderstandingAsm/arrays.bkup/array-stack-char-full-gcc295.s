	.file	"array-stack-char.c"
	.version	"01.01"
gcc2_compiled.:
.section	.rodata
.LC0:
	.string	"hello there, govna!"
.text
	.align 4
.globl charArray
	.type	 charArray,@function
charArray:
	subl $536,%esp
	pushl %ebx
	addl $-4,%esp
	pushl $511
	pushl $.LC0
	leal 28(%esp),%ebx
	pushl %ebx
	call strncpy
	movb $0,51(%esp)
	addl $-12,%esp
	pushl %ebx
	call printArray
	addl $32,%esp
	popl %ebx
	addl $536,%esp
	ret
.Lfe1:
	.size	 charArray,.Lfe1-charArray
	.ident	"GCC: (GNU) 2.95.4  (Debian prerelease)"
