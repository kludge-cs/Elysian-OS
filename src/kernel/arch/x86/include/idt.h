#ifndef _IDT_H
#define _IDT_H
#include <types.h>

#define IDT_DESC_BITS_16    0x06    //00000110
#define IDT_DESC_BITS_32    0x0E    //00001110
#define IDT_DESC_RING1      0x40    //01000000
#define IDT_DESC_RING2      0x20    //00100000
#define IDT_DESC_RING3      0x60    //01100000
#define IDT_DESC_PRESENT    0x80    //10000000

/* * * * * * * * * * * * * * *
* Type_attr is as follows:    *
*   7  6   5  4  3         0  *
* +---+-----+---+-----------+ *
* | P | DPL | S | Gate Type | *
* +---+-----+---+-----------+ *
*                             *
* P - Segment is present      *
* DPL - Ring number           *
* S - Storage segment         *
* * * * * * * * * * * * * * * */

struct idt_entry
{
	uint16 offset_low;
	uint16 selector;
	uint8  zero; //unused
	uint8  type_attr;
	uint16 offset_high;
} __attribute__((packed));

struct idt_pointer_struct
{
	uint16 limit;
	uint32 base;
} __attribute__((packed));

struct idt_entry idt[256];
struct idt_pointer_struct idt_pointer;


void idt_add(uint8 num, uint8 flags, uint16 selector, uint32 *handler);
void install_idt(uint16 selector);

#endif