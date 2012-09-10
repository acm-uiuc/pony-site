	.file	"array-malloc-int1D.c"
	.version	"01.01"
gcc2_compiled.:
.section	.rodata
.LC0:
	.string	"Out of memory\n"
.text
	.align 4
.globl allocate1D
	.type	 allocate1D,@function
allocate1D:
	pushl %ebp
	movl %esp,%ebp
	subl $8,%esp

        /* Mystery args */
	addl $-12,%esp
        /* Move arg1 to eax */
	movl 8(%ebp),%eax

        /* call malloc(arg1*4) */
	sall $2,%eax
	pushl %eax
	call malloc
	addl $16,%esp

	/* jump if (malloc(arg1*1) ne 0) */
        testl %eax,%eax
	jne .L18

        /* Print error message */
	addl $-12,%esp
	pushl $.LC0
	call printf
	addl $-12,%esp

        /* exit(1) */
	pushl $1
	call exit
	.p2align 4,,7
.L18:
	leave
	ret
.Lfe1:
	.size	 allocate1D,.Lfe1-allocate1D
	.align 4
.globl intArray
	.type	 intArray,@function
intArray:
	pushl %ebp
	movl %esp,%ebp
	subl $8,%esp

        /* call allocate1D(512) */
	addl $-12,%esp
	pushl $512
	call allocate1D
	addl $16,%esp

        /* ecx = 511 */
	movl $511,%ecx

        /* We know allocate1D allocates 4*arg1 bytes of memory. We requested
         * 512*4, or 2048. Here we see GCC was smart enough to see this (good 
         * lord), and indexed the last element in our malloc'ed array for the
         * loop.
         */
	leal 2044(%eax),%edx
	.p2align 4,,7
.L23:
        /* put %ecx into this memory location */
	movl %ecx,(%edx)
        /* go down 4 in memory */
	addl $-4,%edx
        /* %ecx-- */
	decl %ecx

        /* FIXME: jns? */
	jns .L23


        /* So what does the array contain after this loop completes? */

	addl $-12,%esp
	pushl %eax
	call printArray
	leave
	ret
.Lfe2:
	.size	 intArray,.Lfe2-intArray
	.ident	"GCC: (GNU) 2.95.4  (Debian prerelease)"
