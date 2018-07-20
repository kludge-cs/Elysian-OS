#ifndef _IDT_H
#define _IDT_H
#include <types.h>

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

struct idt_entry idt[256]; /* We don't actually use all of these */
struct idt_pointer_struct idt_pointer;


void idt_add(uint8 num, uint32 offset, uint16 selector, uint8 flags);
void install_idt();

#endif