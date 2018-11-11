#pragma once
#include <screen.h>
#include <stdarg.h>
inline void puts (char *base,  ...);
void putch (char ch);
void screen_clear (void);
void color_puts (char *text, enum color_e fg, enum color_e bg);
void vputs (char *base, va_list extra);
