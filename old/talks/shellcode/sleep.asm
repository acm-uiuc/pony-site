;By Steve Hanna
;shanna@uiuc.edu
;http://www.vividmachines.com/
;Windows Shellcode
;sleep.asm shellcode example
;this shellcode suspends a thread
; for five seconds
[SECTION .text]

global _start


_start:
        xor eax,eax
		mov ebx, 0x77e61bea	;address of Sleep
        mov ax, 5000		;pause for 5000ms
        push eax
        call ebx		;Sleep(ms);
