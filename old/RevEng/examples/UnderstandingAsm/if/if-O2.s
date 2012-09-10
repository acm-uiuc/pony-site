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
        /* Save ebp */
	pushl %ebp
        /* Work off of sp */
	movl %esp,%ebp
        
        /* allocate space - Notice it goes unused. I'm still not sure why
         * gcc does this. 
         */
	subl $8,%esp
        
        /* 
         * Here we see that GCC has decided to use the test instruction in a
         * very wierd way. If you look at the Intel instruction reference
         * manual, you see that they are using the SF flag that is set with 
         * the sign bit (remember the section we did on two's complement?) 
         * of %eax AND %eax. This allows them to use jge, which
         * jumps on the condition that (SF = OF). Since OF is set to 0 by
         * test, the jge jumps to L18 on the condition that the sign bit of
         * %eax is 0. In otherwords, we jump to the end of the function 
         * if ( %eax >= 0 ).
         *
         */
	testl %eax,%eax
        
        /* So the general way to abstract away a test a,a, jXX pair is to say:
         * "Jump if (a XX 0)"
         */
        
        /* if ( %eax >= 0) then jump */
        jge .L18

        /* following code is executed if (%eax < 0 ) */
	addl $-12,%esp
	pushl $.LC0
	call printf
.L18:
	leave
	ret
.Lfe1:
	.size	 main,.Lfe1-main
	.ident	"GCC: (GNU) 2.95.4  (Debian prerelease)"
