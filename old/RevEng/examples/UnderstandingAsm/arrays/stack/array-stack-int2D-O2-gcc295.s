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

        /* Huge allocation. Must be an array */
	subl $412,%esp

        /* preserve registers */
	pushl %edi
	pushl %esi
	pushl %ebx

        /* %ebx = 0 */
	xorl %ebx,%ebx

        /* Think about where -400(%ebp) is on the stack, and how it is
         * aligned. The fact that it is such a nice number really suggests 
         * that we have the bottom of an array here 
         * %eax = array */
	leal -400(%ebp),%eax

        /* So this is kind of odd.. the pointer to the bottom of array is
         * being stored on the stack. Just make a note of it and move on. */
	movl %eax,-404(%ebp)
	movl %eax,%edi
	.p2align 4,,7
.L21:
        /* %ecx = 0 */
	xorl %ecx,%ecx

        /* %edx = %ebx*4 */
	leal 0(,%ebx,4),%edx

        /* %esi = %ebx + 1 */
	leal 1(%ebx),%esi

        /* %eax = %ebx + %edx = %ebx*5 */
	leal (%ebx,%edx),%eax

        /* %eax = %eax*8 =  %ebx*40 */
	sall $3,%eax

        /* %edx = %ebx*40 + %ebx*4 */
	addl %eax,%edx

        /* %eax = %ebx*40 + array */
	addl %edi,%eax
	.p2align 4,,7
.L25:
        /* if(%ebx != %ecx) jump */
	cmpl %ecx,%ebx
	jne .L26

        /* code executed if(%ebx = %ecx) */

        /* array + %edx = 1 */
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
