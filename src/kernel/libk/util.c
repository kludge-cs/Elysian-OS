#include <types.h>
#include <screen.h>

void * memcpy( void * s1, const void * s2, int64 n )
{
    int8 * dest = (int8 *) s1;
    const int8 * src = (const int8 *) s2;
    while ( n-- )
    {
        *dest++ = *src++;
    }
    return s1;
}

void * memset( void * dest, int32 val, int64 size )
{
    uint8 * buf = (uint8 *) dest;
    while ( size-- )
    {
        *buf++ = (uint8) val;
    }
    return dest;
}

void * memsetw(uint16 * dest, uint16 val, int64 n )
{
	for (; n != 0; n--)
		*dest++ = val;
	return dest;
}

void puts(char *text)
{
	while(*text)
		putch(*text++);
	putch('\n');
}