; 8259 PIC stuff

PIC1_CTRL equ 0x20 ;Control register for the main PIC
PIC2_CTRL equ 0xA0 ;Control register for the secondary PIC
PIC1_DATA equ 0x21 ;Main PIC data register
PIC2_DATA equ 0xA1 ;Secondary PIC data register

IRQ_0 equ 32 ;mapping to 32-40
IRQ_8 equ 40 ;8 IRQs per PIC

%macro irq_macro 1
irq%1:
	cli
	push byte %1
	push byte (%1+32)
	jmp irq_common
%endmacro


section .text
extern irq_handler_main
global irq0
global irq1
global irq2
global irq3
global irq4
global irq5
global irq6
global irq7
global irq8
global irq9
global irq10
global irq11
global irq12
global irq13
global irq14
global irq15

irq_macro 0
irq_macro 1
irq_macro 2
irq_macro 3
irq_macro 4
irq_macro 5
irq_macro 6
irq_macro 7
irq_macro 8
irq_macro 9
irq_macro 10
irq_macro 11
irq_macro 12
irq_macro 13
irq_macro 14
irq_macro 15

;I have to do this shit all over again... I could make it a function that changes the call based on a stack argument or something, or at least a macro, but eh
irq_common:
	pusha
	push ds
	push es
	push fs
	push gs
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov eax, esp
	push eax
	mov eax, irq_handler_main
	call eax
	pop eax
	pop gs
	pop fs
	pop es
	pop ds
	popa
	add esp, 8
	iret

global pic_init
pic_init:
	;ICW 1 format:
	;-------------------------------------------------------------------------------------------------------------------------------------
	; # | Val  | Description
	;---|------|--------------------------------------------------------------------------------------------------------------------------
	; 0 | IC4  | If set, the PIC expects to recieve IC4 during initialization.
	; 1 | SNGL | If set, there is only one PIC in system. If cleared, PIC is cascaded with slave PICs, and ICW3 must be sent to controller.
	; 2 | ADI  | If set, CALL address interval is 4, else 8. This is useually ignored by x86, and is default to 0
	; 3 | LTIM | If set, use in Level Triggered Mode. (otherwise edge trigger)
	; 4 | 1    | Set 1 if PIC is to be initialized
	; 5 | 0    | Must be 0
	; 6 | 0    | Must be 0
	; 7 | 0    | Must be 0
	;--------------------------------------------------------------------------------------------------------------------------------------

	; send ICW 1:
	mov al, 00010001b
	out PIC1_CTRL, al
	out PIC2_CTRL, al


	; send ICW 2
	mov al, IRQ_0
	out PIC1_DATA, al
	mov al, IRQ_8
	out PIC2_DATA, al

	; ICW 3 for main PIC:
	;-------------------------------------------------------------------------------
	; #   | Value | Description
	;-----|-------|-----------------------------------------------------------------
	; 0-7 | S0-S7 | Specifies what Interrupt Request (IRQ) is connected to slave PIC
	;-------------------------------------------------------------------------------
	;Uses binary format where # of set bit = # of IRQ line

	; ICW 3 for secondary PIC:
	;---------------------------------------------------------
	; 0-2 | ID0 | IRQ number the master PIC uses to connect to
	; 3-7 | 0   | Reserved, must be 0
	;---------------------------------------------------------
	;Normal binary


	;send ICW 3
	mov al, 00000100b ;Bit #2 is set, IRQ line 2 is used on x86
	out PIC1_DATA, al
	
	mov al, 00000_010b
	out PIC2_DATA, al
	
	; ICW 4
	;---------------------------------------------------------------------------------------------------------------------------------
	; 0   | uPM  | If set (1), it is in 80x86 mode. Cleared if MCS-80/86 mode
	; 1   | AEOI | If set, on the last interrupt acknowledge pulse, controller automatically performs End of Interrupt (EOI) operation
	; 2   | M/S  | Only use if BUF is set. If set (1), selects buffer master. Cleared if buffer slave.
	; 3   | BUF  | If set, controller operates in buffered mode
	; 4   | SFNM | Special Fully Nested Mode. Used in systems with a large amount of cascaded controllers.
	; 5-7 | 0    | Reserved, must be 0
	;----------------------------------------------------------------------------------------------------------------------------------
	
	;send ICW 4
	mov al, 0000_0001b
	out PIC1_DATA, al
	out PIC2_DATA, al

	;Now we clear the data registers
	mov al, 0
	out PIC1_DATA, al
	out PIC2_DATA, al

	ret ;This is still a function...