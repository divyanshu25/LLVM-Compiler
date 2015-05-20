section .data ;Data segment
	msg db "Compiler"
	msg_len equ $-msg  ;$
 	msg_rev db "relipmoC"
	msg_Upper db "COMPILER"
	temp DD 48D
	NL db 10D 
section .bss            ;Uninitialized data
	acc1 resb 4
	acc2 resb 4
section .text
	global _start    ;must be declared for linker (gcc)
_start:    ;tell linker entry point
	 mov esi ,msg_rev
	 mov edi ,msg
	 mov ecx ,msg_len
	cld
	rep	movsb
	mov eax,4
	mov ebx,1
	mov ecx,msg
	mov edx,msg_len
	int 80h

	mov eax,4
	mov ebx,1
	mov ecx,NL
	mov edx,1
	int 80h

	mov eax,1
	mov ebx,0
	int 80h

