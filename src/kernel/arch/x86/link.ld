OUTPUT_FORMAT(elf32-i386)
OUTPUT(Kernel.bin)
ENTRY(_start)

SECTIONS
{
	. = 0xC0100000;
	
	.text : AT(ADDR(.text) - 0xC0000000)
	{
		*(.multiboot)
		*(.text)
	}
	
	.rodata : AT(ADDR(.rodata) - 0xC0000000)
	{
		*(.rodata)
	}
	
	. = ALIGN(4K);
	PROVIDE(ro_end = .);

	.data : AT(ADDR(.data) - 0xC0000000)
	{
		*(.data)
	}

	.bss : AT(ADDR(.bss) - 0xC0000000)
	{
		*(.bss)
	}
	PROVIDE(rw_end = .);
}