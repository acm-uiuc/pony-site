	.file	"array-stack-int3D.c"
	.version	"01.01"
gcc2_compiled.:
.text
	.align 4
.globl intArray3D
	.type	 intArray3D,@function
intArray3D:
	pushl %ebp
	movl %esp,%ebp
        /* Woah thats a lot of memory */
	subl $1224,%esp
	nop
        /* Set var1 = 0 */
	movl $0,-1204(%ebp)
	.p2align 4,,7
.L3:
        /* While(var1 <= 2) */
	cmpl $2,-1204(%ebp)
	jle .L6
	jmp .L4
	.p2align 4,,7
.L6:
        /* set var2 = 0 */
	movl $0,-1208(%ebp)
	.p2align 4,,7
.L7:
        /* While(var2 <= 9) */
	cmpl $9,-1208(%ebp)
	jle .L10
	jmp .L5
	.p2align 4,,7
.L10:
        /* Set var3 = 0 */
	movl $0,-1212(%ebp)
	.p2align 4,,7
.L11:
        /* While(var3 <= 9) */
	cmpl $9,-1212(%ebp)
	jle .L14
	jmp .L9
	.p2align 4,,7
.L14:
        /* var2 -> eax */
	movl -1208(%ebp),%eax

        /* if(var2 != var3) then jump*/
	cmpl -1212(%ebp),%eax
	jne .L15

        /* code executed if(var2 == var3) */

        /* place var3 in eax */       
	movl -1212(%ebp),%eax
	movl %eax,%edx

        /* eax = var3 *4 */
	leal 0(,%edx,4),%eax

        /* place var2 in ecx */
	movl -1208(%ebp),%ecx
	movl %ecx,%edx

        /* edx = var2*4 */
	sall $2,%edx
        /* edx = var2*5 */
	addl %ecx,%edx

        /* ecx = var2*40 */
	leal 0(,%edx,8),%ecx

        /* eax = var2*40 + var3 * 4 */
	addl %ecx,%eax

        /* ecx = var1 */
	movl -1204(%ebp),%ecx
	movl %ecx,%edx

        /* edx = var1*4 */
	sall $2,%edx
        /* edx = var1*5 */
	addl %ecx,%edx

        /* ecx = var1*20 */
	leal 0(,%edx,4),%ecx

        /* edx = var1*25 */
	addl %ecx,%edx
	movl %edx,%ecx

        /* ecx = var1*25*16 = var1*100*4 = var1*400 */
	sall $4,%ecx

        /* eax = var1*400 + var2*40 + var3*4 */
	addl %ecx,%eax
        
        /* edx = base of array */
	leal -1200(%ebp),%edx

        /* ecx = var1 */
	movl -1204(%ebp),%ecx

        /* set *(array + var1*400 + var2*40 + var3*4) = var1.
         * So: array[var1][var2][var3] = var1;
         *
         * Can we guess the dimensions of our array at this point yet?
         *
         * From the formula given, 400 = dim2*dim1*type, 40 = dim1*type,
         * 4=type.
         * 
         * So type is int, dim1 is 10, dim2 is 10, dim3 is unknown.
         * For a hint at dim3, what does the loop iterate var1 over? 
         * It executes so long as var1 <= 2. So our array is probably declared
         * as:
         * int array[3][10][10];
         */
	movl %ecx,(%eax,%edx)
	jmp .L13
	.p2align 4,,7
.L15:
        /* else clause for if(var2 == var3) */

        /* this is pretty much the same code as above.. with one exception.. */
	movl -1212(%ebp),%eax
	movl %eax,%edx

	leal 0(,%edx,4),%eax
	movl -1208(%ebp),%ecx
	movl %ecx,%edx
	sall $2,%edx
	addl %ecx,%edx
	leal 0(,%edx,8),%ecx
	addl %ecx,%eax
	movl -1204(%ebp),%ecx
	movl %ecx,%edx
	sall $2,%edx
	addl %ecx,%edx
	leal 0(,%edx,4),%ecx
	addl %ecx,%edx
	movl %edx,%ecx
	sall $4,%ecx
	addl %ecx,%eax
	leal -1200(%ebp),%edx

        /* set *(array + var1*400 + var2*40 + var3*4) = 0 */
	movl $0,(%eax,%edx)
.L16:
.L13:
        /* var3++ */
	incl -1212(%ebp)
	jmp .L11
	.p2align 4,,7
.L12:
.L9:
        /* var2++ */
	incl -1208(%ebp)
	jmp .L7
	.p2align 4,,7
.L8:
.L5:
        /* var1++ */
	incl -1204(%ebp)
	jmp .L3
	.p2align 4,,7
.L4:

        /* So can you visualize what is going on with our 3D array?
         * What does it look like? You should be able to do this on your own
         * with little to no difficulty now.
         */

	addl $-12,%esp
	leal -1200(%ebp),%eax
	pushl %eax
	call printArray
	addl $16,%esp
.L2:
	leave
	ret
.Lfe1:
	.size	 intArray3D,.Lfe1-intArray3D
	.ident	"GCC: (GNU) 2.95.4  (Debian prerelease)"
