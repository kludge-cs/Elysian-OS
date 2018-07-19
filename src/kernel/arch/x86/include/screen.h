#ifndef _SCREEN_H
#define _SCREEN_H

#define index(X, Y) ((Y * 80) + X)
#define setattr(chr, attr) (chr | (attr << 8))

int attrib;
unsigned int pos_x;
unsigned int pos_y;

void puts (char *text);
void putch (char chr);
void clear_screen (void);
void update_curs (void);
void update_scroll (void);
void set_color (unsigned char foreground, unsigned char background);
void screen_init(void);
#endif