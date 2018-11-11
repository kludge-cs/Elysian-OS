#include <screen.h>
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

void vputs (char *base, va_list extra)
{
	char *current;
	
	while(*base)
	{
		if (*base == '%' && *(base-1) != '\\')
		{
			current = va_arg(extra, char *);
			while (*current)
				putch(*current++);
			base++;
		}
		else
			putch(*base++);
	}
	putch('\n');
}

inline void puts (char *base, ...)
{
	va_list extra;
	va_start(extra, base);
	vputs(base, extra);
}

void screen_clear (void)
{
	uint8 i;
	for (i=0; i <= 24; i++)
	{
		uint8 j;
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

void color_puts (char *text, enum color_e fg, enum color_e bg)
{
	enum color_e colors[2];
	get_colors(colors);
	set_colors(fg, bg);
	puts(text);
	set_colors(colors[0], colors[1]);
}