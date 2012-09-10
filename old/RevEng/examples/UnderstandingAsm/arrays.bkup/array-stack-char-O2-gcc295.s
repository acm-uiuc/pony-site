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
	pushl %ebp
	movl %esp,%ebp
	subl $532,%esp
	pushl %ebx
	addl $-4,%esp
	pushl $511
	pushl $.LC0
	leal -512(%ebp),%ebx
	pushl %ebx
	call strncpy
	movb $0,-493(%ebp)
	addl $-12,%esp
	pushl %ebx
	call printArray
	movl -536(%ebp),%ebx
	leave
	ret
.Lfe1:
	.size	 charArray,.Lfe1-charArray
	.ident	"GCC: (GNU) 2.95.4  (Debian prerelease)"
