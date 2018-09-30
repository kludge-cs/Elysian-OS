#pragma once
#include <screen.h>
void puts (char *text);
void putch (char ch);
void screen_clear (void);
void color_puts (char *text, enum color_e fg, enum color_e bg);