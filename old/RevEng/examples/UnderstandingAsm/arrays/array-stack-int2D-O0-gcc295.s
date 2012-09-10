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
        /* Lots of stack space.. Clue that we're working with arrays */
	subl $424,%esp
	nop

        /* Give -404(%ebp) the label var1 on your stack sheet, set it 0 */
        /* This also gives us a bound on the total array size.. Most likely
         * they specified the array first, then the vars */
	movl $0,-404(%ebp)
	.p2align 4,,7
.L3:
        /* Uh oh.. a loop! */
        /* "Jump if var1 le 9" -> Loop while var1 <= 9 */
	cmpl $9,-404(%ebp)
	jle .L6
	jmp .L4
	.p2align 4,,7
.L6:
        /* Lable this space var2 */
	movl $0,-408(%ebp)
	.p2align 4,,7
.L7:

        /* Hrmm.. could this be a nested loop? YEP! */

        /* "Loop while var2 <= 9" */
	cmpl $9,-408(%ebp)
	jle .L10
	jmp .L5
	.p2align 4,,7
.L10: /* Loop body */
        /* move var1 to eax */ 
	movl -404(%ebp),%eax
        /* Jump if var2 ne var1 */
	cmpl -408(%ebp),%eax
	jne .L11

        /* Code executed if (var2 == var1) */
	
        /* Put var2 into eax */
        movl -408(%ebp),%eax
	movl %eax,%edx

        /* Indexing operation coming! (%eax = var2*4*/
	leal 0(,%edx,4),%eax

        /* put var1 into ecx, then edx */
	movl -404(%ebp),%ecx
	movl %ecx,%edx

        /* The sal instruction bitshifts the operand left by the specified
         * number. It is basically a faster way of multiplying by powers of 2.*/
        /* %edx *= 4; (edx = var1*4)*/
	sall $2,%edx
        
        /* %edx = var1 * 5 */
	addl %ecx,%edx

        /* %ecx = var1 * 5 * 8 = var1 * 40 (hrmm.. 40 is 10*4... coincidence?)*/
	leal 0(,%edx,8),%ecx

        
        /* %eax = var1*40 + var2*4 */
	addl %ecx,%eax

        /* Put the base of the array into %edx */
	leal -400(%ebp),%edx

        /* put 1 into the address %eax + %edx. You see that gcc likes to use
         * the base and index backwards if there is no scale.. Lord only knows..
         * 
         * The important thing to notice is that we have stored a 1 at memory
         * location array + var1*40 + var2*4, and we have done it HORRIBLY
         * inefficiently! (A human should have been able to do this with 2
         * leals and an add). 
         *
         * Why 40 and 4? Well, recall that 2D arrays on the
         * stack of the form  'type array[dim2][dim1];' 
         * are represented by a single array of size type*dim1*dim2. So 
         * visualize long array as being divided into rows now (like text that
         * wraps around the screen). To get to the var1 row, we have to go past
         * var1*dim1*type cells, and to get to the var2 column, we have to add 
         * on var2*type cells. Thus array[var1][var2] is
         *
         * array + var1*dim1*type + var2*type.
         */
	movl $1,(%eax,%edx)
	jmp .L9
	.p2align 4,,7
.L11:
        /* Else clause to if(var2 == var1) */

        /* put var2 into eax */
	movl -408(%ebp),%eax
	movl %eax,%edx

        /* eax now has var2*4 */
	leal 0(,%edx,4),%eax

        /* ecx has var1 */
	movl -404(%ebp),%ecx
	movl %ecx,%edx

        /* edx = var1*4 */
	sall $2,%edx
        /* edx = var1*5 (because ecx = var1) */
	addl %ecx,%edx

        /* ecx = var1*40 */
	leal 0(,%edx,8),%ecx
        
        /* eax = var1*40 + var2*4 */
	addl %ecx,%eax

        /* Base of our array in edx */
	leal -400(%ebp),%edx
        
        /* put the zero in eax */
	movl $0,(%eax,%edx)
.L12:
.L9:

        /* var2++ */
	incl -408(%ebp)
	jmp .L7
	.p2align 4,,7
.L8:
.L5:
        /* var1++ */
	incl -404(%ebp)
	jmp .L3
	.p2align 4,,7
.L4:

        /* So, can you visualize what this code is doing based on the assembly
         * we just went through without reverting back to the C code? 
         * What does the 2D array look like after the program is done? Can you
         * draw it in 2D? How about in 1D? How about on the stack? (recall it 
         * is on the stack) */


        /* Answer: 
           So let's summarize:
           We have an outer loop that is iterating over var1 until it hits 10
            We have an inner loop that is iterating over var2 until it hits 10
             The inner loop sets array[var1][var2] to 1 if var1 == var2
             else it sets array[var1][var2] to 0.

           So can you draw the array now? 
         */
        
	addl $-12,%esp
	leal -400(%ebp),%eax
	pushl %eax
	call printArray
	addl $16,%esp
.L2:
	leave
	ret
.Lfe1:
	.size	 intArray2D,.Lfe1-intArray2D
	.ident	"GCC: (GNU) 2.95.4  (Debian prerelease)"
