#include <screen.h>
#include <string.h>
#include <stdarg.h>

void putch (char ch)
{
	switch (ch)
	{
		case 0x08: /* Backspace */
			raw_putch(' ');
			if (pos_x != 0)
				pos_x--;
			else if (pos_y != 0) /* (pos_x == 0) && (pos_y != 0) */
				pos_y--;
			break;
			
		case 0x09: /* Tab */
			if (pos_x >= 80)
			{
				pos_x = 0;
				pos_y++;
			}
			pos_x = (pos_x + 4) & ~(4 - 1); /* Increment X to the next place where it is divisible by 4 */
			break;

		case '\r': /* Carriage return */
			pos_x = 0;
			break;

		case '\n': /* newline */
			pos_x = 0;
			pos_y++;
			break;

		default:
			if (pos_x >= 80)
			{
				pos_x = 0;
				pos_y++;
			}
			raw_putch(ch);
			pos_x++;
	}
	update_scroll();
	update_curs();
}

void puts (char * base)
{
	while(*base)
	{
		putch(*base++);
	}
	putch('\n');
}

void vprintf (char *base, va_list extra)
{
	uint64_t val_x;
	char * val_s;
	char * buf = "0000000000000000";
	const char charset[17] = "0123456789ABCDEF";
	int32_t pos;
	uint32_t digits;
	char * tmp;
	
	while(*base)
	{
		if (*base == '%' && *(base-1) != '\\')
		{
			base++;
			switch(*base)
			{
				case 's':
					val_s = va_arg(extra, char *);
					while (*val_s) putch(*val_s++);
					break;

				case 'x': case 'y':
					
					if (*base == 'x')
					{
						val_x = va_arg(extra, uint32_t);
						tmp = buf + 8;
						digits = 8;
					}
					else
					{
						val_x = va_arg(extra, uint64_t);
						tmp = buf;
						digits = 16;
					}
					pos = digits - 1;
				
					while (pos >= 0)
					{
						tmp[pos] = charset[val_x % 16];
						pos--;
						val_x /= 16;
					}
				
					while (*tmp) putch(*tmp++);
					break;
				default:
					putch('%');
					putch(*(base+1));
			}
		}
		else putch(*base);

		base++;
	}
	putch('\n');
}

void printf (char *base, ...)
{
	va_list extra;
	va_start(extra, base);
	vprintf(base, extra);
}

void screen_clear (void)
{
	uint8_t i;
	for (i=0; i <= 24; i++)
	{
		uint8_t j;
		for (j=0; j <= 80; j++)
		{
			pos_x = j;
			pos_y = i;
			update_curs();
			raw_putch(' ');
		}
	}
	pos_x = 0;
	pos_y = 0;
	update_curs();
}

void color_printf (enum color_e fg, enum color_e bg, char *base, ...)
{
	enum color_e colors[2];

	va_list extra;
	va_start(extra, base);

	get_colors(colors);
	set_colors(fg, bg);
	vprintf(base, extra);
	set_colors(colors[0], colors[1]);
}