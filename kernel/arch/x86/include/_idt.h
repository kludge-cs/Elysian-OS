#pragma once
#include <types.h>

struct regs_s
{
	uint32_t gs, fs, es, ds;
	uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; /* pushed by 'pusha' */
	uint32_t int_num, err_code;
	uint32_t eip, cs, eflags, useresp, ss; /* Auto-pushed */ 
};

#define IDT_DESC_BITS_16    0x06    /* 00000110 */
#define IDT_DESC_BITS_32    0x0E    /* 00001110 */
#define IDT_DESC_RING1      0x40    /* 01000000 */
#define IDT_DESC_RING2      0x20    /* 00100000 */
#define IDT_DESC_RING3      0x60    /* 01100000 */
#define IDT_DESC_PRESENT    0x80    /* 10000000 */

void idt_add (uint8_t num, uint8_t flags, uint16_t selector, uint32_t offset);
void install_idt (uint16_t selector);

void int_handler (struct regs_s *regs);