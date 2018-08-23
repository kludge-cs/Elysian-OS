#ifndef _IDT_H
#define _IDT_H
#include <types.h>

void idt_add(uint8 num, uint8 flags, uint16 selector, uint32 offset);
void install_idt(uint16 selector);

void int_handler(struct regs_struct *regs);

#endif