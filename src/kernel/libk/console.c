#include <screen.h>
void puts(char *text)
{
	while(*text)
		putch(*text++);
	putch('\n');
}