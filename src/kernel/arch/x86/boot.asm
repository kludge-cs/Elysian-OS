[BITS 32]

MBOOT_MAGIC    equ 0x1BADB002
MBOOT_ALIGN    equ 1<<0
MBOOT_MEM_INFO equ 1<<1
MBOOT_VID_MODE equ 1<<2
MBOOT_FLAGS    equ MBOOT_ALIGN | MBOOT_MEM_INFO | MBOOT_VID_MODE

VIDEO_USE_TEXT equ 1
VIDEO_HEIGHT   equ 115 ;chars
VIDEO_WIDTH    equ 58

KERNEL_RO_VIRT_BASE equ 0xC0000000 ;rodata, text
KERNEL_RW_VIRT_BASE equ 0xC0400000 ;data, bss

KERNEL_PD_INDEX  equ KERNEL_RO_VIRT_BASE >> 22

PD_PRESENT      equ 0000001b
PD_READWRITE    equ 0000010b ;Read & Write
PD_USER         equ 0000100b ;if not set, only supervisor can access
PD_WRITETHROUGH equ 0001000b ;writethrough if enabled, writeback if not
PD_NOCACHE      equ 0010000b ;disable caching if set
PD_ACCESSED     equ 0100000b ;should set set if page has been accessed
PD_SIZE         equ 1000000b ;4kib vs 4mib


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

section .data
align 0x1000
global mboot_magic_check
global mboot_info_struct
multiboot_magic_check: dd 0
multiboot_info: resb 115
boot_page_dir:
	;lower half
	dd (PD_PRESENT | PD_SIZE) ;rodata, text
	dd (PD_PRESENT | PD_READWRITE | PD_SIZE) ;data, bss

	;make blank pages for everything in the middle
	times (KERNEL_PD_INDEX - 2) dd 0

	;higher half
	dd (PD_PRESENT | PD_SIZE)
	dd (PD_PRESENT | PD_READWRITE | PD_SIZE)

	times (1024 - KERNEL_PD_INDEX - 2) dd 0 ;fill rest of dir

section .text
global _start
extern kbegin
_start:
	mov [multiboot_magic_check], eax
	;mov [multiboot_info_struct], [ebx]
	mov eax, (boot_page_dir - KERNEL_RW_VIRT_BASE)
	mov cr3, eax

	mov eax, cr4
	or eax, 0x10 ;enable PSE (4MiB pages)
	mov cr4, eax

	mov eax, cr0
	or eax, 0x80010001 ;enable paging, WP, PE
	mov cr0, eax

	lea eax, [higher_half_start]
	jmp eax

higher_half_start:
	;now in higher half
	mov dword [boot_page_dir], 0 ;zero out first entry of page dir
	invlpg [0] ;get rid of it
	;boom, done

	mov esp, stack_top ;set up stack
	cli ;we can't do interrupts yet

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

section .bss
global stack_bottom
global stack_top
align 0x1000
stack_bottom:
	resb 0x4000 ;16KiB stack
stack_top: