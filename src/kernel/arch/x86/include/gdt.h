#ifndef _GDT_H
#define _GDT_H
#include <types.h>

/******************************************
* GDT access field:                       *
*   7  6   5   4  3    0                  *
* +---+-----+----+------+                 *
* | P | DPL | DT | Type |                 *
* +---+-----+----+------+                 *
* P - segment is present                  *
* DPL - Ring number                       *
* DT - Descriptor type                    *
* Type - Type                             *
*******************************************
* GDT granularity field:                  *
*   7   6   5   4  3      0               *
* +---+---+---+---+--------+              *
* | G | D | 0 | A | Length |              *
* +---+---+---+---+--------+              *
* G - Granularity (0 for byte, 1 for 4KB) *
* D - Operand size                        *
* A - Available for system                *
******************************************/
struct gdt_ptr_struct
{
   uint16 limit;               // The upper 16 bits of all selector limits.
   uint32 base;                // The address of the first gdt_entry_t struct.
} __attribute__((packed));

struct gdt_entry
{
	uint16 limit_low;
	uint16 base_low;
	uint8  base_middle;
	uint8  access;
	uint8  granularity;
	uint8  base_high; 
} __attribute__((packed));

struct gdt_entry gdt[3];
struct gdt_ptr_struct gdt_pointer;

void install_gdt();
#endif