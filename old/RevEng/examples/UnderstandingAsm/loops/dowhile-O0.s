	.file	"dowhile.c"
	.version	"01.01"
gcc2_compiled.:
.section	.rodata
.LC0:
	.string	"%d\n"
.text
	.align 4
.globl main
	.type	 main,@function
main:
	pushl %ebp
	movl %esp,%ebp
	subl $24,%esp

        /* Move 0 to var1 */
	movl $0,-4(%ebp)
	.p2align 4,,7
.L3:
        /* call to printf */
	addl $-8,%esp
	movl -4(%ebp),%eax
	pushl %eax
	pushl $.LC0
	call printf
	addl $16,%esp

        /* var++ */
	incl -4(%ebp)
.L5:
        /* Now, here we see the comparason at the bottom, so that the loop
         * runs at least once before termination. Turns out the code for the 
         * comarison is generated the exact same way */
        
        /* jump if var1 <= 9 */
	cmpl $9,-4(%ebp)
	jle .L6
        
        /* else quit */
	jmp .L4
	.p2align 4,,7
.L6:
	jmp .L3
	.p2align 4,,7
.L4:
.L2:
	leave
	ret
.Lfe1:
	.size	 main,.Lfe1-main
	.ident	"GCC: (GNU) 2.95.4  (Debian prerelease)"
