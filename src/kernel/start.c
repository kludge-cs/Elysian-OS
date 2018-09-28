#include <arch.h>
#include <types.h>

void __attribute__((__cdecl__)) kbegin()
{
	//extern char *stack_top, *stack_bottom;

	arch_init();
	while(1);
}