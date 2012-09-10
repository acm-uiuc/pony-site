	.file	"struct.c"
	.version	"01.01"
gcc2_compiled.:
.section	.rodata
.LC0:
	.string	"%d, %d, %d\n"
.LC1:
	.string	"%d %d %d\n"
.text
	.align 16
.globl retstruct
	.type	 retstruct,@function
retstruct:
	pushl %ebx
	movl $1,%edx
	movb $2,%cl
	movl %ecx,%eax
	andl $65535,%eax
	movl %eax,%ecx
	orl $196608,%ecx
	movl 8(%esp),%ebx
	movl %edx,(%ebx)
	movl %ecx,4(%ebx)
	movl %ebx,%eax
	popl %ebx
	ret $4
.Lfe1:
	.size	 retstruct,.Lfe1-retstruct
	.align 16
.globl passtruct
	.type	 passtruct,@function
passtruct:
	subl $12,%esp
	movl 20(%esp),%edx
	movb $2,%dl
	movl %edx,%ecx
	andl $65535,%ecx
	orl $196608,%ecx
	movl %ecx,%edx
	shrl $16,%ecx
	movsbl %dl,%eax
	pushl %ecx
	pushl %eax
	pushl $1
	pushl $.LC0
	call printf
	addl $16,%esp
	addl $12,%esp
	ret
.Lfe2:
	.size	 passtruct,.Lfe2-passtruct
	.align 16
.globl main
	.type	 main,@function
main:
	subl $16,%esp
	movl %edi,%edx
	pushl %edi
	pushl %esi
	pushl %ebx
	movl $1,%esi
	movl %esi,%eax
	movb $2,%dl
	movl %edx,%edi
	movl %edi,%ebx
	andl $65535,%ebx
	orl $196608,%ebx
	movl %ebx,%edi
	addl $-8,%esp
	pushl %edi
	pushl %esi
	call passstruct
	shrl $16,%ebx
	pushl %ebx
	movl %edi,%edx
	movsbl %dl,%eax
	pushl %eax
	pushl $1
	pushl $.LC1
	call printf
	addl $32,%esp
	popl %ebx
	popl %esi
	popl %edi
	addl $16,%esp
	ret
.Lfe3:
	.size	 main,.Lfe3-main
	.ident	"GCC: (GNU) 2.95.3 20010315 (SuSE)"
