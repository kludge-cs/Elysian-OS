%macro isr_noval 1
	isr%1:
		cli
		push byte 0 ;no error
		push byte %1
		jmp common_wrapper
%endmacro

%macro isr_val 1
	isr%1:
		cli
		push byte %1 ;we already have an error code on stack
		jmp common_wrapper
%endmacro

section .text
extern int_handler

global isr0
global isr1
global isr2
global isr3
global isr4
global isr5
global isr6
global isr7
global isr8
global isr9
global isr10
global isr11
global isr12
global isr13
global isr14
global isr15
global isr16
global isr17
global isr18
global isr19
global isr20
global isr21
global isr22
global isr23
global isr24
global isr25
global isr26
global isr27
global isr28
global isr29
global isr30
global isr31

isr_noval 0
isr_noval 1
isr_noval 2
isr_noval 3
isr_noval 4
isr_noval 5
isr_noval 6
isr_noval 7
isr_val   8
isr_noval 9
isr_val   10
isr_val   11
isr_val   12
isr_val   13
isr_val   14
isr_noval 15
isr_noval 16
isr_noval 17
isr_noval 18
isr_noval 19
isr_noval 20
isr_noval 21
isr_noval 22
isr_noval 23
isr_noval 24
isr_noval 25
isr_noval 26
isr_noval 27
isr_noval 28
isr_noval 29
isr_noval 30
isr_noval 31


common_wrapper:
	pusha
	push ds
	push es
	push fs
	push gs
	mov ax, 0x10 ;Kernel data segment descriptor
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov eax, esp ;save stack pointer
	push eax
	mov eax, int_handler
	call eax ;calling from a reg preserves eip
	pop eax
	pop gs
	pop fs
	pop es
	pop ds
	popa
	add esp, 8 ;move back the stack pointer, getting rid of the error code and isr num
	sti
	iret