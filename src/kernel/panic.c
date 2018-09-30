#include <console.h>
#include <screen.h>

 __attribute__ ((cdecl))
void panic (char message[])
{
	set_colors(White, Red);
	puts("\n==============================");
	puts("-------- KERNEL PANIC --------");
	puts("==============================");
	puts("                              ");
	puts("                              ");
	puts(message);
	puts("System halted.                ");
	while (1);
}