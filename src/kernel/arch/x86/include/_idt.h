#pragma once
#include <types.h>

struct regs_s
{
	unsigned int gs, fs, es, ds;
	unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax; /* pushed by 'pusha' */
	unsigned int int_num, err_code;
	unsigned int eip, cs, eflags, useresp, ss; /* Auto-pushed */ 
};

#define IDT_DESC_BITS_16    0x06    /* 00000110 */
#define IDT_DESC_BITS_32    0x0E    /* 00001110 */
#define IDT_DESC_RING1      0x40    /* 01000000 */
#define IDT_DESC_RING2      0x20    /* 00100000 */
#define IDT_DESC_RING3      0x60    /* 01100000 */
#define IDT_DESC_PRESENT    0x80    /* 10000000 */

void idt_add (uint8 num, uint8 flags, uint16 selector, uint32 offset);
void install_idt (uint16 selector);

void int_handler (struct regs_s *regs);