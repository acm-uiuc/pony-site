	.file	"functions.c"
	.version	"01.01"
gcc2_compiled.:
.section	.rodata
.LC0:
	.string	"%d, %d, %d\n"
.text
	.align 4
.globl function3args
	.type	 function3args,@function
function3args:
	/* This push saves the ebp, and in combination with the move is called
	 * the function prolog. */
	pushl %ebp /* at (%ebp) on the stack */
	movl %esp,%ebp

	/* This subl is used to allocate space for any local variables. In
	 * this case we have none, and we can see the fact that this
	 * instruction is useless because no stack references are negative
	 * offsets from the %ebp (visualize or draw the stack to see this).
	 * I'm not sure why GCC does this. */
	subl $8,%esp 
	/* (%esp) == -8(%ebp) */
	
	/* remember our comments. This instruction copies the last argument of
	 * the function to %eax*/
	movl 16(%ebp),%eax
	
	/* push this value as the last argument to the printf call. 
	 * Note: This is why we have an %ebp register, because this push will 
	 * affect the %esp, not the %ebp, and our references to local
	 * variables all remain the same still. */
	pushl %eax 
	/* (%esp) == -12(%ebp) */

	/* Now access the second argument of the function, and push it */
	movl 12(%ebp),%eax
	pushl %eax 
	/* (%esp) == -16(%ebp) */

	/* Access the first argument of the function. Remember that the
	 * remaining two things below 8(%ebp) are the return address at
	 * 4(%ebp) and the old value of %ebp, which is at (%ebp) */
	movl 8(%ebp),%eax
	pushl %eax 
	/* (%esp) == -20(%ebp) */

	/* Push the string onto the stack */
	pushl $.LC0
	/* (%esp) == -24(%ebp) */
	call printf
	/* (%esp) == -24(%ebp) because the stack is reset fixed after a call */
	
	/* Again, "pop" all 16 bytes of arguments off the stack */
	addl $16,%esp

	/* (%esp) == -8(%ebp) */
	
.L2:
	/* Leave copies the value of %ebp into %esp, effectively popping all
	 * extra local variables and junk off the stack. It then pops the top
	 * value off the stack (which is the saved %ebp) and stores it in %ebp
	 *
	 * So it is basically the reverse of the function
	 * prolog, and implicityly removes any local variables and junk that
	 * GCC may have thrown on the stack. This is key, because GCC loves to
	 * throw junk on the stack for no reason. It is all taken care of at
	 * function exit because of this instruction */
	leave

	/* (%esp) == (%ebp) == (old %ebp) just after call */

	/* pops the return address saved on the stack into %eip, and thus
	 * execution transfers to just after the call */
	ret
.Lfe1:
	.size	 function3args,.Lfe1-function3args
	.align 4
.globl function3argsRet
	.type	 function3argsRet,@function
function3argsRet:
	pushl %ebp
	movl %esp,%ebp

	/* Move the first argument to %edx */
        /* The first argument is at 8 above the ebp. Ie it as at the lowest
         * address of all arguments. The rest are at higher address */
	movl 8(%ebp),%edx

	/* multiply the second argument with %edx, store in %edx */
	imull 12(%ebp),%edx

	/* multiply the third argument with %edx, store in %edx */
	imull 16(%ebp),%edx

	/* Move %edx to %eax. %eax is the return value */
	movl %edx,%eax

	/* Alignment junk */
	jmp .L3
	.p2align 4,,7
.L3:
	leave
	ret
.Lfe2:
	.size	 function3argsRet,.Lfe2-function3argsRet
	.align 4
.globl functionPtrArg
	.type	 functionPtrArg,@function
functionPtrArg:
	pushl %ebp
	movl %esp,%ebp
	subl $8,%esp

	/* move the third argument (the pointer) into eax */
	movl 16(%ebp),%eax

	/* derefrence it. Remember how I said that leal does not deref, but
         * mov does? */
	movl (%eax),%edx

	/* push the rest of the args, and call printf */
	pushl %edx
	movl 12(%ebp),%eax
	pushl %eax
	movl 8(%ebp),%eax
	pushl %eax
	pushl $.LC0
	call printf
	addl $16,%esp
.L4:
	leave
	ret
.Lfe3:
	.size	 functionPtrArg,.Lfe3-functionPtrArg
	.align 4
.globl functionPtrRet
	.type	 functionPtrRet,@function
functionPtrRet:
	pushl %ebp
	movl %esp,%ebp

	/* Put the first argument of our function */
	movl 8(%ebp),%eax
	movl %eax,%edx

	/* put the address made by 0 + %edx*4 into register %eax */
	leal 0(,%edx,4),%eax
	movl %eax,%edx

	/* Add the third argument of our function (the pointer) to the result */
	addl 16(%ebp),%edx

	/* Put the second arg into eax */
	movl 12(%ebp),%eax
	movl %eax,%ecx

	/* put the address 0 + %ecx*4 into %eax. */
	leal 0(,%ecx,4),%eax

	/* add %eax to %edx, store in %edx.
         * If you were keeping track of the registers like you should have been,
         * you should now realize that %edx contains pointer + second_arg*4 +
         * third_ard*4. In other words, we know pointer is an integer pointer
         * because the scale was 4 during all the pointer arithmetic */
	addl %eax,%edx

	/* Put the result into the return value register %eax */
	movl %edx,%eax
	jmp .L5
	.p2align 4,,7
.L5:
	leave
	ret
.Lfe4:
	.size	 functionPtrRet,.Lfe4-functionPtrRet
	.align 4
.globl functionLocalVars
	.type	 functionLocalVars,@function
functionLocalVars:
	pushl	%ebp
	movl	%esp, %ebp
	/* so this is enough space for 4 integer variables, but sometimes GCC
	 * allocates more space than it needs, especially in recent versions.
         * Note in this case, we have only THREE variables in our function.
         * But we will actually get to see GCC use this magic local variable
         * in a bit. Most times we aren't so lucky. */
	subl	$16, %esp

	/* recall 12 from ebp is the second 4-byte function argument (note
         * that if this function had non-integer arguments, 12(%ebp) might be 
         * like the 3rd or 5th argument. Just something to keep in mind) */
	movl	12(%ebp), %eax
	
	/* XOR the second function arg with the first function arg */
	xorl	8(%ebp), %eax

	/* Store it in the first local variable. So the first local variable
         * now contains arg1 ^ arg2. This update of a local variable should
         * clue you into the completetion of a C statement.
         * In this case, we have determined that the statement was 
         * local1 = arg1 ^ arg2; 
         */
	movl	%eax, -4(%ebp)



	/* put the first arg into %edx */
	movl	8(%ebp), %edx

	/* Take the address of the second function arg.. */
	leal	12(%ebp), %eax

	/* put it into what appears to be the fourth local variable (again, 
         * it could be the the 9th, 17th, etc)
	 * 
	 * HOWEVER, NOTE: We do NOT have 4 local variables in the
	 * corresponding C code. GCC has created a temporary here to do the
	 * calculation. This is further evidence of non-optimized code. */
	movl	%eax, -16(%ebp)

	/* check your sheet for %edx */
	movl	%edx, %eax

	/* Move the fourth local variable into %ecx. So, following your sheet, 
         * %ecx now contains the address of the second function arg. */
	movl	-16(%ebp), %ecx

	/* FIXME: BUH? */
	cltd

	/* So here's an odd intruction. Basically, if you check the Intel
         * Instruction set reference, you see that idiv takes a single
         * argument of either a register %reg or an indirected register (ie a
         * register containing a memory location, (%reg)) and then divides 
         * %eax by the value in %reg or at memory location (%reg). The result is
         * stored in %eax, and the remainder is in %edx.
	 */

	/* Do: %eax = %eax/(%ecx); %edx = %eax MOD (%ecx); 
         * so from your sheet, %eax = arg1/arg2; %edx = arg1 MOD arg2 */
	idivl	(%ecx)

	/* Move result to second local variable. So local2 = arg1 / arg2; */
	movl	%eax, -8(%ebp)


	
	/* Move first arg to %edx */
	movl	8(%ebp), %edx

	/* Put the address of the second arg into %eax */
	leal	12(%ebp), %eax

	/* Use that temporary variable again */
	movl	%eax, -16(%ebp)
	movl	%edx, %eax
	movl	-16(%ebp), %ecx

	cltd

	/* %eax = %eax/(%ecx); %edx = %eax MOD (%ecx); 
         * So, %eax = arg1/arg2; %edx = arg1 MOD arg2;
	 */
	idivl	(%ecx)

	/* Store %edx into third local variable. So local3 = arg1 MOD arg2; */
	movl	%edx, -12(%ebp)



	/* Put the local2 into %eax */	
	movl	-8(%ebp), %eax

	/* %eax = local1 | %eax */
	orl	-4(%ebp), %eax

	/* local3 = local1 | local2 */
	movl	%eax, -12(%ebp)

	/* Put local2 into eax */
	movl	-12(%ebp), %eax

	/* %eax = local1 & local2 */
	andl	8(%ebp), %eax
	
	/* Junk instruction that says return %eax */
	movl	%eax, %eax
	leave
	ret
.Lfe5:
	.size	 functionLocalVars,.Lfe5-functionLocalVars
	.align 4
.globl main
	.type	 main,@function
main:
	/* save ebp */
	pushl %ebp

	/* move esp to ebp so we can access vars from ebp */
	movl %esp,%ebp

	/* allocate stack space.. Notice that gcc likes to allocate WAY more
	 * space than it needs in some cases.. why this is, I don't know.
	 * We really only need 4 bytes of space here for our int a, and a
	 * quick scroll through the function shows that -4(%ebp) is the only
	 * local variable we use */
	subl $24,%esp
#APP
	nop
#NO_APP
	/* So here we see that GCC pushes some mystery arg onto the stack, 
	 * and then the three arguments in reverse order, followed by the call
	 * to function3args. Remember that the call instruction places the
	 * address of the next instruction onto the stack. So at the entrance
	 * to function3args, esp points to the return address, and we have 20
	 * bytes above the esp, including ret and the mystery argument.
	 *
	 * However, since we are working on source generated without
	 * -fomit-frame-pointer, there will be a push of the ebp, and then the
	 * esp will be copied to ebp, and variables will be referenced from the
	 * ebp.
	 */
	addl $-4,%esp 		/* 20(%ebp) after prolog */
	pushl $3 		/* 16(ebp) */
	pushl $2 		/* 12(%ebp) */
	pushl $1		/* 8(%ebp) */
	call function3args 	/* 4(%ebp) */

	/* Go to function3args and see the comments there to see these
	 * variables in action */
	
	/* This stack ajustment is the same as popping all 4 arguments off the
	 * stack, ie the 3 integers and the mystery arg. */
	addl $16,%esp
#APP
	nop
#NO_APP

	/* So this function is the same exact deal as the previous, except we
	 * have a return value. GCC uses the eax register to store the return
	 * value  of a function.
         * A good excercise would be to follow the stack along yourself with 
         * a sheet of paper for this example. */
	addl $-4,%esp
	pushl $3
	pushl $2
	pushl $1
	call function3argsRet
	addl $16,%esp

	/* Junk instruction, unoptimized code */	
	movl %eax,%eax

	/* Notice now that %eax is copied into the first local variable */
	movl %eax,-4(%ebp)
#APP
	nop
#NO_APP

	/* This function exists as an example of what happens when you have a
         * pointer as an argument. */
	addl $-4,%esp

	/* the lea instruction loads the effective address of its first
         * argument and places it in the second. In other words, it simply 
         * adds the offset to the register being indexed, and then moves that
         * into the destination. 
         *
         * It is easy to become confused with this instruction, because it
         * actually does NOT derefrence the first arg, where as a mov does.
	 */

	/* Load the address of the first local variable into %eax */
	leal -4(%ebp),%eax

	/* push it. Thus the pointer is the third argument */
	pushl %eax
	pushl $3
	pushl $1
	call functionPtrArg
	addl $16,%esp
#APP
	nop
#NO_APP
	/* The example is the same as the previous, except we return a 
         * pointer */
	addl $-4,%esp
	
	leal -4(%ebp),%eax
	pushl %eax
	pushl $3
	pushl $1
	call functionPtrRet
	addl $16,%esp
	movl %eax,%eax
	/* Put the value in %eax into the second local variable. So the second
	 * var must be an int pointer from out conclusions in functionPtrRet */
	movl %eax,-8(%ebp)
#APP
	nop
#NO_APP
	/* This example is intended to show how a function handles local
	 * variables as always being negative offsets from the %ebp */

	/* Here we see another mystery stack allocation.. */
	subl	$8, %esp
	pushl	$2
	pushl	$1
	call	functionLocalVars
	addl	$16, %esp
	movl	%eax, %eax
	movl	%eax, -4(%ebp)
#APP
	nop
#NO_APP

.L6:
	leave
	ret
.Lfe6:
	.size	 main,.Lfe6-main
	.ident	"GCC: (GNU) 2.95.4  (Debian prerelease)"
// vim:noet
