#include <screen.h>
#include <arch.h>

void kbegin(void)
{
	extern char *stack_top, *stack_bottom;

	arch_init();
	while (1);
}