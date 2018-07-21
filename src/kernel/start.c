#include <string.h>
#include <ports.h>
#include <screen.h>
#include <gdt.h>
#include <idt.h>

void kbegin(void)
{
	extern char *stack_top, *stack_bottom, stack_size;
	
	/* Init screen */
	screen_init();
	clear_screen();

	puts("Initializing GDT...");
	install_gdt();

	puts("Initializing IDT...");
	install_idt(0x8);
	
	while(1); /* Do nothing */
}
