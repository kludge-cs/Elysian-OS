#include <console.h>

 __attribute__ ((cdecl))
void panic (char message[])
{
	screen_clear();
	puts("==============================");
	puts("-------- KERNEL PANIC --------");
	puts("==============================\n\n");

	puts(message);
	puts("Debugger unimplemented!");
	puts("System halted.");
	while (1);
}