#include <arch.h>
#include <types.h>
#include <panic.h>

 __attribute__ ((cdecl))
void kbegin ()
{
	//extern char *stack_top, *stack_bottom;

	arch_init();

	while(1);
}