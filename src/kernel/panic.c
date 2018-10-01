#include <console.h>
#include <screen.h>
#include <stdarg.h>

__attribute__((cdecl))
void panic (char * msg, ...)
{
	va_list extra;
	set_colors(White, Red);
	puts("\n==============================");
	puts("-------- KERNEL PANIC --------");
	puts("==============================");
	puts("                              ");
	puts("                              ");
	va_start(extra, msg);
	vputs(msg, extra);
	puts("System halted.                ");
	while (1);
}