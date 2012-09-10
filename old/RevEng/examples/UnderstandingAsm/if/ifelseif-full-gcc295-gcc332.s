	.file	"ifelseif.c"
	.version	"01.01"
gcc2_compiled.:
.section	.rodata
.LC0:
	.string	"A is less than 0\n"
.LC1:
	.string	"A is 0\n"
.LC2:
	.string	"A > 0\n"
.LC3:
	.string	"Leaving main\n"
.text
	.align 4
.globl main
	.type	 main,@function
main:
        /* again, not much has changed except this prolog. See if you can 
         * follow this program's flow without help from the comments */
	subl $12,%esp
	testl %eax,%eax
	jge .L18
	addl $-12,%esp
	pushl $.LC0
	jmp .L22
	.p2align 4,,7
.L18:
	testl %eax,%eax
	jne .L20
	addl $-12,%esp
	pushl $.LC1
	jmp .L22
	.p2align 4,,7
.L20:
	addl $-12,%esp
	pushl $.LC2
.L22:
	call printf
	addl $16,%esp
	addl $-12,%esp
	pushl $.LC3
	call printf
	addl $16,%esp
	addl $12,%esp
	ret
.Lfe1:
	.size	 main,.Lfe1-main
	.ident	"GCC: (GNU) 2.95.4  (Debian prerelease)"
