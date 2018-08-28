#include <arch.h>
#include <types.h>

void __attribute__((__cdecl__)) kbegin(uint32 multiboot_magic, uint32 multiboot_addr)
{
	extern char *stack_top, *stack_bottom;

	if (arch_init(multiboot_magic, multiboot_addr) != 0)
		; //dead loop, but we already have one, so nothing goes here
	while(1);
}