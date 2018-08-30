OUTPUT_FORMAT(elf32-i386)
OUTPUT(Kernel.bin)
ENTRY(_start)

SECTIONS
{
	. = 0xC0100000;

	.multiboot : AT(ADDR(.multiboot) - 0xC0000000)
	{
		*(.multiboot)
	}
	
	.text ALIGN(4K) : AT(ADDR(.text) - 0xC0000000)
	{
		*(.text)
	}
	
	.rodata ALIGN(4K) : AT(ADDR(.rodata) - 0xC0000000)
	{
		*(.rodata)
	}

	. = 0xC0400000; /* data and bss are in an RW page together */
	
	.data ALIGN(4K) : AT(ADDR(.data) - 0xC0400000)
	{
		*(.data)
	}

	.bss ALIGN(4K) : AT(ADDR(.bss) - 0xC0400000)
	{
		*(.bss)
		*(.bss:)
	}
	
	PROVIDE(kernel_end = .);
}