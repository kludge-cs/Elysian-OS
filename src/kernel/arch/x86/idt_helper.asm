section .rodata

section .text
global handler_wrapper
extern int_handler
global idt_load
extern idt_pointer
idt_load:
	xchg bx, bx ;do we at least fucking get here?
	lidt [idt_pointer]
	ret

handler_wrapper:
	xchg bx, bx ;test if we actually made it
	pusha
	call int_handler
	popa
	iret