#include <types.h>

uint32_t strlen(char * str)
{
	char * temp;
	for(temp = str; *temp; temp++);
	return (uint32_t) (temp - str);
}

void * memcpy( void * s1, const void * s2, int64_t n )
{
    int8_t * dest = (int8_t *) s1;
    const int8_t * src = (const int8_t *) s2;
    while ( n-- )
    {
        *dest++ = *src++;
    }
    return s1;
}

void * memset( void * dest, int32_t val, int64_t size )
{
    uint8_t * buf = (uint8_t *) dest;
    while ( size-- )
    {
        *buf++ = (uint8_t) val;
    }
    return dest;
}

void * memsetw(uint16_t * dest, uint16_t val, int64_t n )
{
	for (; n != 0; n--)
		*dest++ = val;
	return dest;
}

char * reverse(char * str, uint32_t n)
{
	char * end;
	char * current;

	end = str + n - 1;

	for (current = str; current < end; current++, end--)
	{
		*current ^= *end;
		*end ^= *current;
		*current ^= *end;
	}
	return str;
}