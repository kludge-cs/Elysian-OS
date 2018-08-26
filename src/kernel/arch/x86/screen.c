#include <util.h>
#include <screen.h>
#include <ports.h>

static int attrib;
unsigned short *output_pointer;

extern void blink_on();
extern void blink_off();
extern void blink_toggle();


void puts(char *text)
{
	while(*text)
		putch(*text++);
	putch('\n');
}

void putch(char chr)
{
	unsigned short *loc;

	switch (chr)
	{
		case 0x08: /* Backspace */
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
			loc = output_pointer + index(pos_x, pos_y); /* Get correct location */
			*loc = setattr(chr, attrib); /* Set memory */
			pos_x++;
	}
	update_scroll();
	update_curs();
}

void update_curs(void)
{
	unsigned int pos_index = index(pos_x, pos_x);

	/* Send command to indicies 14 and 15 of the control register of the VGA controller. */
	port_out(0x3D4, 14);
	port_out(0x3D5, pos_index >> 8);

	port_out(0x3D4, 15);
	port_out(0x3D5, pos_index);
}

void clear_screen(void)
{
	const unsigned int blank = setattr(' ', attrib);
	unsigned short *loc;
	unsigned int i, j;

	for (i=0; i <= 24; i++)
	{
		for (j=0; j <= 80; j++)
		{
			loc = output_pointer + index(j, i);
			*loc = blank;
		}
	}

	pos_x = 0;
	pos_y = 0;
	update_curs();
}

void update_scroll (void)
{
	unsigned int temp;
	const unsigned int blank = setattr(' ', attrib);
	
	if (pos_y >= 25) /* Check if we are at end of screen */
	{
		/* move up by two lines */
		temp = pos_y - 25 + 1; //should be just 1
		/* copy screen up */
		memcpy(output_pointer, output_pointer + temp * 80, (25 - temp) * 80 * 2);
		/* blank out last line */
		memsetw(output_pointer + index(0, 24), blank, 80);
		
		pos_y = 24;
	}
}

void set_color(unsigned char fg, unsigned char bg)
{
	/* Top 4 bytes is bg, bottom 4 is fg */
	attrib = (bg << 4) | (fg & 0x0F);
}

void screen_init(void)
{
	attrib = 0x8F;
	output_pointer = (unsigned short *)0xB8000;
	blink_off();
}