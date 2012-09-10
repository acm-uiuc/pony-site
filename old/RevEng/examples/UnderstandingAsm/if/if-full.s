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
        /* Notice we have no function prolog with -fomit-frame-pointer */
        /* Also notice that we STILL allocate unneeded stack space.. go gcc! */
	subl $12,%esp
        
        /* Again that odd use of test */
	testl %eax,%eax

        /* jump if (%eax ge 0) */
	jge .L18


	addl $-12,%esp
	pushl $.LC0
	call printf
	addl $16,%esp
.L18:
	addl $12,%esp
	ret
.Lfe1:
	.size	 main,.Lfe1-main
	.ident	"GCC: (GNU) 2.95.4  (Debian prerelease)"
