	.file	"if.c"
	.version	"01.01"
gcc2_compiled.:
.section	.rodata
.LC0:
	.string	"A is less than 0\n"
.text
	.align 4
.globl main
	.type	 main,@function
main:
        /* save ebp */
        pushl %ebp

        /* move esp to ebp so we can access vars from ebp */
        movl %esp,%ebp

        /* allocate stack space */
        subl $24,%esp

        /* compare a to 0. The way this comparason works is that
         * the subtraction a - 0 is performed, and all of the flags on p65-66
         * of the Intel Basic Archetecture manual are updated. */
        cmpl $0,-4(%ebp)

        /* If you check the Intel Instruction Reference, the conditions for 
         * jge are jump if SF == OF, ie jump if the result of the subtraction 
         * was positive and there was no overflow, or jump if the 
         * result of the subtraction was negative and there was an overflow */

        /* So the proper way to abstract all this away in your brain 
         * is to think of cmp a,b and jXX as a pair that says: 
         * "Jump if b XX a"
         */
    
        /* Jump if a ge 0, so jump to .L3 if (a >= 0) */
        jge .L3
        
        /* This code is now executed if (0 > a) */
        addl $-12,%esp
        pushl $.LC0
        call printf
        addl $16,%esp

.L3:
.L2:
	leave
	ret
.Lfe1:
	.size	 main,.Lfe1-main
	.ident	"GCC: (GNU) 2.95.4  (Debian prerelease)"
