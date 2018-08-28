[BITS 32]

MBOOT_MAGIC    equ 0x1BADB002
MBOOT_ALIGN    equ 1<<0
MBOOT_MEM_INFO equ 1<<1
MBOOT_VID_MODE equ 1<<2
MBOOT_FLAGS    equ MBOOT_ALIGN | MBOOT_MEM_INFO | MBOOT_VID_MODE

VIDEO_USE_TEXT equ 1
VIDEO_HEIGHT   equ 115 ;chars
VIDEO_WIDTH    equ 58

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
	dd 0, 0, 0, 0, 0 ;header_addr, load_addr, load_end_addr, bss_end_addr, entry_addr
	dd VIDEO_USE_TEXT
	dd VIDEO_HEIGHT
	dd VIDEO_WIDTH
	dd 32 ;color depth, ignored in text mode
	
section .text
global _start
extern kbegin
extern stack_top ;defined in linker script
_start:
	mov esp, stack_top ;load stack before doing anything
	cli ;we can't deal with interrupts yet
	push ebx ;multiboot info structure
	push eax ;multiboot magic num
	call kbegin
	cli
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