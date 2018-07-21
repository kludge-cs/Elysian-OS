%macro isr_stub 2
cli
push byte %1
push byte %2
jmp isr_stub_main
%endmacro

%macro isr_errorcode_stub 1
cli
push byte %1
jmp isr_common_stub
%endmacro

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
global isr32

;#  Description                         Error Code?
;--------------------------------------------------
;0  Division By Zero Exception             No
;1  Debug Exception                        No
;2  Non Maskable Interrupt Exception       No
;3  Breakpoint Exception                   No
;4  Into Detected Overflow Exception       No
;5  Out of Bounds Exception                No
;6  Invalid Opcode Exception               No
;7  No Coprocessor Exception               No
;8  Double Fault Exception                 Always 0
;9  Coprocessor Segment Overrun Exception  No
;10 Bad TSS Exception                      Yes
;11 Segment Not Present Exception          Yes
;12 Stack Fault Exception                  Yes
;13 General Protection Fault Exception     Yes
;14 Page Fault Exception                   Yes
;15 Unknown Interrupt Exception            No
;16 Coprocessor Fault Exception            No
;17 Alignment Check Exception (486+)       No
;18 Machine Check Exception (Pentium/586+) No
;..31 Reserved Exceptions                  No
;32..255 Free


isr0: isr_stub 0 0
isr1: isr_stub 0 1
isr2: isr_stub 0 2
isr3: isr_stub 0 3
isr4: isr_stub 0 4
isr5: isr_stub 0 5
isr6: isr_stub 0 6
isr7: isr_stub 0 7
isr8: isr_errorcode_stub 8 ;8 has an error code
isr9: isr_stub 0 9
isr10: isr_errorcode_stub 10 ;10-14 have error codes
isr11: isr_errorcode_stub 11
isr12: isr_errorcode_stub 12
isr13: isr_errorcode_stub 13
isr14: isr_errorcode_stub 14
isr15: isr_stub 0 15
isr16: isr_stub 0 16
isr17: isr_stub 0 17
isr18: isr_stub 0 18
isr19: isr_stub 0 19
isr20: isr_stub 0 20
isr21: isr_stub 0 21
isr22: isr_stub 0 22
isr23: isr_stub 0 23
isr24: isr_stub 0 24
isr25: isr_stub 0 25
isr26: isr_stub 0 26
isr27: isr_stub 0 27
isr28: isr_stub 0 28
isr29: isr_stub 0 29
isr30: isr_stub 0 30
isr31: isr_stub 0 31
isr32: isr_stub 0 32

isr_stub_main:
	pusha
	push ds ;data segment register
	push es ;extra segment register
	push fs ;another one of the above
	push gs ;and yet another

	mov ax, 0x10