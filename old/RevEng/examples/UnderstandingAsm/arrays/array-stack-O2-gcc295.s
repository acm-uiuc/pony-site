	.file	"array-stack.c"
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
.L22:
	movl %ecx,(%eax)
	addl $-4,%eax
	decl %ecx
	jns .L22
	addl $-12,%esp
	pushl %edx
	call printArray
	leave
	ret
.Lfe2:
	.size	 intArray,.Lfe2-intArray
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
.L28:
	xorl %ecx,%ecx
	leal 0(,%ebx,4),%edx
	leal 1(%ebx),%esi
	leal (%ebx,%edx),%eax
	sall $3,%eax
	addl %eax,%edx
	addl %edi,%eax
	.p2align 4,,7
.L32:
	cmpl %ecx,%ebx
	jne .L33
	movl $1,(%edx,%edi)
	jmp .L31
	.p2align 4,,7
.L33:
	movl $0,(%eax)
.L31:
	addl $4,%eax
	incl %ecx
	cmpl $9,%ecx
	jle .L32
	movl %esi,%ebx
	cmpl $9,%ebx
	jle .L28
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
.Lfe3:
	.size	 intArray2D,.Lfe3-intArray2D
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
.L41:
	xorl %esi,%esi
	movl -1204(%ebp),%edx
	incl %edx
	movl %edx,-1208(%ebp)
	movl -1204(%ebp),%edi
	leal (%edi,%edi,4),%eax
	leal (%eax,%eax,4),%ebx
	sall $4,%ebx
	.p2align 4,,7
.L45:
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
.L49:
	cmpl %ecx,%esi
	jne .L50
	movl -1204(%ebp),%edi
	movl %edi,-1200(%edx,%ebp)
	jmp .L48
	.p2align 4,,7
.L50:
	movl $0,-1200(%eax,%ebp)
.L48:
	addl $4,%eax
	incl %ecx
	cmpl $9,%ecx
	jle .L49
	movl -1216(%ebp),%esi
	cmpl $9,%esi
	jle .L45
	movl -1208(%ebp),%eax
	movl %eax,-1204(%ebp)
	cmpl $2,%eax
	jle .L41
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
.Lfe4:
	.size	 intArray3D,.Lfe4-intArray3D
	.align 4
.globl main
	.type	 main,@function
main:
	pushl %ebp
	movl %esp,%ebp
	leave
	ret
.Lfe5:
	.size	 main,.Lfe5-main
	.ident	"GCC: (GNU) 2.95.4  (Debian prerelease)"
