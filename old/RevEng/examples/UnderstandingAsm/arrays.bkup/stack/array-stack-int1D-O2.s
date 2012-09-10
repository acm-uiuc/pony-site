	.file	"array-stack-int1D.c"
	.version	"01.01"
gcc2_compiled.:
.text
	.align 4
.globl intArray
	.type	 intArray,@function
intArray:
	pushl %ebp
	movl %esp,%ebp

        /* A whole lot of stack space is clue to an array */
	subl $2056,%esp

        /* leals are clue to the fact that we are going to be doing some more
         * indexing in the future. From this its save to assume that -2048
         * down from %ebp is our array, and local variables are after it. */
	leal -2048(%ebp),%edx

	movl $511,%ecx

        /* Here is the top of our array */
	leal -4(%ebp),%eax
	.p2align 4,,7
.L21:

        /* *%eax = %ecx;.. Note: 32bit integer operation */
	movl %ecx,(%eax)

        /* move %eax down by 4. We are now sure we're dealing with ints here */
	addl $-4,%eax

        /* Decrement counter */
	decl %ecx

        /* JNS means jump if not signed, ie if the result of the previous
         * instruction was not negative. So jump if %ecx >= 0 */
	jns .L21

        /* So can you predict the results of the following imaginary
         * printArray call? Our resulting code is a bit different than
         * the original code. Instead of running the loop forwards, the
         * optimizer has decided that we should start at index 511, and run
         * backwards until %ecx < 0. So the array is still numbered 0..511, we
         * just did it in reverse. Pretty strange optimization, eh?
         */

	addl $-12,%esp
	pushl %edx
	call printArray
	leave
	ret
.Lfe1:
	.size	 intArray,.Lfe1-intArray
	.ident	"GCC: (GNU) 2.95.4  (Debian prerelease)"
