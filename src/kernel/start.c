#include <screen.h>
#include <arch.h>

void kbegin(void)
{
	extern char *stack_top, *stack_bottom;
	/* Init screen */
	screen_init();
	clear_screen();

	arch_init();
	asm volatile ("int $0"); //generate an interrupt
	while (1);
}