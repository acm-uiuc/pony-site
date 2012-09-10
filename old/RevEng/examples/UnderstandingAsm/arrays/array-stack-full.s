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
	.align 4
.globl intArray
	.type	 intArray,@function
intArray:
	subl $2060,%esp
	movl %esp,%edx
	movl $511,%ecx
	leal 2044(%esp),%eax
	.p2align 4,,7
.L22:
	movl %ecx,(%eax)
	addl $-4,%eax
	decl %ecx
	jns .L22
	addl $-12,%esp
	pushl %edx
	call printArray
	addl $16,%esp
	addl $2060,%esp
	ret
.Lfe2:
	.size	 intArray,.Lfe2-intArray
	.align 4
.globl intArray2D
	.type	 intArray2D,@function
intArray2D:
	subl $412,%esp
	pushl %ebp
	pushl %edi
	pushl %esi
	pushl %ebx
	xorl %ebx,%ebx
	leal 16(%esp),%ebp
	movl %ebp,%edi
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
	pushl %ebp
	call printArray
	addl $16,%esp
	popl %ebx
	popl %esi
	popl %edi
	popl %ebp
	addl $412,%esp
	ret
.Lfe3:
	.size	 intArray2D,.Lfe3-intArray2D
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
.L41:
	xorl %esi,%esi
	leal 1(%ebp),%eax
	movl %eax,28(%esp)
	leal (%ebp,%ebp,4),%eax
	leal (%eax,%eax,4),%ebx
	sall $4,%ebx
	.p2align 4,,7
.L45:
	xorl %ecx,%ecx
	leal 0(,%esi,4),%edx
	leal 1(%esi),%edi
	leal (%esi,%edx),%eax
	sall $3,%eax
	addl %eax,%edx
	addl %ebx,%edx
	addl %ebx,%eax
	.p2align 4,,7
.L49:
	cmpl %ecx,%esi
	jne .L50
	movl %ebp,32(%esp,%edx)
	jmp .L48
	.p2align 4,,7
.L50:
	movl $0,32(%esp,%eax)
.L48:
	addl $4,%eax
	incl %ecx
	cmpl $9,%ecx
	jle .L49
	movl %edi,%esi
	cmpl $9,%esi
	jle .L45
	movl 28(%esp),%ebp
	cmpl $2,%ebp
	jle .L41
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
.Lfe4:
	.size	 intArray3D,.Lfe4-intArray3D
	.align 4
.globl main
	.type	 main,@function
main:
	ret
.Lfe5:
	.size	 main,.Lfe5-main
	.ident	"GCC: (GNU) 2.95.4  (Debian prerelease)"
