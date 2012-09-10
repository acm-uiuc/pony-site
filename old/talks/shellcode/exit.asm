;By Steve Hanna
;shanna@uiuc.edu
;http://www.vividmachines.com/
;Linux Shellcode
;exit.asm shellcode example
;this shellcode just calls sysexit

[SECTION .text]

global _start


_start:
	xor ebx, ebx
	mov al, 1
	int 0x80

