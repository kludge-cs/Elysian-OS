[BITS 32]

MBOOT_MAGIC    equ 0x1BADB002
MBOOT_ALIGN    equ 1<<0
MBOOT_MEM_INFO equ 1<<1
MBOOT_FLAGS    equ MBOOT_ALIGN | MBOOT_MEM_INFO

section .multiboot
align 4
	; Multiboot header format:
	; +==============+
	; | Magic Number | 0
	; +--------------+
	; |    Flags     | 4
	; +--------------+
	; |   Checksum   | 8
	; +--------------+
	; |    Extra     | 12...44
	; | depending on |
	; |   flags set  |
	; +==============+
mboot_header:
	dd MBOOT_MAGIC
	dd MBOOT_FLAGS
	dd -(MBOOT_MAGIC + MBOOT_FLAGS) ;checksum
	
section .text
global _start
_start:
	mov esp, stack_top
	cli ;we can't deal with interrupts yet
	extern kbegin
	call kbegin
.hang:
	hlt
	jmp .hang

global flush_gdt
extern gdt_pointer
flush_gdt:
	;Each segment selector has the following structure:
	; 15              3  2  1   0
	; +----------------+---+-----+
	; | Index [13 bit] | T | RPL |
	; +----------------+---+-----+
	; Where T is the table indicator, with 0 being GDT and 1 being LDT,
	; and RPL is the requestor privelage level (ring)

	lgdt [gdt_pointer]
	jmp dword 0x08:flush_end ;This is weird, but we far jump in order to set CS
flush_end:
	mov ax, 0x10 ;0x10 = 10000, so 0000000000010:0:00, being index 2 (data)
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	ret

section .bss
global stack_top
global stack_bottom
align 16
	stack_top:
	resb 16384 ;16 KiB for our stack
	stack_bottom: