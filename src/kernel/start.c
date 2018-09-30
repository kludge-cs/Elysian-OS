#include <arch.h>
#include <panic.h>

 __attribute__ ((cdecl))
void kbegin ()
{
	//extern char *stack_top, *stack_bottom;

	arch_init();
	panic("Reason: Nothing to do.        ");
}