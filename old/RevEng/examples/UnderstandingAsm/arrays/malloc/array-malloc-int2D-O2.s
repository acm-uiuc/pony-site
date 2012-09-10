	.file	"array-malloc-int2D.c"
	.version	"01.01"
gcc2_compiled.:
.section	.rodata
.LC0:
	.string	"Not enough space\n"
.text
	.align 4
.globl allocate2D
	.type	 allocate2D,@function
allocate2D:
	pushl %ebp
	movl %esp,%ebp
        /* stack space */
	subl $12,%esp
        /* Save the edi, esi, ebx registers */
	pushl %edi
	pushl %esi
	pushl %ebx

        /* arg1 */
	movl 8(%ebp),%edi

        /* %esi = malloc(4*arg1) */
	addl $-12,%esp
	leal 0(,%edi,4),%eax
	pushl %eax
	call malloc
	movl %eax,%esi
	addl $16,%esp

        /* if (retval ne 0), skip exit */
	testl %esi,%esi
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
        /* ebx = 0 */
	xorl %ebx,%ebx
        /* if (%ebx ge %edi) jump to return, ie if (arg1 <= 0) return */
	cmpl %edi,%ebx
	jge .L20
	.p2align 4,,7
.L22:
        
        /* malloc(4*arg2) */
	addl $-12,%esp
	movl 12(%ebp),%eax
	sall $2,%eax
	pushl %eax
	call malloc

        /* *(%esi + %ebx*4) = %eax 
         * Recall %esi is the return from the first malloc.
         * %ebx starts at 0 in the loop init */ 
	movl %eax,(%esi,%ebx,4)
	addl $16,%esp

        /* if(%eax ne 0) skip to L21. Ie if malloc == 0, exit */
	testl %eax,%eax
	jne .L21

        /* printf(.LC0) -> "out of memory" */
	addl $-12,%esp
	pushl $.LC0
	call printf
	addl $-12,%esp

        /* exit(1) */
	pushl $1
	call exit
	.p2align 4,,7
.L21:
        /* %ebx++ */
	incl %ebx

        /* if (%ebx l %edi) continue loop;
         * ie loop while(%ebx < arg1) */
	cmpl %edi,%ebx
	jl .L22
.L20:
        /* Move array to retval*/
	movl %esi,%eax

        /* Restore %esp to saved register region (there was 12 bytes of local
         * vars, and 3 4 byte registers on the stack after the %ebp was set.)
         */
	leal -24(%ebp),%esp
	popl %ebx
	popl %esi
	popl %edi
	leave
	ret
.Lfe1:
	.size	 allocate2D,.Lfe1-allocate2D
	.align 4
.globl intArray2D
	.type	 intArray2D,@function
intArray2D:
	pushl %ebp
	movl %esp,%ebp
	subl $16,%esp
        /* Save esi, ebx (edi not used) */
	pushl %esi
	pushl %ebx
        
        /* ebx = allocate2D(10,10); %ecx = 0; */
	addl $-8,%esp
	pushl $10
	pushl $10
	call allocate2D
	movl %eax,%ebx
	xorl %ecx,%ecx
	addl $16,%esp


	.p2align 4,,7
.L29:

        /* %edx = 0 */
	xorl %edx,%edx

        /* %esi = 1+%ecx, starting at %ecx = 0 */
	leal 1(%ecx),%esi
	.p2align 4,,7
.L33:

        /* if (%ecx ne %edx) jump to else clause */
	cmpl %edx,%ecx
	jne .L34

        /* %eax = *(ebx + %ecx*4)
         * %eax = array[%ecx] */ 
	movl (%ebx,%ecx,4),%eax

        /* Since array[%ecx] is points to an array of ints, we can index into
         * it by adding 4*index to it.
         * *(array[%ecx] + ecx*4) = 1;
         * array[%ecx][%ecx] = 1;
         */
	movl $1,(%eax,%ecx,4)

        /* Jump past else clause */
	jmp .L32
	.p2align 4,,7
.L34:
        /* %eax = array[%ecx] */
	movl (%ebx,%ecx,4),%eax

        /* array[%ecx][%ebx] = 0 */
	movl $0,(%eax,%edx,4)
.L32:

        /* edx++ */
	incl %edx
        /* if (%edx <= 9) loop */
	cmpl $9,%edx
	jle .L33

        /* %ecx = %esi, but from before, %esi = 1+ecx and does not change.
         * so %ecx = %ecx+1. So this is just a complicated increment
         * statement, written this way so that the compiler would be free to
         * change %ecx kif need be. (Also recall that %ecx is NOT saved across
         * function calls, another reason for this style of increment) */
         */
	movl %esi,%ecx

        /* if (%ecx <= 9) loop 
         *
         * So we see two loops at this point, an inner and an outer.
         * The inner loops over %edx, the outer over %ecx */
	cmpl $9,%ecx
	jle .L29


        /* So, lets see if we can predict the output of printArray, if it
         * actually existed that is. 

         * So we have two loops.. How do they work? Lets summarize the
         * whole function:
         * intArray2D()
         * {
         *   Allocate the array, store it in %ebx
         *   Set %ecx = 0
         * 
         *   Label1:
         *    set %edx = 0;
         *    Label2:
         *     if (%ecx == %edx)
         *       array[%ecx][%ecx] = 1;
         *     else
         *       array[%ecx][%edx] = 0;
         *
         *     %edx++
         *
         *     if (%edx <= 9) 
         *       goto label2;
         *
         *     %ecx = %ecx + 1;
         *
         *     if(%ecx <= 9)
         *       goto label1;
         * }
         *
         * So it looks as if gcc took our for loop and turned it into a
         * do-while loop, but the operation is the same. We iterate over %edx 
         * filling in each column with a 1 or a 0, depending on if we are on
         * the diagonal, and we iterate over %ecx filling in each row in this
         * fashion. Nested loop, diagonal of our array is 1.
         */ 


	addl $-12,%esp
	pushl %ebx
	call printArray

        /* restore regs */
	leal -24(%ebp),%esp
	popl %ebx
	popl %esi
	leave
	ret
.Lfe2:
	.size	 intArray2D,.Lfe2-intArray2D
	.ident	"GCC: (GNU) 2.95.4  (Debian prerelease)"
