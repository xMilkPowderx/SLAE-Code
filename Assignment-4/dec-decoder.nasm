;Filename: dec-decoder.nasm
;Author: Jonathan So
;Student ID: SLAE-1470
;SLAE-Exercise 4

global _start			

section .text
_start:

	jmp short call_shellcode
decoder:
	pop esi			
	xor ecx, ecx
	mul ecx
	mov cl, 6		;length of shellcode
decode: 
	mov al, byte[esi]	;decrease the value byte by byte
	dec eax
	mov byte[esi], al
	inc esi
	loop decode
	jmp EncodedShellcode	;execute decoded shellcode

call_shellcode:

	call decoder
	EncodedShellcode: db 0x42,0x43,0x44,0x45,0x46,0x47





