#pragma once
#include <types.h>
#include <_types.h>

#define IDT_DESC_BITS_16    0x06    /* 00000110 */
#define IDT_DESC_BITS_32    0x0E    /* 00001110 */
#define IDT_DESC_RING1      0x40    /* 01000000 */
#define IDT_DESC_RING2      0x20    /* 00100000 */
#define IDT_DESC_RING3      0x60    /* 01100000 */
#define IDT_DESC_PRESENT    0x80    /* 10000000 */

void idt_add (uint8 num, uint8 flags, uint16 selector, uint32 offset);
void install_idt (uint16 selector);

void int_handler (struct regs_struct *regs);