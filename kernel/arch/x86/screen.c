#include <types.h>
#include <string.h>
#include <screen.h>
#include <ports.h>

static int attrib;
unsigned short *output_pointer;

extern void blink_on();
extern void blink_off();
extern void blink_toggle();

void raw_putch (char ch)
{
	unsigned short *loc;

	loc = output_pointer + index(pos_x, pos_y);
	*loc = setattr(ch, attrib);
}

void update_curs (void)
{
	unsigned int pos_index = index(pos_x, pos_x);

	/* Send command to indicies 14 and 15 of the control register of the VGA controller. */
	port_out(0x3D4, 14);
	port_out(0x3D5, pos_index >> 8);

	port_out(0x3D4, 15);
	port_out(0x3D5, pos_index);
}

void clear_screen (void)
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
		temp = pos_y - 25 + 1;
		/* copy screen up */
		memcpy(output_pointer, output_pointer + temp * 80, (25 - temp) * 80 * 2);
		/* blank out last line */
		memsetw(output_pointer + index(0, 24), blank, 80);
		
		pos_y = 24;
	}
}

void set_colors (enum color_e fg, enum color_e bg)
{
	/* Top 4 bytes is bg, bottom 4 is fg */
	attrib = ((uint8_t)bg << 4) | ((uint8_t)fg & 0x0F);
}

void screen_init (void)
{
	blink_off();
	output_pointer = (unsigned short *)0xC00B8000;
	set_colors(White, Black);
	clear_screen();
}

void get_colors (enum color_e * colors)
{
	colors[1] = attrib >> 4; /* Background */
	colors[0] = attrib & 0x0F; /* Foreground */
}