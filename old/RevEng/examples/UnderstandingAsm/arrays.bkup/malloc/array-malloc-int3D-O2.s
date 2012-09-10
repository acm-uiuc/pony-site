	.file	"array-malloc-int3D.c"
	.version	"01.01"
gcc2_compiled.:
.section	.rodata
.LC0:
	.string	"Not enough space\n"
.text
	.align 4
.globl allocate3D
	.type	 allocate3D,@function
allocate3D:
	pushl %ebp
	movl %esp,%ebp
	subl $12,%esp
	pushl %edi
	pushl %esi
	pushl %ebx
	addl $-12,%esp
	movl 8(%ebp),%eax
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
	cmpl 8(%ebp),%edi
	jge .L20
	.p2align 4,,7
.L22:
	addl $-12,%esp
	movl 12(%ebp),%eax
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
	leal 1(%edi),%eax
	movl %eax,-4(%ebp)
	cmpl 12(%ebp),%ebx
	jge .L21
	.p2align 4,,7
.L27:
	addl $-12,%esp
	movl 16(%ebp),%eax
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
	cmpl 12(%ebp),%ebx
	jl .L27
.L21:
	movl -4(%ebp),%edi
	cmpl 8(%ebp),%edi
	jl .L22
.L20:
	movl %esi,%eax
	leal -24(%ebp),%esp
	popl %ebx
	popl %esi
	popl %edi
	leave
	ret
.Lfe1:
	.size	 allocate3D,.Lfe1-allocate3D
	.align 4
.globl intArray3D
	.type	 intArray3D,@function
intArray3D:
	pushl %ebp
	movl %esp,%ebp
	subl $12,%esp
	pushl %edi
	pushl %esi
	pushl %ebx
	addl $-4,%esp
	pushl $10
	pushl $10
	pushl $3
	call allocate3D
	movl %eax,%esi
	xorl %ebx,%ebx
	addl $16,%esp
	.p2align 4,,7
.L35:
	xorl %ecx,%ecx
	leal 1(%ebx),%eax
	movl %eax,-4(%ebp)
	.p2align 4,,7
.L39:
	xorl %edx,%edx
	leal 1(%ecx),%edi
	.p2align 4,,7
.L43:
	cmpl %edx,%ecx
	jne .L44
	movl (%esi,%ebx,4),%eax
	movl (%eax,%ecx,4),%eax
	movl %ebx,(%eax,%ecx,4)
	jmp .L42
	.p2align 4,,7
.L44:
	movl (%esi,%ebx,4),%eax
	movl (%eax,%ecx,4),%eax
	movl $0,(%eax,%edx,4)
.L42:
	incl %edx
	cmpl $9,%edx
	jle .L43
	movl %edi,%ecx
	cmpl $9,%ecx
	jle .L39
	movl -4(%ebp),%ebx
	cmpl $2,%ebx
	jle .L35
	addl $-12,%esp
	pushl %esi
	call printArray
	leal -24(%ebp),%esp
	popl %ebx
	popl %esi
	popl %edi
	leave
	ret
.Lfe2:
	.size	 intArray3D,.Lfe2-intArray3D
	.ident	"GCC: (GNU) 2.95.4  (Debian prerelease)"
