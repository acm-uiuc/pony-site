	.file	"array-malloc-int2D.c"
	.version	"01.01"
gcc2_compiled.:
.section	.rodata
.LC0:
	.string	"Not enough space\n"
.text
	.align 4
.globl intArray2D
	.type	 intArray2D,@function
intArray2D:
	subl $20,%esp
	pushl %esi
	pushl %ebx
	addl $-12,%esp
	pushl $40
	call malloc
	movl %eax,%esi
	addl $16,%esp
	testl %esi,%esi
	jne .L26
	addl $-12,%esp
	pushl $.LC0
	call printf
	addl $-12,%esp
	pushl $1
	call exit
	.p2align 4,,7
.L26:
	xorl %ebx,%ebx
	.p2align 4,,7
.L29:
	addl $-12,%esp
	pushl $40
	call malloc
	movl %eax,(%esi,%ebx,4)
	addl $16,%esp
	testl %eax,%eax
	jne .L31
	addl $-12,%esp
	pushl $.LC0
	call printf
	addl $-12,%esp
	pushl $1
	call exit
	.p2align 4,,7
.L31:
	incl %ebx
	cmpl $10,%ebx
	jl .L29
	xorl %ecx,%ecx
	.p2align 4,,7
.L37:
	xorl %edx,%edx
	leal 1(%ecx),%ebx
	.p2align 4,,7
.L41:
	cmpl %edx,%ecx
	jne .L42
	movl (%esi,%ecx,4),%eax
	movl $1,(%eax,%ecx,4)
	jmp .L40
	.p2align 4,,7
.L42:
	movl (%esi,%ecx,4),%eax
	movl $0,(%eax,%edx,4)
.L40:
	incl %edx
	cmpl $9,%edx
	jle .L41
	movl %ebx,%ecx
	cmpl $9,%ecx
	jle .L37
	addl $-12,%esp
	pushl %esi
	call printArray
	addl $16,%esp
	popl %ebx
	popl %esi
	addl $20,%esp
	ret
.Lfe1:
	.size	 intArray2D,.Lfe1-intArray2D
	.align 4
.globl allocate2D
	.type	 allocate2D,@function
allocate2D:
	subl $12,%esp
	pushl %ebp
	pushl %edi
	pushl %esi
	pushl %ebx
	movl 32(%esp),%edi
	movl 36(%esp),%ebp
	addl $-12,%esp
	leal 0(,%edi,4),%eax
	pushl %eax
	call malloc
	movl %eax,%esi
	addl $16,%esp
	testl %esi,%esi
	jne .L18
	addl $-12,%esp
	pushl $.LC0
	call printf
	addl $-12,%esp
	pushl $1
	call exit
	.p2align 4,,7
.L18:
	xorl %ebx,%ebx
	cmpl %edi,%ebx
	jge .L20
	.p2align 4,,7
.L22:
	addl $-12,%esp
	leal 0(,%ebp,4),%eax
	pushl %eax
	call malloc
	movl %eax,(%esi,%ebx,4)
	addl $16,%esp
	testl %eax,%eax
	jne .L21
	addl $-12,%esp
	pushl $.LC0
	call printf
	addl $-12,%esp
	pushl $1
	call exit
	.p2align 4,,7
.L21:
	incl %ebx
	cmpl %edi,%ebx
	jl .L22
.L20:
	movl %esi,%eax
	popl %ebx
	popl %esi
	popl %edi
	popl %ebp
	addl $12,%esp
	ret
.Lfe2:
	.size	 allocate2D,.Lfe2-allocate2D
	.ident	"GCC: (GNU) 2.95.4  (Debian prerelease)"
