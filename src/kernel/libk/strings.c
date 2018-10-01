#include <types.h>

uint32 strlen(char * str)
{
	char * temp;
	for(temp = str; *temp; temp++);
	return (uint32) (temp - str);
}

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

char * reverse(char * str)
{
	char * end;
	char * current;

	end = str + strlen(str) - 1;

	for (current = str; current < end; current++, end--)
	{
		*current ^= *end;
		*end ^= *current;
		*current ^= *end;
	}
	return str;
}

char * tohex(uint32 val, char * buf)
{
	uint32 digits;
	uint32 pos;
	const char charset[17] = "0123456789ABCDEF";

	digits = strlen(buf);
	pos = 0;

	while(pos <= digits)
	{
		buf[pos] = charset[val % 16];
		val /= 16;
		pos++;
	}

	return reverse(buf);
}