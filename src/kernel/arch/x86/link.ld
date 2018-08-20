OUTPUT_FORMAT(elf32-i386)
OUTPUT(Kernel.bin)
ENTRY(_start)

SECTIONS
{
	. = 0x00100000; /* Multiboot requires kernels to be placed at 1M or above. */

	.text : ALIGN(4K)
	{
		*(.multiboot)
		*(.text)
	}
	
	.rodata : ALIGN(4K)
	{
		*(.rodata)
	}

	.data : ALIGN(4K)
	{
		*(.data)
	}

	.bss : ALIGN(4K)
	{
		*(.bss)
	}
	
	PROVIDE(kernel_end = .);
}