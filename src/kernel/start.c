#include <string.h>
#include <screen.h>
//#include <gdt.h>
//#include <idt.h>
#include <arch.h>

void kbegin(void)
{
	extern char *stack_top, *stack_bottom;
	/* Init screen */
	screen_init();
	clear_screen();

	arch_init();
	while (1);
}