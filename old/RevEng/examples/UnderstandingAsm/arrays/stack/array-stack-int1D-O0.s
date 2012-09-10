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
        
        /* Woah thats a lot of space */
	subl $2072,%esp
        
        /* nop is a Null OPeration. It does nothing but padd our instruction
         * space */
	nop

        /* Set some variable var1 to zero. (Keep track of it on your stack 
         * sheet!) */
	movl $0,-2052(%ebp)

        /* alignment noise */
	.p2align 4,,7
.L3:

        /* Scanning ahead, we see what looks like it could be a loop: Double
         * jump, label here, label after comparason.. */
        /* Recall: "Jump if -2052(%ebp) le $511" */
	cmpl $511,-2052(%ebp)
	jle .L7

        /* if var1 > 511, exit loop */
	jmp .L5
	.p2align 4,,7
.L6:

        /* put var1 in eax */
	movl -2052(%ebp),%eax
	movl %eax,%edx

        /* Here we see our indexing operation begin:
           Place var1*4 into %eax */
	leal 0(,%edx,4),%eax

        /* place the address of some nicely aligned quantity into %edx 
         (A large array, perhaps?) */
	leal -2048(%ebp),%edx
        
        /* Place var1 into ecx */
	movl -2052(%ebp),%ecx

        /* *(%eax + %edx) = %ecx; -> array1[var1] = var1 
           (because %eax = var1*4 */
	movl %ecx,(%eax,%edx)
.L5:
        /* var1++ */
	incl -2052(%ebp)
        /* loop */
	jmp .L4
	.p2align 4,,7
.L4:
        /* Printarray call to prevent over-optimization */
	addl $-12,%esp
	leal -2048(%ebp),%eax
	pushl %eax
	call printArray
	addl $16,%esp
.L2:
	leave
	ret
.Lfe1:
	.size	 intArray,.Lfe1-intArray
	.ident	"GCC: (GNU) 2.95.4  (Debian prerelease)"
