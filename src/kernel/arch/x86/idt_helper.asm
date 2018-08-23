section .rodata

section .text
global handler_wrapper
extern int_handler

handler_wrapper:
	pusha
	call int_handler
	popa
	iret