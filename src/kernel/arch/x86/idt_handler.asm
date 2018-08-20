section .rodata
txt: db "Unknown exception"

section .text
global default_handler
extern puts
default_handler:
	pusha
	mov edi, txt
	push edi
	mov eax, puts
	call eax
	iret