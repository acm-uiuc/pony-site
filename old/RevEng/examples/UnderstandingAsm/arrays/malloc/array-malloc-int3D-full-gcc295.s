	.file	"array-malloc-int3D.c"
	.version	"01.01"
gcc2_compiled.:
.section	.rodata
.LC0:
	.string	"Not enough space\n"
.text
	.align 4
.globl intArray3D
	.type	 intArray3D,@function
intArray3D:
	subl $12,%esp
	pushl %ebp
	pushl %edi
	pushl %esi
	pushl %ebx
	addl $-12,%esp
	pushl $12
	call malloc
	movl %eax,%esi
	addl $16,%esp
	testl %esi,%esi
	jne .L32
	addl $-12,%esp
	pushl $.LC0
	call printf
	addl $-12,%esp
	pushl $1
	call exit
	.p2align 4,,7
.L32:
	xorl %edi,%edi
	.p2align 4,,7
.L35:
	addl $-12,%esp
	pushl $40
	call malloc
	movl %eax,(%esi,%edi,4)
	addl $16,%esp
	testl %eax,%eax
	jne .L36
	addl $-12,%esp
	pushl $.LC0
	call printf
	addl $-12,%esp
	pushl $1
	call exit
	.p2align 4,,7
.L36:
	xorl %ebx,%ebx
	leal 1(%edi),%ebp
	.p2align 4,,7
.L39:
	addl $-12,%esp
	pushl $40
	call malloc
	movl %eax,%edx
	movl (%esi,%edi,4),%eax
	movl %edx,(%eax,%ebx,4)
	addl $16,%esp
	testl %edx,%edx
	jne .L41
	addl $-12,%esp
	pushl $.LC0
	call printf
	addl $-12,%esp
	pushl $1
	call exit
	.p2align 4,,7
.L41:
	incl %ebx
	cmpl $10,%ebx
	jl .L39
	movl %ebp,%edi
	cmpl $3,%edi
	jl .L35
	xorl %ebx,%ebx
	.p2align 4,,7
.L49:
	xorl %ecx,%ecx
	leal 1(%ebx),%ebp
	.p2align 4,,7
.L53:
	xorl %edx,%edx
	leal 1(%ecx),%edi
	.p2align 4,,7
.L57:
	cmpl %edx,%ecx
	jne .L58
	movl (%esi,%ebx,4),%eax
	movl (%eax,%ecx,4),%eax
	movl %ebx,(%eax,%ecx,4)
	jmp .L56
	.p2align 4,,7
.L58:
	movl (%esi,%ebx,4),%eax
	movl (%eax,%ecx,4),%eax
	movl $0,(%eax,%edx,4)
.L56:
	incl %edx
	cmpl $9,%edx
	jle .L57
	movl %edi,%ecx
	cmpl $9,%ecx
	jle .L53
	movl %ebp,%ebx
	cmpl $2,%ebx
	jle .L49
	addl $-12,%esp
	pushl %esi
	call printArray
	addl $16,%esp
	popl %ebx
	popl %esi
	popl %edi
	popl %ebp
	addl $12,%esp
	ret
.Lfe1:
	.size	 intArray3D,.Lfe1-intArray3D
	.align 4
.globl allocate3D
	.type	 allocate3D,@function
allocate3D:
	subl $12,%esp
	pushl %ebp
	pushl %edi
	pushl %esi
	pushl %ebx
	addl $-12,%esp
	movl 44(%esp),%eax
	sall $2,%eax
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
	xorl %edi,%edi
	cmpl 32(%esp),%edi
	jge .L20
	.p2align 4,,7
.L22:
	addl $-12,%esp
	movl 48(%esp),%eax
	sall $2,%eax
	pushl %eax
	call malloc
	movl %eax,(%esi,%edi,4)
	addl $16,%esp
	testl %eax,%eax
	jne .L23
	addl $-12,%esp
	pushl $.LC0
	call printf
	addl $-12,%esp
	pushl $1
	call exit
	.p2align 4,,7
.L23:
	xorl %ebx,%ebx
	leal 1(%edi),%ebp
	cmpl 36(%esp),%ebx
	jge .L21
	.p2align 4,,7
.L27:
	addl $-12,%esp
	movl 52(%esp),%eax
	sall $2,%eax
	pushl %eax
	call malloc
	movl %eax,%edx
	movl (%esi,%edi,4),%eax
	movl %edx,(%eax,%ebx,4)
	addl $16,%esp
	testl %edx,%edx
	jne .L26
	addl $-12,%esp
	pushl $.LC0
	call printf
	addl $-12,%esp
	pushl $1
	call exit
	.p2align 4,,7
.L26:
	incl %ebx
	cmpl 36(%esp),%ebx
	jl .L27
.L21:
	movl %ebp,%edi
	cmpl 32(%esp),%edi
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
	.size	 allocate3D,.Lfe2-allocate3D
	.ident	"GCC: (GNU) 2.95.4  (Debian prerelease)"
