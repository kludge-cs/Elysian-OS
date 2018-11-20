#include <arch.h>
#include <panic.h>

 __attribute__ ((cdecl))
void kbegin ()
{
	arch_init();
	/* panic("Reason: Nothing to do.        "); */
	while (1);
}