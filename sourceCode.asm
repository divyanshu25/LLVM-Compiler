section .data ;Data segment
	gr db 49,50,51,51,52,53,53,54,55,
	gr_len equ 1
	a DD 48D
	a_len equ 1
	x DD 50D
	x_len equ 1
	y DD 50D
	y_len equ 1
	temp DD 48D
	NL db 10D 
section .bss            ;Uninitialized data
	acc1 resb 4
	acc2 resb 4
section .text
	global _start    ;must be declared for linker (gcc)
_start:    ;tell linker entry point
	mov ecx,gr
	 mov ebx,[x]
	sub ebx,'0'
	 mov eax,3
	 mul ebx
	add eax,[y]
	sub eax,'0'
	mov edx,eax
	add ecx,edx
	mov eax,[ecx]
	mov [a],eax
	mov eax,4
	mov ebx,1
	mov ecx,a
	mov edx,a_len
	int 80h

	mov eax,4
	mov ebx,1
	mov ecx,NL
	mov edx,1
	int 80h

	mov eax,1
	mov ebx,0
	int 80h

