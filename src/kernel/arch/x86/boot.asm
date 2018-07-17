[BITS 32]


section .multiboot
align 4
	MBOOT_MAGIC    equ 0x1BAD002
	MBOOT_ALIGN    equ 1<<0
	MBOOT_MEM_INFO equ 1<<1
	MBOOT_FLAGS    equ MBOOT_ALIGN | MBOOT_MEM_INFO
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

	dd MBOOT_MAGIC
	dd MBOOT_FLAGS
	dd -(MBOOT_MAGIC + MBOOT_FLAGS) ;checksum

section .bss
align 16
	resb 16384 ;16 KiB for our stack

section .text
global _start
_start:
	extern _stack_top ;defined in linker script
	mov esp, _stack_top
	cli
.hang:
	hlt
	jmp .hang
