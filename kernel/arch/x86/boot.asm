[BITS 32]

MBOOT_MAGIC    equ 0x1BADB002
MBOOT_ALIGN    equ 1<<0
MBOOT_MEM_INFO equ 1<<1
MBOOT_VID_MODE equ 1<<2
MBOOT_FLAGS    equ MBOOT_ALIGN | MBOOT_MEM_INFO | MBOOT_VID_MODE

VIDEO_USE_TEXT equ 1
VIDEO_HEIGHT   equ 115 ;chars here
VIDEO_WIDTH    equ 58

PD_PRESENT      equ 00000001b
PD_READWRITE    equ 00000010b ;Read & Write
PD_USER         equ 00000100b ;if not set, only supervisor can access
PD_WRITETHROUGH equ 00001000b ;writethrough if enabled, writeback if not
PD_NOCACHE      equ 00010000b ;disable caching if set
PD_ACCESSED     equ 00100000b ;should set set if page has been accessed
;this missing bit is always zero
PD_SIZE         equ 10000000b ;4kib vs 4mib

PT_PRESENT      equ 000000001b
PT_READWRITE    equ 000000010b
PT_USER         equ 000000100b
PT_WRITETHROUGH equ 000001000b
PT_NOCACHE      equ 000010000b
PT_ACCESSED     equ 000100000b
PT_DIRTY        equ 001000000b ;has it been written to
PT_PAT          equ 010000000b ;fuck intel
PT_GLOBAL       equ 100000000b ;prevents the TLB from updating the address in its cache if CR3 is reset

PAGE_SIZE equ 0x1000
HUGE_SIZE equ 0x400000

VIRT_BASE equ 0xC0000000
PD_INDEX  equ VIRT_BASE >> 22

%define PHYS(x) (x - VIRT_BASE)

STACK_SIZE equ 0x4000 ;16KiB

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
extern ro_end
extern rw_end
_start:
	mov [PHYS(multiboot_magic_check)], eax
	mov [PHYS(multiboot_info_ptr)], ebx

	mov dword [PHYS(page_dir.ident)], (PD_PRESENT | PD_READWRITE | PD_SIZE)

	mov eax, PHYS(k_tab)
	or eax, PD_PRESENT | PD_READWRITE
	mov dword [PHYS(page_dir.kentry)], eax

	mov eax, PHYS(page_dir)
	or eax, (PD_PRESENT | PD_READWRITE)
	mov dword [PHYS(page_dir.self)], eax

	mov eax, (PT_PRESENT | PT_READWRITE)
	mov ebx, 0 ;current index
	mov ecx, PHYS(k_tab)
.fill_tab:
	mov [ecx + ebx * 4], eax
	add eax, 4096
	inc ebx
	cmp ebx, 1024
	jne .fill_tab

;actually enable paging and stuff
	lea eax, [PHYS(page_dir)]
	mov cr3, eax
 
	mov eax, cr4
	or eax, 0x00000010 ;enable PSE (4MiB pages)
	mov cr4, eax

	mov eax, cr0
	or eax, 0x80010000 ;enable paging, WP
	mov cr0, eax

	lea eax, [higher_half_start]
	jmp eax

higher_half_start:
	;now in higher half
	mov dword [page_dir.ident], 0 ;zero out first entry of page dir
	invlpg [0] ;get rid of it
	;boom, done

	mov esp, stack_top
	cli ;we can't do interrupts yet

	call kbegin
	cli
hang:
	hlt
	jmp hang

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

section .data
global multiboot_magic_check
global multiboot_info_ptr
global page_dir
global k_tab
multiboot_magic_check: dd 0
multiboot_info_ptr: dd 0
align 4096
page_dir:
	.ident:
		dd 0;PD_PRESENT | PD_READWRITE | PD_SIZE

	times (PD_INDEX - 1) dd 0

	.kentry:
		dd 0;PD_PRESENT | PD_READWRITE | PD_SIZE

	times (1024 - (PD_INDEX - 2)) dd 0

	.self:
		dd 0
align 4096
k_tab:
	times 1024 dd 0

section .bss
global stack_bottom
global stack_top
align 0x1000
stack_bottom:
	resb STACK_SIZE
stack_top: