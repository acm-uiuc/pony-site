	.file	"array-stack-int3D.c"
	.version	"01.01"
gcc2_compiled.:
.text
	.align 4
.globl intArray3D
	.type	 intArray3D,@function
intArray3D:
	pushl %ebp
	movl %esp,%ebp
	subl $1228,%esp
	pushl %edi
	pushl %esi
	pushl %ebx
	movl $0,-1204(%ebp)
	leal -1200(%ebp),%eax
	movl %eax,-1212(%ebp)
	.p2align 4,,7
.L21:
	xorl %esi,%esi
	movl -1204(%ebp),%edx
	incl %edx
	movl %edx,-1208(%ebp)
	movl -1204(%ebp),%edi
	leal (%edi,%edi,4),%eax
	leal (%eax,%eax,4),%ebx
	sall $4,%ebx
	.p2align 4,,7
.L25:
	xorl %ecx,%ecx
	leal 0(,%esi,4),%edx
	leal 1(%esi),%eax
	movl %eax,-1216(%ebp)
	leal (%esi,%edx),%eax
	sall $3,%eax
	addl %eax,%edx
	addl %ebx,%edx
	addl %ebx,%eax
	.p2align 4,,7
.L29:
	cmpl %ecx,%esi
	jne .L30
	movl -1204(%ebp),%edi
	movl %edi,-1200(%edx,%ebp)
	jmp .L28
	.p2align 4,,7
.L30:
	movl $0,-1200(%eax,%ebp)
.L28:
	addl $4,%eax
	incl %ecx
	cmpl $9,%ecx
	jle .L29
	movl -1216(%ebp),%esi
	cmpl $9,%esi
	jle .L25
	movl -1208(%ebp),%eax
	movl %eax,-1204(%ebp)
	cmpl $2,%eax
	jle .L21
	addl $-12,%esp
	movl -1212(%ebp),%edx
	pushl %edx
	call printArray
	leal -1240(%ebp),%esp
	popl %ebx
	popl %esi
	popl %edi
	leave
	ret
.Lfe1:
	.size	 intArray3D,.Lfe1-intArray3D
	.ident	"GCC: (GNU) 2.95.4  (Debian prerelease)"
