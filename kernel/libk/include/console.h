#pragma once
#include <screen.h>
#include <stdarg.h>
void printf (char  *base,  ...);
void puts (char * text);
void putch (char ch);
void screen_clear (void);
void color_printf (enum color_e fg, enum color_e bg, char *text, ...);
void vprintf (char *base, va_list extra);
