#include <string.h>
#include <ports.h>
#include <screen.h>
#include <gdt.h>

void kbegin(void)
{
	extern char *stack_top, *stack_bottom, stack_size;
	
	/* Init screen */
	screen_init();
	clear_screen();

	puts("Initilaizing GDT...");
	install_gdt();
	
	while(1); /* Do nothing */
}