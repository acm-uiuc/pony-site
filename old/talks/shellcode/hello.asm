;By Steve Hanna
;shanna@uiuc.edu
;http://www.vividmachines.com/
;Linux Shellcode
;hello.asm shellcode example
;this shellcode uses the syscall write
;to output text to the screen
;also demonstrated dynamic string location

[SECTION .text]

global _start


_start:

	jmp short ender 

	starter:
	
	xor eax, eax	;clean up the registers
	xor ebx, ebx
	xor edx, edx
	xor ecx, ecx

	mov al, 4	;syscall write
	mov bl, 1	;stdout is 1
	pop ecx		;get the address of the string from the stack
	mov dl, 5	;length of the string
	int 0x80

	xor eax, eax
	mov al, 1	;exit the shellcode
	xor ebx,ebx
	int 0x80	
	
	ender:
	call starter
	db 'hello'
