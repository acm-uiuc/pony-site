	.file	"array-stack-int2D.c"
	.version	"01.01"
gcc2_compiled.:
.text
	.align 4
.globl intArray2D
	.type	 intArray2D,@function
intArray2D:
	pushl %ebp
	movl %esp,%ebp
	subl $412,%esp
	pushl %edi
	pushl %esi
	pushl %ebx
	xorl %ebx,%ebx
	leal -400(%ebp),%eax
	movl %eax,-404(%ebp)
	movl %eax,%edi
	.p2align 4,,7
.L21:
	xorl %ecx,%ecx
	leal 0(,%ebx,4),%edx
	leal 1(%ebx),%esi
	leal (%ebx,%edx),%eax
	sall $3,%eax
	addl %eax,%edx
	addl %edi,%eax
	.p2align 4,,7
.L25:
	cmpl %ecx,%ebx
	jne .L26
	movl $1,(%edx,%edi)
	jmp .L24
	.p2align 4,,7
.L26:
	movl $0,(%eax)
.L24:
	addl $4,%eax
	incl %ecx
	cmpl $9,%ecx
	jle .L25
	movl %esi,%ebx
	cmpl $9,%ebx
	jle .L21
	addl $-12,%esp
	movl -404(%ebp),%eax
	pushl %eax
	call printArray
	leal -424(%ebp),%esp
	popl %ebx
	popl %esi
	popl %edi
	leave
	ret
.Lfe1:
	.size	 intArray2D,.Lfe1-intArray2D
	.ident	"GCC: (GNU) 2.95.4  (Debian prerelease)"
