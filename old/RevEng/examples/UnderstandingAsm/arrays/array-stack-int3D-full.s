	.file	"array-stack-int3D.c"
	.version	"01.01"
gcc2_compiled.:
.text
	.align 4
.globl intArray3D
	.type	 intArray3D,@function
intArray3D:
	subl $1228,%esp
	pushl %ebp
	pushl %edi
	pushl %esi
	pushl %ebx
	xorl %ebp,%ebp
	leal 32(%esp),%eax
	movl %eax,24(%esp)
	.p2align 4,,7
.L21:
	xorl %esi,%esi
	leal 1(%ebp),%eax
	movl %eax,28(%esp)
	leal (%ebp,%ebp,4),%eax
	leal (%eax,%eax,4),%ebx
	sall $4,%ebx
	.p2align 4,,7
.L25:
	xorl %ecx,%ecx
	leal 0(,%esi,4),%edx
	leal 1(%esi),%edi
	leal (%esi,%edx),%eax
	sall $3,%eax
	addl %eax,%edx
	addl %ebx,%edx
	addl %ebx,%eax
	.p2align 4,,7
.L29:
	cmpl %ecx,%esi
	jne .L30
	movl %ebp,32(%esp,%edx)
	jmp .L28
	.p2align 4,,7
.L30:
	movl $0,32(%esp,%eax)
.L28:
	addl $4,%eax
	incl %ecx
	cmpl $9,%ecx
	jle .L29
	movl %edi,%esi
	cmpl $9,%esi
	jle .L25
	movl 28(%esp),%ebp
	cmpl $2,%ebp
	jle .L21
	addl $-12,%esp
	movl 36(%esp),%eax
	pushl %eax
	call printArray
	addl $16,%esp
	popl %ebx
	popl %esi
	popl %edi
	popl %ebp
	addl $1228,%esp
	ret
.Lfe1:
	.size	 intArray3D,.Lfe1-intArray3D
	.ident	"GCC: (GNU) 2.95.4  (Debian prerelease)"
