	.file	"array-stack.c"
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
        /* Subtract enough space for the array and then some */
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

        /* Set some variable to zero. (Keep track of it on your stack sheet!) */
	movl $0,-2052(%ebp)

        /* alignment noise */
	.p2align 4,,7
.L4:
        /* Compare our variable to 0. (Remember the if and loop examples?) */
	cmpl $511,-2052(%ebp)

	jle .L7
	jmp .L5
	.p2align 4,,7
.L7:
	movl -2052(%ebp),%eax
	movl %eax,%edx
	leal 0(,%edx,4),%eax
	leal -2048(%ebp),%edx
	movl -2052(%ebp),%ecx
	movl %ecx,(%eax,%edx)
.L6:
	incl -2052(%ebp)
	jmp .L4
	.p2align 4,,7
.L5:
	addl $-12,%esp
	leal -2048(%ebp),%eax
	pushl %eax
	call printArray
	addl $16,%esp
.L3:
	leave
	ret
.Lfe2:
	.size	 intArray,.Lfe2-intArray
	.align 4
.globl intArray2D
	.type	 intArray2D,@function
intArray2D:
	pushl %ebp
	movl %esp,%ebp
	subl $424,%esp
	nop
	movl $0,-404(%ebp)
	.p2align 4,,7
.L9:
	cmpl $9,-404(%ebp)
	jle .L12
	jmp .L10
	.p2align 4,,7
.L12:
	movl $0,-408(%ebp)
	.p2align 4,,7
.L13:
	cmpl $9,-408(%ebp)
	jle .L16
	jmp .L11
	.p2align 4,,7
.L16:
	movl -404(%ebp),%eax
	cmpl -408(%ebp),%eax
	jne .L17
	movl -408(%ebp),%eax
	movl %eax,%edx
	leal 0(,%edx,4),%eax
	movl -404(%ebp),%ecx
	movl %ecx,%edx
	sall $2,%edx
	addl %ecx,%edx
	leal 0(,%edx,8),%ecx
	addl %ecx,%eax
	leal -400(%ebp),%edx
	movl $1,(%eax,%edx)
	jmp .L15
	.p2align 4,,7
.L17:
	movl -408(%ebp),%eax
	movl %eax,%edx
	leal 0(,%edx,4),%eax
	movl -404(%ebp),%ecx
	movl %ecx,%edx
	sall $2,%edx
	addl %ecx,%edx
	leal 0(,%edx,8),%ecx
	addl %ecx,%eax
	leal -400(%ebp),%edx
	movl $0,(%eax,%edx)
.L18:
.L15:
	incl -408(%ebp)
	jmp .L13
	.p2align 4,,7
.L14:
.L11:
	incl -404(%ebp)
	jmp .L9
	.p2align 4,,7
.L10:
	addl $-12,%esp
	leal -400(%ebp),%eax
	pushl %eax
	call printArray
	addl $16,%esp
.L8:
	leave
	ret
.Lfe3:
	.size	 intArray2D,.Lfe3-intArray2D
	.align 4
.globl intArray3D
	.type	 intArray3D,@function
intArray3D:
	pushl %ebp
	movl %esp,%ebp
	subl $1224,%esp
	nop
	movl $0,-1204(%ebp)
	.p2align 4,,7
.L20:
	cmpl $2,-1204(%ebp)
	jle .L23
	jmp .L21
	.p2align 4,,7
.L23:
	movl $0,-1208(%ebp)
	.p2align 4,,7
.L24:
	cmpl $9,-1208(%ebp)
	jle .L27
	jmp .L22
	.p2align 4,,7
.L27:
	movl $0,-1212(%ebp)
	.p2align 4,,7
.L28:
	cmpl $9,-1212(%ebp)
	jle .L31
	jmp .L26
	.p2align 4,,7
.L31:
	movl -1208(%ebp),%eax
	cmpl -1212(%ebp),%eax
	jne .L32
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
	movl -1204(%ebp),%ecx
	movl %ecx,(%eax,%edx)
	jmp .L30
	.p2align 4,,7
.L32:
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
	movl $0,(%eax,%edx)
.L33:
.L30:
	incl -1212(%ebp)
	jmp .L28
	.p2align 4,,7
.L29:
.L26:
	incl -1208(%ebp)
	jmp .L24
	.p2align 4,,7
.L25:
.L22:
	incl -1204(%ebp)
	jmp .L20
	.p2align 4,,7
.L21:
	addl $-12,%esp
	leal -1200(%ebp),%eax
	pushl %eax
	call printArray
	addl $16,%esp
.L19:
	leave
	ret
.Lfe4:
	.size	 intArray3D,.Lfe4-intArray3D
	.align 4
.globl main
	.type	 main,@function
main:
	pushl %ebp
	movl %esp,%ebp
.L34:
	leave
	ret
.Lfe5:
	.size	 main,.Lfe5-main
	.ident	"GCC: (GNU) 2.95.4  (Debian prerelease)"
