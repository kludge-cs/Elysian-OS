#ifndef _SCREEN_H
#define _SCREEN_H

#define index(X, Y) ((Y * 80) + X)
#define setattr(chr, attr) (chr | (attr << 8))

enum color
{
	Black,
	Blue,
	Green,
	Cyan,
	Red,
	Magenta,
	Brown,
	Light_Grey,
	Dark_Grey,
	Light_Blue,
	Light_Green,
	Light_Cyan,
	Light_Red,
	Light_Magenta,
	Light_Brown,
	White,
};

unsigned int pos_x;
unsigned int pos_y;

void putch (char chr);
void clear_screen (void);
void update_curs (void);
void update_scroll (void);
void set_color (enum color foreground, enum color background);
void screen_init(void);
#endif