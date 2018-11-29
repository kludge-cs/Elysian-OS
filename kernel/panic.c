#include <console.h>
#include <screen.h>
#include <stdarg.h>

__attribute__((cdecl))
void panic (char * msg, ...)
{
	va_list extra;
#if ARCH  == x86
	__asm__ volatile ("cli");
#endif
	set_colors(White, Red);
	puts("\n==============================");
	puts("-------- KERNEL PANIC --------");
	puts("==============================");
	puts("                              ");
	puts("                              ");
	va_start(extra, msg);
	vprintf(msg, extra);
	puts("System halted.                ");
	while (1);
}