#include <arch.h>
#include <types.h>

void __attribute__((__cdecl__)) kbegin()
{
	extern char *stack_top, *stack_bottom;

	if (arch_init() != 0)
		; //dead loop, but we already have one, so nothing goes here
	while(1);
}