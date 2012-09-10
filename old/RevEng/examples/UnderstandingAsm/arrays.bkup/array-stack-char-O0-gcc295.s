	.file	"array-stack-char.c"
	.version	"01.01"
gcc2_compiled.:
.section	.rodata
.LC0:
	.string	"hello there, govna!"
.text
	.align 4
.globl charArray
	.type	 charArray,@function
charArray:
	pushl %ebp
	movl %esp,%ebp
        /* Subtract enough space for the array and then some. Such large stack
         * allocations are a HUGE clue that somebody is working with arrays on 
         * the stack. */
	subl $520,%esp

	
        /* mystery arg to strncpy */
        addl $-4,%esp

        /* This line is perplexing at first, but scan down. Its the length
         * argument to strncpy. This gives us the hint that GCC allocated 8
         * extra bytes on the stack */
	pushl $511
        /* string to copy */
	pushl $.LC0
        /* address of the buffer to copy into */
	leal -512(%ebp),%eax
	pushl %eax
	call strncpy
        /* Post-call stack adjust */
	addl $16,%esp

        /* more mystery args */
	addl $-12,%esp
        /* Strlen */
	pushl $.LC0
	call strlen
        /* stack ajust */
	addl $16,%esp

        /* Return value transfer (unoptimized) */
	movl %eax,%eax

        /* put address of string into edx */
	leal -512(%ebp),%edx

	

        movb $0,(%eax,%edx)
        /* 
           Recall: disp(%base, %index, scale) = disp + %base + %index*scale. 
           In this case, base and scale were omitted, so we have the address 
           %eax + %edx. (Scale is assumed to be one). Since %eax contains the
           return value from strlen, we are doing string[strlen(.LC0)] = 0.
           In otherwords, we are null terminating the string, in case the
           strncpy call failed to copy everything. Think about this for a
           minute. This is a bug. Can you see why?

           Answer: If the strncpy call failed, LESS than .LC0 would have been 
           copied because there wasn't enough room! Hence this is a bug that we            have discovered through painstaking analysis of the assembly that the
           author of the C code overlooked! (To those of you who worry this may
           be a contrived example, I wrote the .c file, and didn't notice this 
           bug until looking at the assembly just now).

           Techniques to use bugs like this to our advantage will be discussed
           later, in the buffer overflow chapter.
         */


        /* mystery arg */ 
	addl $-12,%esp
	leal -512(%ebp),%eax
	pushl %eax
        /* 
           printArray is a bogus function that we call simply to prevent the
           optimizer from optimizing away all our code in future examples. 
         */
	call printArray
	addl $16,%esp
.L2:
	leave
	ret
.Lfe1:
	.size	 charArray,.Lfe1-charArray
	.ident	"GCC: (GNU) 2.95.4  (Debian prerelease)"
