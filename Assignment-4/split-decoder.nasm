;Filename: split-decoder.nasm
;Author: Jonathan So
;Student ID: SLAE-1470
;SLAE-Exercise 4

global _start			

section .text
_start:

	jmp short call_shellcode
decoder:
	pop esi
	lea edi, [esi + 3]	;second half of our encoded shellcode
	xor ebx, ebx		;init all register
	xor ecx, ecx
	mul ecx			;set EAX,EDX = 0
	mov edx, esp		;edx will point to our rearranged shellcode
	mov cl, 3		;loop will be len(shellcode)/2
decode: 
	mov bl, byte[esi]	;Copy value in esi
	mov byte[edx + eax], bl	;Rearrange in edx
	inc eax			
	mov bl, byte[edi]	;Copy value in edi
	mov byte[edx + eax], bl	;Rearrange in edx
	inc eax
	inc esi			;Pass to next value in esi
	inc edi			;Pass to next value in edi
	loop decode

	jmp edx

call_shellcode:

	call decoder
	EncodedShellcode: db 0x41,0x43,0x45,0x42,0x44,0x46
