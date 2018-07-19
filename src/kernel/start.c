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
	
	install_gdt();
	/* It doesn't get here but how the fuck */
	asm volatile ("xchgw %bx, %bx");

	puts("Hello world!  GDT initialized.  Loading...");
	
	while(1); /* Do nothing */
}