	.file	"for.c"
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
	nop
        
        /* move 0 to var1 */
	movl $0,-4(%ebp)
	.p2align 4,,7
.L3:
        /* Jump if var1 le 9, ie if var1 <= 9 */
	cmpl $9,-4(%ebp)
	jle .L6
        /* exit loop */
	jmp .L4
	.p2align 4,,7
.L6:
        /* call to printf */
	addl $-8,%esp
	movl -4(%ebp),%eax
	pushl %eax
	pushl $.LC0
	call printf
	addl $16,%esp
.L5:
        /* var++ */
	incl -4(%ebp)
	jmp .L3
	.p2align 4,,7
        /* So we see that aside from some extra labels generated for each of
         * the sections of the loop, they are the same instructions */
.L4:
.L2:
	leave
	ret
.Lfe1:
	.size	 main,.Lfe1-main
	.ident	"GCC: (GNU) 2.95.4  (Debian prerelease)"
